#include "PosixLocalSocketIPC.h"

#define LOG_TAG "PosixSocket"
#include "Logger.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

std::atomic<ClientId_t> PosixLocalSocketServer::sNextClientId { 0 };

PosixLocalSocketServer::PosixLocalSocketServer(IIPCServer::HandleRequestCallback cb) : IIPCServer(cb)
{}

PosixLocalSocketServer::~PosixLocalSocketServer()
{
    unregisterService();
}

bool PosixLocalSocketServer::registerService(ServiceDescriptor service)
{
    struct sockaddr_in addr = { 0 };
    addr.sin_family = AF_INET;
    addr.sin_port = htons(service.servicePort);
    addr.sin_addr.s_addr = inet_addr(service.serviceAddress.c_str());

    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == -1) {
        LOG_ERR << "Unable to create socket" << std::endl;
        return false;
    }

    return true;
}

void PosixLocalSocketServer::unregisterService()
{

}

int PosixLocalSocketServer::writeToClient(ClientId_t clientId, const ByteArray &buffer)
{

}

ClientId_t PosixLocalSocketServer::generateNextClientId()
{
    sNextClientId = sNextClientId + 1 == UINT64_MAX ? 1 : sNextClientId + 1;
    return sNextClientId;
}

PosixLocalSocketClient::PosixLocalSocketClient(IIPCClient::HandleResponseCallback cb) : IIPCClient(cb)
{

}

PosixLocalSocketClient::~PosixLocalSocketClient()
{

}

bool PosixLocalSocketClient::connectToService(ServiceDescriptor service)
{

}

bool PosixLocalSocketClient::disconnectFromService()
{

}

int PosixLocalSocketClient::writeToServer(const ByteArray& buffer)
{

}
