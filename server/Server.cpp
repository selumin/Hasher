#include "Server.h"
#include "IPCInterface.h"
#include "IPCFactory.h"
#include "Serializer.h"
#include "Hasher.h"

#define LOG_TAG "SERVER"
#include "Logger.h"

#include <memory>

Server::Server(IPCType_t type)
{
    IIPCServer::HandleRequestCallback callback = std::bind
            (&Server::handleNewRequest, this, std::placeholders::_1, std::placeholders::_2);
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

    if (!mIpcServer->registerService(desc)) {
        LOG_ERR << "IPC Server registration failed" << std::endl;
        return false;
    }

    return true;
}

bool Server::stop()
{
    if (!mIpcServer) {
        LOG_ERR << "Invalid pointer to transfer implementation" << std::endl;
        return false;
    }

    mIpcServer->unregisterService();

    return true;
}

const ByteArray Server::handleNewRequest(ClientId_t clientId, const ByteArray &bytes)
{
    BaseMessagePtr msg(Serializer::deserialize(bytes));
    if (msg) {
        BaseRequestPtr request = std::dynamic_pointer_cast<BaseRequest>(msg);
        if (request) {
            BaseResponsePtr response;
            response.reset(process(clientId, request));
            if (response != nullptr) {
                return Serializer::serialize(response.get());
            }
        }
    }

    return ByteArray();
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
    LOG_INFO << "Hashing string=" << text << " | with alg=" << type << std::endl;

    switch (type) {
        case MD5:       return Hasher::hashMd5(text);
        case SHA256:    return Hasher::hashSha256(text);
        case SHA384:    return Hasher::hashSha384(text);
        case SHA512:    return Hasher::hashSha512(text);
    }

    return std::string();
}
