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

class IIPCClient {

    DISABLE_COPY(IIPCClient);

public:
    typedef std::function<void(const ByteArray&)> onDataReceived;

    IIPCClient(onDataReceived callback) : mCallback(callback) {}
    virtual ~IIPCClient() = default;

    virtual int writeToServer(const ByteArray buffer) = 0;

    virtual bool connectToService(ServiceDescriptor service) = 0;
    virtual bool disconnectFromService() = 0;

protected:
    onDataReceived mCallback;
};

class IIPCServer {

    DISABLE_COPY(IIPCServer);

public:
    typedef std::function<void(ClientId_t clientId, const ByteArray&)> onDataReceived;

    IIPCServer(onDataReceived callback) : mCallback(callback) {}
    virtual ~IIPCServer() = default;

    virtual bool registerService(ServiceDescriptor service) = 0;
    virtual bool unregisterService() = 0;

    virtual int writeToClient(uint64_t clientId, const ByteArray buffer) = 0;

protected:
    onDataReceived mCallback;

};

#endif // IPCINTERFACE_H
