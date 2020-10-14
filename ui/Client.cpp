#include "Client.h"
#include "IPCInterface.h"
#include "IPCFactory.h"
#include "Serializer.h"
#include "Utils.h"


#define LOG_TAG "Client"
#include "Logger.h"

Client::Client(IPCType_t type) :
    mActive(false)
{
    IIPCClient::HandleResponseCallback callback = std::bind
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

bool Client::sendRequest(BaseRequestPtr request)
{
    if (request && mIpc) {
        ByteArray bytes = Serializer::serialize(request.get());
        int rc = mIpc->writeToServer(bytes);
        if (rc > 0) {
            LOG_INFO << "Request send" << std::endl;
            return true;
        }
    }

    return false;
}

void Client::setHashUpdatedCallback(Client::HashUpdatedCallback cb)
{
    mHashCallback = cb;
}

void Client::setHistoryUpdatedCallback(Client::HistoryUpdatedCallback cb)
{
    mHistoryCallback = cb;
}

void Client::loop()
{
    LOG_INFO << "Worker thread has started" << std::endl;
    mActive = true;
    ByteArray buffer;
    while (mActive) {
        auto response = getNextItem();
        if (response) {
            handleResponse(response);
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

void Client::handleResponse(BaseResponsePtr response)
{
    if (response) {
        const auto type = response->getMessageType();
        LOG_INFO << "Handle response type=" << type << std::endl;
        switch (type) {
            case HASH: {
                const auto hashResponse = std::dynamic_pointer_cast<HashResponse>(response);
                if (hashResponse) {
                    if (mHashCallback) mHashCallback( hexArrayToString(hashResponse->getHash()) );
                    LOG_INFO << "Hash | Data=" << hexArrayToString(hashResponse->getHash()) << std::endl;
                }
                break;
            }
            case GET_HISTORY: {
                const auto historyResponse = std::dynamic_pointer_cast<GetHistoryResponse>(response);
                if (historyResponse) {
                    if (mHistoryCallback) mHistoryCallback(historyResponse->getHistory());
                    LOG_INFO << "History | Start" << std::endl;
                    for (const auto &pair : historyResponse->getHistory()) {
                        LOG_INFO << "String =" << pair.first << " ||| Hash = " << hexArrayToString(pair.second) << std::endl;
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
