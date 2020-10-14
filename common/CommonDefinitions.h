#ifndef COMMONDEFINITIONS_H
#define COMMONDEFINITIONS_H

#include <inttypes.h>
#include <list>
#include <map>
#include <string>
#include <vector>

#define DISABLE_COPY(T) T(const T&) = delete;

typedef enum HashAlgorithms {
    MD5,
    SHA256,
    SHA384,
    SHA512
} HashAlgorithm_t;

typedef enum IPCTypes {
    SOCKET,
    INTERNET_SOCKET,
    QT_SOCKET,
    DBUS,
} IPCType_t;

typedef uint64_t ClientId_t;

typedef std::vector<char> ByteArray;
typedef std::list<std::pair<std::string, ByteArray> > HistoryMap;
typedef std::map<ClientId_t, HistoryMap> ClientsHistoryMap;

#endif // COMMONDEFINITIONS_H
