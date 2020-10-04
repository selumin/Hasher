#include "QtSocketIPC.h"

#define LOG_TAG "QtIPC"
#include "Logger.h"

std::atomic<ClientId_t> QtSocketIpcServer::sNextClientId { 0 };

QtSocketIpcServer::QtSocketIpcServer(IIPCServer::onDataReceived cb, QObject *parent) :
    QObject(parent), IIPCServer(cb)
{}

QtSocketIpcServer::~QtSocketIpcServer()
{}

bool QtSocketIpcServer::registerService(ServiceDescriptor service)
{
    mServer.reset( new QTcpServer(this) );
    if (!mServer) {
        LOG_ERR << "Unable to create server socket" << std::endl;
        return false;
    }

    if (!mServer->listen(QHostAddress(QString(service.serviceAddress.c_str())), service.servicePort)) {
        LOG_ERR << "Unable to bind server socket" << std::endl;
        mServer->close();
        return false;
    }

    LOG_INFO << "Service registered host=" << service.serviceAddress
             << " port=" << service.servicePort << std::endl;

    connect(mServer.get(), &QTcpServer::newConnection, this, &QtSocketIpcServer::handleNewClientConnection);

    return true;
}

bool QtSocketIpcServer::unregisterService()
{
    mServer->close();
    return true;
}

int QtSocketIpcServer::writeToClient(ClientId_t clientId, const ByteArray buffer)
{
    const auto conn = mConnections.find(clientId);
    if (conn == mConnections.end()) {
        LOG_ERR << "There is no client with id=" << clientId << std::endl;
        return -1;
    }

    const auto ret =  conn->second->write(buffer.data());
    if (ret < 0) LOG_ERR << "Sending data to the client=" << clientId << " failed" << std::endl;

    return ret;
}

void QtSocketIpcServer::handleNewClientConnection()
{
    QTcpSocket* socket = mServer->nextPendingConnection();
    mConnections[generateNextClientId()] = socket;

    LOG_INFO << "New client connection id=" << sNextClientId << std::endl;

    connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(handleNewClientRequest()));
}

void QtSocketIpcServer::handleNewClientRequest()
{
    QTcpSocket* clientSocket = (QTcpSocket*)sender();
    QByteArray data = clientSocket->readAll();

    if (!mCallback) {
        LOG_ERR << "Callback is not set" << std::endl;
    }

    ByteArray bytes;
    bytes.reserve(data.length());
    memcpy(bytes.data(), data.data(), data.length());

    int clientId = findClientByConnection(clientSocket);
    if (clientId == 0) {
        LOG_ERR << "Invalid clientId";
        return;
    }

    mCallback(clientId, bytes);
}

ClientId_t QtSocketIpcServer::findClientByConnection(QTcpSocket *socket) const
{
    if (socket) {
        for (const auto &pair : mConnections) {
            if (pair.second == socket) return pair.first;
        }
    }

    return 0;
}

QtSocketIpcClient::QtSocketIpcClient(onDataReceived cb, QObject *parent) :
    QObject(parent), IIPCClient(cb)
{}

QtSocketIpcClient::~QtSocketIpcClient()
{}

bool QtSocketIpcClient::connectToService(ServiceDescriptor service)
{
    mSocket.reset(new QTcpSocket(this));

    if (!mSocket) {
        LOG_ERR << "Unable to create client socket" << std::endl;
        return false;
    }

    mSocket->connectToHost(QHostAddress(QString(service.serviceAddress.c_str())), service.servicePort);

    connect(mSocket.get(), &QTcpSocket::connected, this, [=]()
    {
        LOG_INFO << "Connected to service=" << service.serviceAddress
                 << " port=" << service.servicePort << std::endl;
    } );

    connect(mSocket.get(), &QTcpSocket::readyRead, this, [=]()
    {
        QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
        if (socket) {
            socket->readAll();
        }

        QByteArray data = socket->readAll();

        ByteArray bytes;
        bytes.reserve(data.length());
        memcpy(bytes.data(), data.data(), data.length());

        mCallback(bytes);
    } );

    connect(mSocket.get(), &QTcpSocket::errorOccurred, this, [=](QAbstractSocket::SocketError err)
    {
        LOG_ERR << "Socket error = " << err;
    } );

    return true;
}

ClientId_t QtSocketIpcServer::generateNextClientId()
{
    sNextClientId = sNextClientId + 1 == UINT64_MAX ? 1 : sNextClientId + 1;
    return sNextClientId;
}

bool QtSocketIpcClient::disconnectFromService()
{
    if (mSocket) {
        mSocket->disconnectFromHost();
        mSocket->abort();
        disconnect(mSocket.get(), SIGNAL(connected));
        disconnect(mSocket.get(), SIGNAL(errorOccurred));
        disconnect(mSocket.get(), SIGNAL(readyRead));

        return true;
    }

    return false;
}

int QtSocketIpcClient::writeToServer(const ByteArray buffer)
{
    if (mSocket) {
        int rc = mSocket->write(buffer.data());
        if (rc > 0) {
            LOG_INFO << "Bytes written=" << rc;
        }

        return rc;
    }

    return -1;
}
