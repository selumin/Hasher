#ifndef IPCFACTORY_H
#define IPCFACTORY_H

#include "CommonDefinitions.h"
#include "IPCInterface.h"
#include "QtSocketIPC.h"
//#include "PosixLocalSocketIPC.h"

IIPCClient* createIPCClientFactory(IPCType_t type, IIPCClient::HandleResponseCallback cb) {
    switch (type) {
        case QT_SOCKET:
            return static_cast<QtSocketIpcClient*>(new QtSocketIpcClient(cb));
        case SOCKET:
            //return static_cast<PosixLocalSocketClient*>(new PosixLocalSocketClient(cb));
        case INTERNET_SOCKET:
        default:
            return nullptr;
    }
}

IIPCServer* createIPCServerFactory(IPCType_t type, IIPCServer::HandleRequestCallback cb) {
    switch (type) {
        case QT_SOCKET:
            return static_cast<QtSocketIpcServer*>(new QtSocketIpcServer(cb));
        case SOCKET:
            //return static_cast<PosixLocalSocketServer*>(new PosixLocalSocketServer(cb));
        case INTERNET_SOCKET:
        default:
            return nullptr;
    }
}

#endif // IPCFACTORY_H
