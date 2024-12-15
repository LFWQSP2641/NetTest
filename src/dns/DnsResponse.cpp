#include "DnsResponse.h"

QString DnsResponse::toString() const
{
    QString result;
    result += QString("Transaction ID: %1\n").arg(transactionId);
    result += QString("Flags: %1\n").arg(flags);
    result += QString("Questions:\n");
    for (const auto &question : questions)
    {
        result += QString("  - %1\n").arg(question);
    }
    result += QString("Answers:\n");
    for (const auto &answer : answers)
    {
        result += QString("  - Domain: %1, Type: %2, Value: %3\n")
                      .arg(answer.domainName)
                      .arg(answer.type)
                      .arg(answer.value);
    }
    return result;
}
