#ifndef POSIXLOCALSOCKETIPC_H
#define POSIXLOCALSOCKETIPC_H

#include "CommonDefinitions.h"
#include "IPCInterface.h"

#include <atomic>
#include <memory>

class PosixLocalSocketServer : public IIPCServer {

    DISABLE_COPY(PosixLocalSocketServer);

public:
    PosixLocalSocketServer(HandleRequestCallback cb);
    virtual ~PosixLocalSocketServer();

    virtual bool registerService(ServiceDescriptor service) override;
    virtual void unregisterService() override;

    virtual int writeToClient(ClientId_t clientId, const ByteArray& buffer) override;

private:
    ClientId_t generateNextClientId();

    static std::atomic<ClientId_t> sNextClientId;
};

class PosixLocalSocketClient : public IIPCClient {

    DISABLE_COPY(PosixLocalSocketClient);

public:
    PosixLocalSocketClient(HandleResponseCallback cb);
    virtual ~PosixLocalSocketClient();

    virtual bool connectToService(ServiceDescriptor service) override;
    virtual bool disconnectFromService() override;

    virtual int writeToServer(const ByteArray& buffer) override;

private:
};

#endif // POSIXLOCALSOCKETIPC_H
