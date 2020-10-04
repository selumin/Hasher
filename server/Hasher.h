#ifndef HASHER_H
#define HASHER_H

#include "CommonDefinitions.h"

#include <string>

class Hasher
{
    DISABLE_COPY(Hasher)
    Hasher() = delete;
    ~Hasher() = delete;

public:
    static std::string hashMd5(const std::string &text);
    static std::string hashSha256(const std::string &text);
    static std::string hashSha384(const std::string &text);
    static std::string hashSha512(const std::string &text);
};

#endif // HASHER_H
