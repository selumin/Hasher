#ifndef CLIENT_H
#define CLIENT_H

#include "CommonDefinitions.h"
#include "IPCInterface.h"
#include "Messages.h"

#include <atomic>
#include <mutex>
#include <queue>
#include <thread>

class Client
{
public:
    Client(IPCType_t type);
    ~Client();

    bool connect(const ServiceDescriptor &desc);
    bool disconnect();

private:
    void loop();
    void onDataReceived(const ByteArray &bytes);
    void enqueue(BaseResponsePtr response);
    void handleResponse(BaseRequestPtr response);
    BaseResponsePtr getNextItem();

    std::thread mLoopThread;
    std::atomic_bool mActive;
    std::mutex mQueueLock;
    std::queue<BaseResponsePtr> mQueue;
    std::unique_ptr<IIPCClient> mIpc;
};

#endif // CLIENT_H
