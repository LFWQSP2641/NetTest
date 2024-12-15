#pragma once

#include "DnsResponse.h"

#include <QString>

class DnsParser
{
public:
    DnsParser() = delete;

    static DnsResponse parseDnsResponsePacket(const QByteArray &data);
    static QByteArray createDnsQueryPacket(const QString &domain);

private:
    static quint16 readUInt16(const QByteArray &data, int offset);
    static quint16 readUInt32(const QByteArray &data, int offset);
    static bool writeQName(QDataStream &stream, const QString &domain);
};
