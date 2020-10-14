#ifndef IPCSERVER_H
#define IPCSERVER_H

#include "CommonDefinitions.h"
#include "IPCInterface.h"
#include "Messages.h"

class Server
{
    typedef std::pair<const ClientId_t, BaseRequestPtr> ClientRequest_t;

public:
    Server(IPCType_t type);
    ~Server();

    bool start(const ServiceDescriptor &desc);
    bool stop();

private:
    const ByteArray handleNewRequest(ClientId_t clientId, const ByteArray &bytes);
    BaseResponse* process(ClientId_t id, BaseRequestPtr req);
    ByteArray generateHash(HashAlgorithm_t type, const std::string &text);

    std::unique_ptr<IIPCServer> mIpcServer;
    ClientsHistoryMap mHistory;
};

#endif // IPCSERVER_H
