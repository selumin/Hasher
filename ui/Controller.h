#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Client.h"
#include "CommonDefinitions.h"

#include <QObject>

Q_ENUMS(HashAlgorithms)

class Controller : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int algorithmType READ algorithmType WRITE setAlgorithmType NOTIFY algorithmTypeChanged)
    Q_PROPERTY(QString originalText READ originalText WRITE setOriginalText NOTIFY originalTextChanged)
    Q_PROPERTY(QString hashString READ hashString WRITE setHashString NOTIFY hashStringChanged)

public:
    Controller();

    int algorithmType() const;
    QString originalText() const;
    QString hashString() const;

    void setAlgorithmType(int t);
    void setOriginalText(QString text);
    void setHashString(QString hash);

    void hashUpdateCallback(const std::string& hash);
    void historyUpdateCallback(const HistoryMap& history);

    Q_INVOKABLE void doHash(int type, QString text);
    Q_INVOKABLE void getHistory();

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

#endif // CONTROLLER_H
