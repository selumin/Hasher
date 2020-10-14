#include "Hasher.h"

#include <QCryptographicHash>

ByteArray Hasher::hashMd5(const std::string &text)
{
    const auto hash = QCryptographicHash::hash(
                QByteArray(text.c_str(), text.length()),
                QCryptographicHash::Md5);
    return ByteArray(hash.begin(), hash.end());
}

ByteArray Hasher::hashSha256(const std::string &text)
{
    const auto hash = QCryptographicHash::hash(
                QByteArray(text.c_str(), text.length()),
                QCryptographicHash::Sha256);
    return ByteArray(hash.begin(), hash.end());
}

ByteArray Hasher::hashSha384(const std::string &text)
{
    const auto hash =  QCryptographicHash::hash(
                QByteArray(text.c_str(), text.length()),
                QCryptographicHash::Sha384);
    return ByteArray(hash.begin(), hash.end());
}

ByteArray Hasher::hashSha512(const std::string &text)
{
    const auto hash =  QCryptographicHash::hash(
                QByteArray(text.c_str(), text.length()),
                QCryptographicHash::Sha512);
    return ByteArray(hash.begin(), hash.end());
}
