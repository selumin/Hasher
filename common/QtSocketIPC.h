#ifndef QTSOCKETTRANSPORT_H
#define QTSOCKETTRANSPORT_H

#include "IPCInterface.h"
#include "CommonDefinitions.h"

#include <atomic>
#include <map>
#include <QTcpSocket>
#include <QTcpServer>

class QtSocketIpcServer : public QObject, public IIPCServer {

    Q_OBJECT
    DISABLE_COPY(QtSocketIpcServer);

public:
    QtSocketIpcServer(onDataReceived cb, QObject *parent = nullptr);
    virtual ~QtSocketIpcServer();

    virtual bool registerService(ServiceDescriptor service) override;
    virtual bool unregisterService() override;

    virtual int writeToClient(ClientId_t clientId, const ByteArray buffer) override;

private slots:
    void handleNewClientConnection();
    void handleNewClientRequest();

private:
    ClientId_t findClientByConnection(QTcpSocket * socket) const;
    ClientId_t generateNextClientId();

    std::unique_ptr<QTcpServer> mServer;

    static std::atomic<ClientId_t> sNextClientId;
    std::map<uint64_t, QTcpSocket* > mConnections;
};

class QtSocketIpcClient : public QObject, public IIPCClient {

    Q_OBJECT
    DISABLE_COPY(QtSocketIpcClient);

public:
    QtSocketIpcClient(onDataReceived cb, QObject *parent = nullptr);
    virtual ~QtSocketIpcClient();

    virtual bool connectToService(ServiceDescriptor service) override;
    virtual bool disconnectFromService() override;

    virtual int writeToServer(const ByteArray buffer) override;

private:
    std::unique_ptr<QTcpSocket> mSocket;
};

#endif // QTSOCKETTRANSPORT_H
