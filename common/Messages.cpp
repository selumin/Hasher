#include "Messages.h"

#include <cstring>

BaseMessage::BaseMessage(MessageType_t type) : mMessageType(type) {}

MessageType_t BaseMessage::getMessageType() const
{
    return mMessageType;
}

BaseRequest::BaseRequest(RequestType_t type) : BaseMessage(REQUEST)
{
    mType = type;
}

RequestType_t BaseRequest::getMessageType() const
{
    return mType;
}

BaseResponse::BaseResponse(ResponseType_t type) : BaseMessage(RESPONSE)
{
    mType = type;
}

ResponseType_t BaseResponse::getMessageType() const
{
    return mType;
}

HashRequest::HashRequest(HashAlgorithm_t alg, const std::string &text) : BaseRequest(HASH),
    mAlgorithm(alg),
    mTextString(text) {}

ByteArray HashRequest::serialize() const
{
    ByteArray bytes;
    bytes.push_back(REQUEST);
    bytes.push_back(HASH);

    const uint16_t size = mTextString.length();
    bytes.push_back(static_cast<uint8_t>(size));
    bytes.push_back(static_cast<uint8_t>(size >> 8));
    bytes.push_back(mAlgorithm);
    bytes.insert(bytes.end(), mTextString.begin(), mTextString.end());

    return bytes;
}

HashAlgorithm_t HashRequest::getAlgorithm() const
{
    return mAlgorithm;
}

const std::string HashRequest::getText() const
{
    return mTextString;
}

HashResponse::HashResponse(const std::string &hash) : BaseResponse(HASH),
    mHashString(hash) {}

ByteArray HashResponse::serialize() const
{
    ByteArray bytes;
    bytes.push_back(RESPONSE);
    bytes.push_back(HASH);

    const uint16_t size = mHashString.length();
    bytes.push_back(static_cast<uint8_t>(size));
    bytes.push_back(static_cast<uint8_t>(size >> 8));

    bytes.insert(bytes.end(), mHashString.begin(), mHashString.end());

    return bytes;
}

const std::string HashResponse::getHash() const
{
    return mHashString;
}

ClearHistoryResponse::ClearHistoryResponse(bool isOk) : BaseResponse(CLEAR_HISTORY),
    mDoneSuccesfully(isOk) {}

bool ClearHistoryResponse::isOk() const
{
    return mDoneSuccesfully;
}

ByteArray ClearHistoryResponse::serialize() const
{
    ByteArray bytes;
    bytes.push_back(RESPONSE);
    bytes.push_back(CLEAR_HISTORY);
    bytes.push_back(static_cast<uint8_t>(mDoneSuccesfully));

    return bytes;
}

GetHistoryResponse::GetHistoryResponse(const HistoryMap &map) : BaseResponse(GET_HISTORY),
    mHistoryMap(map) {}

ByteArray GetHistoryResponse::serialize() const
{
    ByteArray bytes;

    bytes.push_back(RESPONSE);
    bytes.push_back(GET_HISTORY);

    bytes.push_back(0);
    bytes.push_back(0);

    const auto pairsCount = mHistoryMap.size();
    bytes.push_back(pairsCount);

    for (const auto &pair : mHistoryMap) {
        const auto keySize = pair.first.size();
        bytes.push_back(keySize);
        bytes.insert(bytes.end(), pair.first.begin(), pair.second.end());

        const auto valueSize = pair.second.size();
        bytes.push_back(valueSize);
        bytes.insert(bytes.end(), pair.second.begin(), pair.second.end());
    }

    return bytes;
}

const HistoryMap GetHistoryResponse::getHistory() const
{
    return mHistoryMap;
}

ClearHistoryRequest::ClearHistoryRequest() : BaseRequest(CLEAR_HISTORY) {}

ByteArray ClearHistoryRequest::serialize() const
{
    ByteArray bytes;
    bytes.push_back(REQUEST);
    bytes.push_back(CLEAR_HISTORY);

    return bytes;
}

GetHistoryRequest::GetHistoryRequest() : BaseRequest(GET_HISTORY) {}

ByteArray GetHistoryRequest::serialize() const
{
    ByteArray bytes;
    bytes.push_back(REQUEST);
    bytes.push_back(GET_HISTORY);

    return bytes;
}




