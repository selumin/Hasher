#include "Contoller.h"

Contoller::Contoller() :
    mClientWrapper(QT_SOCKET)
{
    Client::HashUpdatedCallback hashCb = std::bind
            (&Contoller::hashUpdateCallback, this, std::placeholders::_1);
    Client::HistoryUpdatedCallback historyCb = std::bind
            (&Contoller::historyUpdateCallback, this, std::placeholders::_1);

    mClientWrapper.setHashUpdatedCallback(hashCb);
    mClientWrapper.setHistoryUpdatedCallback(historyCb);
}

int Contoller::algorithmType() const
{
    return mAlgType;
}

QString Contoller::originalText() const
{
    return mOriginalText;
}

QString Contoller::hashString() const
{
    return mHash;
}

void Contoller::setAlgorithmType(int t)
{
    if (mAlgType != t) {
        mAlgType = t;
        emit algorithmTypeChanged();
    }
}

void Contoller::setOriginalText(QString text)
{
    if (mOriginalText != text) {
        mOriginalText = text;
        emit originalTextChanged();
    }
}

void Contoller::setHashString(QString hash)
{
    if (mHash != hash) {
        mHash = hash;
        emit hashStringChanged();
    }
}

void Contoller::hashUpdateCallback(const std::string& hash)
{
    setHashString(hash.c_str());
}

void Contoller::historyUpdateCallback(const HistoryMap& history)
{}

void Contoller::doHash(HashAlgorithm_t type, QString text)
{
    BaseRequestPtr req(new HashRequest(type, text.toStdString()));
    mClientWrapper.sendRequest(req);
}
