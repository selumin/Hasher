#include "Serializer.h"

#define LOG_TAG "Serializer"
#include "Logger.h"

#include "Utils.h"

#include <sstream>

/*  Header - total 3 bytes:
 *      1 byte - Request / Response
 *      2 byte - Type
 *      3 byte - Packet size LSB in bytes
 *      4 byte - Packet size MSB in bytes
 *  --------
 *  Payload
 *      1 byte - Next item size in bytes
 *      N Bytes - Item payload
 */

static constexpr uint8_t sHeaderSize    = 4;
static constexpr uint8_t sDataTypePos   = 0;
static constexpr uint8_t sTypePos       = 1;
static constexpr uint8_t sSizeLSBPos    = 2;
static constexpr uint8_t sSizeMSBPos    = 3;

ByteArray Serializer::serialize(const BaseMessage* request)
{
    if (!request) {
        LOG_ERR << "invalid pointer to request" << std::endl;
        return ByteArray();
    }

    return request->serialize();
}

BaseMessage* Serializer::deserialize(const ByteArray &bytes)
{
    if (bytes.size() < 4) {
        LOG_ERR << "There is no data to deserialize" << std::endl;
        return nullptr;
    }

    const bool isRequest = bytes.at(sDataTypePos) == REQUEST;
    const RequestType_t type = static_cast<RequestType_t>(bytes.at(sTypePos));
    const uint16_t totaPayloadSize = (bytes.at(sSizeMSBPos) << 8) | bytes.at(sSizeLSBPos);

    switch (type) {
        case HASH: {
            if (isRequest) {
                HashAlgorithm_t alg = static_cast<HashAlgorithm_t>( bytes.at(sHeaderSize) );
                return new HashRequest(
                            alg,
                            std::string((bytes.data() + sHeaderSize + 1), totaPayloadSize));
            } else {
                const ByteArray payload(bytes.begin() + sHeaderSize, bytes.end());
                return new HashResponse(ByteArray(bytes.begin() + sHeaderSize, bytes.end()));
            }

            break;
        }

        case GET_HISTORY: {
            if (isRequest) {
                return new GetHistoryRequest();
            } else {
                HistoryMap history;

                const auto payloadStart = sHeaderSize;
                int currPos = payloadStart;

                const auto pairsCount = bytes[currPos++];
                int currentPairNumber = 0;

                while (currentPairNumber < pairsCount) {
                    const uint16_t keySize = bytes.at(currPos++);
                    const std::string key(bytes.data() + currPos, keySize);
                    currPos += keySize;
                    const uint16_t valueSize = bytes.at(currPos++);
                    const ByteArray value(bytes.data() + currPos + keySize, bytes.data() + currPos + keySize + valueSize);
                    currPos += valueSize;

                    ++currentPairNumber;

                    history.push_back(std::make_pair(key, value));
                }

                return new GetHistoryResponse(history);
            }
            break;
        }

        default: {
            LOG_ERR << "Unsupported type: " << type << std::endl;
        }
    }

    return nullptr;
}


