#include "Serializer.h"

#define LOG_TAG "Serializer"
#include "Logger.h"

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
                            std::string((bytes.data() + sHeaderSize + 1), totaPayloadSize - 1));
            } else {
                std::ostringstream s;
                for (int i = sHeaderSize; i < static_cast<int>(bytes.size()); ++i) {
                    s << std::hex << static_cast<int>(bytes[i]);
                }
                return new HashResponse(s.str());
            }

            break;
        }

        case GET_HISTORY: {
            if (isRequest) {
                return new GetHistoryRequest();
            } else {
                HistoryMap map;

                const auto payloadStart = sHeaderSize;
                int currPos = payloadStart + 1;

                while (currPos <= totaPayloadSize) {
                    const auto keySize = bytes.at(currPos++);
                    const std::string key(bytes.data() + currPos + keySize);
                    currPos += keySize;
                    const auto valueSize = bytes.at(currPos++);
                    const std::string value(bytes.data() + currPos + valueSize);
                    currPos += valueSize;

                    map[key] = value;
                }

                return new GetHistoryResponse(map);
            }
            break;
        }

        default: {
            LOG_ERR << "Unsupported type: " << type << std::endl;
        }
    }

    return nullptr;
}


