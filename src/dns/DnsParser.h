#pragma once

#include "3rd/dns.hpp"

#include <QByteArray>
#include <QHostAddress>
#include <QString>
#include <optional>
#include <type_traits>

class DnsParser
{
public:
    DnsParser() = delete;

    static std::optional<dns::DnsMessage> parseDnsResponsePacket(QByteArrayView packet);
    static QString dnsMessageToString(const dns::DnsMessage &message);
    static QByteArray buildDnsQueryPacket(const dns::DnsMessage &message);
    static QByteArray buildDnsQueryPacket(QStringView domain, uint16_t recordType, uint16_t recordClass);

private:
    template <typename T,
              typename = std::enable_if_t<std::is_same_v<typename T::value_type, std::byte>>>
    static QByteArray vectorToQByteArray(const T &data);
    static std::vector<std::byte> QByteArrayToVector(QByteArrayView packet);
    static QHostAddress vectorToQHostAddress(const std::array<std::byte, 16> &data);
};
