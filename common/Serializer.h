#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "CommonDefinitions.h"
#include "Messages.h"

class Serializer
{
    DISABLE_COPY(Serializer);

public:
    Serializer() = delete;
    ~Serializer() = delete;

    static ByteArray serialize(const BaseMessage* request);
    static BaseMessage* deserialize(const ByteArray &bytes);
};

#endif // SERIALIZER_H

