#include "Server.h"
#include "IPCInterface.h"
#include "IPCFactory.h"
#include "Serializer.h"
#include "Hasher.h"

#define LOG_TAG "SERVER"
#include "Logger.h"

#include <memory>

Server::Server(IPCType_t type) :
    mActive(false)
{
    IIPCServer::onDataReceived callback = std::bind
            (&Server::onNewRequest, this, std::placeholders::_1, std::placeholders::_2);
    mIpcServer.reset(createIPCServerFactory(type, callback));
}

Server::~Server()
{
    if (!stop()) {
        LOG_ERR << "Unable to stop IPC Server properly" << std::endl;
    }
}

bool Server::start(const ServiceDescriptor& desc)
{
    if (!mIpcServer) {
        LOG_ERR << "Invalid pointer to transfer implementation" << std::endl;
        return false;
    }

    mLoopThread = std::thread(&Server::loop, this);
    return mIpcServer->registerService(desc);
}

bool Server::stop()
{
    if (!mIpcServer) {
        LOG_ERR << "Invalid pointer to transfer implementation" << std::endl;
        return false;
    }

    return mIpcServer->unregisterService();

    mActive = false;
    if (mLoopThread.joinable()) {
        LOG_INFO << "Waiting for thread to join" << std::endl;
        mLoopThread.join();
        LOG_INFO << "Thread has joined" << std::endl;
    }
}

void Server::loop()
{
    LOG_INFO << "Loop thread has started" << std::endl;

    mActive = true;
    ByteArray buffer;
    while (mActive) {
        const auto request = getNextRequest();
        const auto clientId = request.first;
        if (clientId != 0 && request.second != nullptr) {
            BaseResponsePtr response;
            response.reset( process(clientId, request.second) );
            if (response != nullptr) {
                const auto bytes = Serializer::serialize(response.get());
                const auto c = mIpcServer->writeToClient(clientId, bytes);
                if (c <= 0) {
                    LOG_ERR << "Error sending bytes to client" << std::endl;
                } else {
                    LOG_INFO << c << " bytes send to client " << clientId << std::endl;
                }
            }
        }

    }

    LOG_INFO << "Loop thread has finished" << std::endl;
}

void Server::onNewRequest(ClientId_t clientId, const ByteArray &bytes)
{
    BaseMessagePtr msg(Serializer::deserialize(bytes));
    if (msg) {
        BaseRequestPtr response = std::dynamic_pointer_cast<BaseRequest>(msg);
        if (response) {
            enqueueRequest(ClientRequest_t(clientId, response));
        }
    }
}

void Server::enqueueRequest(Server::ClientRequest_t clientRequest)
{
    std::lock_guard<std::mutex> autoLock(mQueueLock);

    mQueue.push(clientRequest);
}

Server::ClientRequest_t Server::getNextRequest()
{
    std::lock_guard<std::mutex> autoLock(mQueueLock);

    if (!mQueue.empty()) {
        auto next = mQueue.front();
        mQueue.pop();
        return next;
    }

    return ClientRequest_t(0, nullptr);
}

BaseResponse* Server::process(ClientId_t id, BaseRequestPtr req)
{
    switch (req->getMessageType()) {
        case HASH: {
            HashRequest *r = static_cast<HashRequest*>(req.get());
            if (r) {
                const auto hash = generateHash(r->getAlgorithm(), r->getText());
                mHistory[id].insert(std::make_pair(r->getText(), hash));

                return new HashResponse(hash);;
            }
;
            return nullptr;
        }

        case GET_HISTORY: {
            const auto currentClientHistory = mHistory[id];
            return new GetHistoryResponse(currentClientHistory);
        }
        case CLEAR_HISTORY: {
            mHistory[id].clear();
            return new ClearHistoryResponse(true);
        }
    }

    return nullptr;
}

std::string Server::generateHash(HashAlgorithm_t type, const std::string &text)
{
    LOG_INFO << "Hasing string=" << text << " | with alg=" << type << std::endl;

    switch (type) {
        case MD5:       return Hasher::hashMd5(text);
        case SHA256:    return Hasher::hashSha256(text);
        case SHA384:    return Hasher::hashSha384(text);
        case SHA512:    return Hasher::hashSha512(text);
    }

    return std::string();
}
