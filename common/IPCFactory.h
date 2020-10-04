#ifndef IPCFACTORY_H
#define IPCFACTORY_H

#include "CommonDefinitions.h"
#include "IPCInterface.h"
#include "QtSocketIPC.h"

IIPCClient* createIPCClientFactory(IPCType_t type, IIPCClient::onDataReceived cb) {
    switch (type) {
        case QT_SOCKET:
            return static_cast<QtSocketIpcClient*>(new QtSocketIpcClient(cb));
        case SOCKET:
        case INTERNET_SOCKET:
        default:
            return nullptr;
    }
}

IIPCServer* createIPCServerFactory(IPCType_t type, IIPCServer::onDataReceived cb) {
    switch (type) {
        case QT_SOCKET:
            return static_cast<QtSocketIpcServer*>(new QtSocketIpcServer(cb));
        case SOCKET:
        case INTERNET_SOCKET:
        default:
            return nullptr;
    }
}

#endif // IPCFACTORY_H
