#ifndef BASEMESSAGE_H
#define BASEMESSAGE_H

#include "CommonDefinitions.h"

#include <map>
#include <memory>
#include <string>

typedef enum MessageTypes {
    REQUEST,
    RESPONSE
} MessageType_t;

typedef enum RequestTypes {
    HASH,
    GET_HISTORY,
    CLEAR_HISTORY
} RequestType_t;

typedef RequestType_t ResponseType_t;

class BaseMessage
{
public:
    BaseMessage(MessageType_t type);
    virtual ~BaseMessage() = default;

    MessageType_t getMessageType() const;

    virtual ByteArray serialize() const = 0;

private:
    MessageType_t mMessageType;
};

class BaseRequest : public BaseMessage
{
public:
    BaseRequest(RequestType_t type);
    virtual ~BaseRequest() = default;

    RequestType_t getMessageType() const;

    virtual ByteArray serialize() const = 0;

private:
    RequestType_t mType;
};

class BaseResponse : public BaseMessage
{
public:
    BaseResponse(ResponseType_t type);
    virtual ~BaseResponse() = default;

    ResponseType_t getMessageType() const;

    virtual ByteArray serialize() const = 0;

private:
    ResponseType_t mType;
};

class HashRequest : public BaseRequest {
public:
    HashRequest(HashAlgorithm_t alg, const std::string& text);
    ~HashRequest() = default;

    virtual ByteArray serialize() const override;

    HashAlgorithm_t getAlgorithm() const;
    const std::string getText() const;

private:
    HashAlgorithm_t mAlgorithm;
    std::string mTextString;

};

class GetHistoryRequest : public BaseRequest {
public:
    GetHistoryRequest();
    ~GetHistoryRequest() = default;

    virtual ByteArray serialize() const override;
};

class ClearHistoryRequest : public BaseRequest {
public:
    ClearHistoryRequest();
    ~ClearHistoryRequest() = default;

    virtual ByteArray serialize() const override;
};

class HashResponse : public BaseResponse {
public:
    HashResponse(const std::string& hash);
    ~HashResponse() = default;

    virtual ByteArray serialize() const override;

    const std::string getHash() const;

private:
    std::string mHashString;
};

class GetHistoryResponse : public BaseResponse {
public:
    GetHistoryResponse(const HistoryMap &map);
    ~GetHistoryResponse() = default;

    virtual ByteArray serialize() const override;

    const HistoryMap getHistory() const;

private:
    HistoryMap mHistoryMap;
};

class ClearHistoryResponse : public BaseResponse {
public:
    ClearHistoryResponse(bool isOk);
    ~ClearHistoryResponse() = default;

    bool isOk() const;

    virtual ByteArray serialize() const override;
private:
    bool mDoneSuccesfully;
};


typedef std::shared_ptr<BaseMessage> BaseMessagePtr;
typedef std::shared_ptr<BaseRequest> BaseRequestPtr;
typedef std::shared_ptr<BaseResponse> BaseResponsePtr;

#endif // BASEMESSAGE_H
