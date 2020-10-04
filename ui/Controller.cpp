#include "Controller.h"

Controller::Controller() :
    mClientWrapper(QT_SOCKET)
{
    Client::HashUpdatedCallback hashCb = std::bind
            (&Controller::hashUpdateCallback, this, std::placeholders::_1);
    Client::HistoryUpdatedCallback historyCb = std::bind
            (&Controller::historyUpdateCallback, this, std::placeholders::_1);

    mClientWrapper.setHashUpdatedCallback(hashCb);
    mClientWrapper.setHistoryUpdatedCallback(historyCb);

    ServiceDescriptor desc;
    desc.serviceAddress = "127.0.0.1";
    desc.servicePort = 44332;

    mClientWrapper.connect(desc);
}

int Controller::algorithmType() const
{
    return mAlgType;
}

QString Controller::originalText() const
{
    return mOriginalText;
}

QString Controller::hashString() const
{
    return mHash;
}

void Controller::setAlgorithmType(int t)
{
    if (mAlgType != t) {
        mAlgType = t;
        emit algorithmTypeChanged();
    }
}

void Controller::setOriginalText(QString text)
{
    if (mOriginalText != text) {
        mOriginalText = text;
        emit originalTextChanged();
    }
}

void Controller::setHashString(QString hash)
{
    if (mHash != hash) {
        mHash = hash;
        emit hashStringChanged();
    }
}

void Controller::hashUpdateCallback(const std::string& hash)
{
    setHashString(hash.c_str());
}

void Controller::historyUpdateCallback(const HistoryMap& history)
{
    (void)(history);
}

void Controller::doHash(int type, QString text)
{
    BaseRequestPtr req(new HashRequest(
                           static_cast<HashAlgorithm_t>(type), text.toStdString()));
    mClientWrapper.sendRequest(req);
}

void Controller::getHistory()
{
    BaseRequestPtr req(new GetHistoryRequest());
    mClientWrapper.sendRequest(req);
}
