#pragma once
#include <QList>
#include <QPair>
#include <QString>

class DnsResponse
{
public:
    struct Answer
    {
        QString domainName;
        QString type;  // Answer type, e.g., "A", "AAAA"
        QString value; // The resolved value, e.g., "192.168.1.1"
    };

    QString transactionId;
    QString flags;
    QList<QString> questions; // List of question domain names
    QList<Answer> answers;    // List of resolved answers

    // 将类转换为 QString
    QString toString() const;
};
