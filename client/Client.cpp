#include "Client.h"
#include "IPCInterface.h"
#include "IPCFactory.h"
#include "Serializer.h"

#define LOG_TAG "Client"
#include "Logger.h"

Client::Client(IPCType_t type) :
    mActive(false)
{
    IIPCClient::onDataReceived callback = std::bind
            (&Client::onDataReceived, this, std::placeholders::_1);
    mIpc.reset(createIPCClientFactory(type, callback));
}

Client::~Client()
{
    disconnect();
}

bool Client::connect(const ServiceDescriptor &desc)
{
    if (!mIpc) {
        LOG_ERR << "Invalid IPC object" << std::endl;
        return false;
    }

    mLoopThread = std::thread(&Client::loop, this);

    return mIpc->connectToService(desc);
}

bool Client::disconnect()
{
    mActive = false;
    if (mLoopThread.joinable()) {
        LOG_INFO << "Waiting for loop thread to join" << std::endl;
        mLoopThread.join();
        LOG_INFO << "Thread loop joined" << std::endl;
    }

    return true;
}

void Client::loop()
{
    LOG_INFO << "Loop thread has started" << std::endl;
    mActive = true;
    ByteArray buffer;
    while (mActive) {
        const auto response = getNextItem();
        if (response) {

        }

    }

    LOG_INFO << "Loop thread has finished" << std::endl;
}

void Client::onDataReceived(const ByteArray &bytes)
{
    if (bytes.size() > 0) {
        BaseResponse* response =
                static_cast<BaseResponse*>(Serializer::deserialize(bytes));
        if (response) {
            enqueue(BaseResponsePtr(response));
        }
    }
}

void Client::enqueue(BaseResponsePtr response)
{
    std::lock_guard<std::mutex> autoLock(mQueueLock);

    mQueue.push(response);
}

void Client::handleResponse(BaseRequestPtr response)
{
    if (response) {
        const auto type = response->getMessageType();
        LOG_INFO << "Handle response type=" << type << std::endl;
        switch (type) {
            case HASH: {
                const auto hashResponse = std::dynamic_pointer_cast<HashResponse>(response);
                if (hashResponse) {
                    LOG_INFO << "Hash | Data=" << hashResponse->getHash() << std::endl;
                }
                break;
            }
            case GET_HISTORY: {
                const auto historyResponse = std::dynamic_pointer_cast<GetHistoryResponse>(response);
                if (historyResponse) {
                    LOG_INFO << "History | Start" << std::endl;
                    for (const auto &pair : historyResponse->getHistory()) {
                        LOG_INFO << "String =" << pair.first << " ||| Hash = " << pair.second << std::endl;
                    }
                    LOG_INFO << "History | End" << std::endl;
                }
                break;
            }
            case CLEAR_HISTORY: {
                const auto clearResponse = std::dynamic_pointer_cast<ClearHistoryResponse>(response);
                if (clearResponse) {
                    LOG_INFO << "ClearHistory | isOk=" << clearResponse->isOk() << std::endl;
                }
                break;
                break;
            }
        }
    }
}

BaseResponsePtr Client::getNextItem()
{
    std::lock_guard<std::mutex> autoLock(mQueueLock);

    if (!mQueue.empty()) {
        auto next = mQueue.front();
        mQueue.pop();
        return next;
    }

    return BaseResponsePtr();
}
