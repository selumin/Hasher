#include "Utils.h"

std::string hexArrayToString(const ByteArray &bytes) {
    char buffer[1024] = { 0 };
    int i = 0;
    while (i < static_cast<int>(bytes.size())) {
        sprintf(buffer + i, "%02X", bytes[i]);
        ++i;
    }
    buffer[i] = '\0';
    return std::string(buffer);
}
