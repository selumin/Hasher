#include "Hasher.h"

#include <QCryptographicHash>


std::string Hasher::hashMd5(const std::string &text)
{
    return QCryptographicHash::hash(
                QByteArray(text.c_str(), text.length()),
                QCryptographicHash::Md5)
            .toStdString();
}

std::string Hasher::hashSha256(const std::string &text)
{
    return QCryptographicHash::hash(
                QByteArray(text.c_str(), text.length()),
                QCryptographicHash::Sha256)
            .toStdString();
}

std::string Hasher::hashSha384(const std::string &text)
{
    return QCryptographicHash::hash(
                QByteArray(text.c_str(), text.length()),
                QCryptographicHash::Sha384)
            .toStdString();
}

std::string Hasher::hashSha512(const std::string &text)
{
    return QCryptographicHash::hash(
                QByteArray(text.c_str(), text.length()),
                QCryptographicHash::Sha512)
            .toStdString();
}
