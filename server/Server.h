#ifndef IPCSERVER_H
#define IPCSERVER_H

#include "CommonDefinitions.h"
#include "IPCInterface.h"
#include "Messages.h"

#include <atomic>
#include <mutex>
#include <queue>
#include <thread>

class Server
{
    typedef std::pair<const ClientId_t, BaseRequestPtr> ClientRequest_t;

public:
    Server(IPCType_t type);
    ~Server();

    bool start(const ServiceDescriptor &desc);
    bool stop();

private:
    void loop();
    void onNewRequest(ClientId_t clientId, const ByteArray &bytes);
    void enqueueRequest(ClientRequest_t clientRequest);
    ClientRequest_t getNextRequest();
    BaseResponse* process(ClientId_t id, BaseRequestPtr req);
    std::string generateHash(HashAlgorithm_t type, const std::string &text);

    std::thread mLoopThread;
    std::atomic_bool mActive;
    std::mutex mQueueLock;
    std::queue<ClientRequest_t> mQueue;
    std::unique_ptr<IIPCServer> mIpcServer;
    ClientsHistoryMap mHistory;
};

#endif // IPCSERVER_H
