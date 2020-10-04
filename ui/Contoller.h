#ifndef CONTOLLER_H
#define CONTOLLER_H

#include "Client.h"

#include <QObject>

class Contoller : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int algorithmType READ algorithmType WRITE setAlgorithmType NOTIFY algorithmTypeChanged)
    Q_PROPERTY(QString originalText READ originalText WRITE setOriginalText NOTIFY originalTextChanged)
    Q_PROPERTY(QString hashString READ hashString WRITE setHashString NOTIFY hashStringChanged)

public:
    Contoller();

    int algorithmType() const;
    QString originalText() const;
    QString hashString() const;

    void setAlgorithmType(int t);
    void setOriginalText(QString text);
    void setHashString(QString hash);

    void hashUpdateCallback(const std::string& hash);
    void historyUpdateCallback(const HistoryMap& history);

    Q_INVOKABLE void doHash(HashAlgorithm_t type, QString text);

signals:
    void algorithmTypeChanged();
    void originalTextChanged();
    void hashStringChanged();

private:
    int mAlgType;
    QString mOriginalText;
    QString mHash;

    Client mClientWrapper;
};

#endif // CONTOLLER_H
