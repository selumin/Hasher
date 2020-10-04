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
    typedef std::function<void(const std::string)> HashUpdatedCallback;
    typedef std::function<void(const HistoryMap)> HistoryUpdatedCallback;

    Client(IPCType_t type);
    ~Client();

    bool connect(const ServiceDescriptor &desc);
    bool disconnect();

    bool sendRequest(BaseRequestPtr request);

    void setHashUpdatedCallback(HashUpdatedCallback cb);
    void setHistoryUpdatedCallback(HistoryUpdatedCallback cb);

private:
    void loop();
    void onDataReceived(const ByteArray &bytes);
    void enqueue(BaseResponsePtr response);
    void handleResponse(BaseResponsePtr response);
    BaseResponsePtr getNextItem();

    std::thread mLoopThread;
    std::atomic_bool mActive;
    std::mutex mQueueLock;
    std::queue<BaseResponsePtr> mQueue;
    std::unique_ptr<IIPCClient> mIpc;

    HashUpdatedCallback mHashCallback;
    HistoryUpdatedCallback mHistoryCallback;
};

#endif // CLIENT_H
