#pragma once

#include <QString>

class DnsParser
{
public:
    DnsParser() = delete;

    static QString parseDnsResponsePacket(const QByteArray &data);
    static QByteArray createDnsQueryPacket(const QString &domain);

private:
    static quint16 readUInt16(const QByteArray &data, int offset);
    static bool writeQName(QDataStream &stream, const QString &domain);
};
