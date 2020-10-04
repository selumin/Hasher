#ifndef IPCINTERFACE_H
#define IPCINTERFACE_H

#include "CommonDefinitions.h"
#include <string>
#include <functional>

struct ServiceDescriptor {
    std::string serviceAddress;
    std::string serviceName;
    uint32_t servicePort;

};

class IIPCServer {

    DISABLE_COPY(IIPCServer);

public:
    typedef std::function<const ByteArray(ClientId_t clientId, const ByteArray&)> HandleRequestCallback;

    IIPCServer(HandleRequestCallback callback) : mCallback(callback) {}
    virtual ~IIPCServer() = default;

    virtual bool registerService(ServiceDescriptor service) = 0;
    virtual void unregisterService() = 0;

protected:
    HandleRequestCallback mCallback;

};

class IIPCClient {

    DISABLE_COPY(IIPCClient);

public:
    typedef std::function<void(const ByteArray&)> HandleResponseCallback;

    IIPCClient(HandleResponseCallback callback) : mCallback(callback) {}
    virtual ~IIPCClient() = default;

    virtual int writeToServer(const ByteArray& buffer) = 0;

    virtual bool connectToService(ServiceDescriptor service) = 0;
    virtual bool disconnectFromService() = 0;

protected:
    HandleResponseCallback mCallback;
};

#endif // IPCINTERFACE_H
