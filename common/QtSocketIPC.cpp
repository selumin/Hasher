#include "QtSocketIPC.h"

#define LOG_TAG "QtIPC"
#include "Logger.h"

std::atomic<ClientId_t> QtSocketIpcServer::sNextClientId { 0 };

QtSocketIpcServer::QtSocketIpcServer(IIPCServer::HandleRequestCallback cb, QObject *parent) :
    QObject(parent), IIPCServer(cb)
{}

QtSocketIpcServer::~QtSocketIpcServer()
{
    unregisterService();
}

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

void QtSocketIpcServer::unregisterService()
{
    if (mServer) {
        mServer->close();
    }
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

    ByteArray bytes(data.begin(), data.end());

    int clientId = findClientByConnection(clientSocket);
    if (clientId == 0) {
        LOG_ERR << "Invalid clientId";
        return;
    }

    const auto responseBytes = mCallback(clientId, bytes);
    int bytesWritten = clientSocket->write(responseBytes.data(), responseBytes.size());
    if (bytesWritten == -1) {
        LOG_ERR << "Write to socket failed: "
                << clientSocket->errorString().toStdString() << std::endl;
    } else {
        LOG_INFO << "Bytes written to client " << bytesWritten << std::endl;
    }

    return;
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

QtSocketIpcClient::QtSocketIpcClient(HandleResponseCallback cb, QObject *parent) :
    QObject(parent), IIPCClient(cb)
{}

QtSocketIpcClient::~QtSocketIpcClient()
{}

bool QtSocketIpcClient::connectToService(ServiceDescriptor service)
{
    mSocket.reset(new QTcpSocket());

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

    connect(mSocket.get(), &QTcpSocket::disconnected, mSocket.get(), &QObject::deleteLater);
    connect(mSocket.get(), &QTcpSocket::readyRead, this, [=]()
    {
        QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
        if (socket) {
            QByteArray data = socket->readAll();
            ByteArray bytes(data.begin(), data.end());
            mCallback(bytes);
        }
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

int QtSocketIpcClient::writeToServer(const ByteArray& buffer)
{
    if (mSocket) {
        int rc = mSocket->write(buffer.data(), buffer.size());
        if (rc == -1) {
            LOG_ERR << "Write to socket error " << mSocket->errorString().toStdString() << std::endl;
            return -1;
        }

        LOG_INFO << "Bytes written=" << rc << std::endl;
        return rc;
    }

    return -1;
}
