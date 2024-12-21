#include "DnsParser.h"

#include <QRandomGenerator>

std::optional<dns::DnsMessage> DnsParser::parseDnsResponsePacket(QByteArrayView packet)
{
    std::vector<std::byte> responseData(QByteArrayToVector(packet));

    const auto *responsePtr = reinterpret_cast<const uint8_t *>(responseData.data());
    const auto responseSize = responseData.size();

    auto response = dns::Parse(responsePtr, responseSize);
    return response;
}

QString DnsParser::dnsMessageToString(const dns::DnsMessage &message)
{
    QString result;

    std::vector<dns::DnsAnswer> answers;

    answers = message.answers;
    answers.insert(answers.end(), message.authorityAnswers.begin(), message.authorityAnswers.end());
    answers.insert(answers.end(), message.additionalAnswers.begin(), message.additionalAnswers.end());

    // print answers
    for (const auto &answer : answers)
    {
        result.append(QObject::tr("Answer:")).append(QString::fromStdString(answer.name)).append(QStringLiteral("ttl:")).append(QString::number(answer.ttl));
        result.append(QStringLiteral("\n"));
        if (auto a = std::get_if<dns::AData>(&answer.value))
        {
            result.append(QStringLiteral("A:")).append(QHostAddress{ *a }.toString());
        }
        else if (auto aaaa = std::get_if<dns::AAAAData>(&answer.value))
        {
            result.append(QStringLiteral("AAAA:")).append(vectorToQHostAddress(*aaaa).toString());
        }
        else if (auto mx = std::get_if<dns::MXData>(&answer.value))
        {
            result.append(QStringLiteral("MX:")).append(QString::fromStdString(mx->exchange));
        }
        else if (auto ptr = std::get_if<dns::PTRData>(&answer.value))
        {
            result.append(QStringLiteral("PTR:")).append(QString::fromStdString(*ptr));
        }
        else if (auto txt = std::get_if<dns::TXTData>(&answer.value))
        {
            result.append(QStringLiteral("TXT:")).append(QString::fromStdString(txt->txt));
        }
        else if (auto soa = std::get_if<dns::SOAData>(&answer.value))
        {
            result.append(QStringLiteral("SOA:"))
                .append(QObject::tr("PrimaryServer:"))
                .append(soa->primaryServer)
                .append(QObject::tr("Administrator:"))
                .append(soa->administrator)
                .append(QObject::tr("SerialNo:"))
                .append(QString::number(soa->serialNo))
                .append(QObject::tr("Refresh:"))
                .append(QString::number(soa->refresh))
                .append(QObject::tr("Retry:"))
                .append(QString::number(soa->retry))
                .append(QObject::tr("Expire:"))
                .append(QString::number(soa->expire))
                .append(QObject::tr("DefaultTtl:"))
                .append(QString::number(soa->defaultTtl));
        }
        else
        {
            result.append(QObject::tr("Unknown record type"));
        }
        result.append(QStringLiteral("\n"));
    }

    if (answers.empty())
    {
        result.append("No answers found");
        result.append(QStringLiteral("\n"));
    }

    return result;
}

QByteArray DnsParser::buildDnsQueryPacket(const dns::DnsMessage &message)
{
    std::vector<std::byte> data = dns::Build(message);
    return vectorToQByteArray(data);
}

QByteArray DnsParser::buildDnsQueryPacket(QStringView domain, uint16_t recordType, uint16_t recordClass)
{
    dns::DnsHeaderVars headerVars;
    headerVars.xid = QRandomGenerator::global()->generate() & 0xFFFF;
    headerVars.recursionDesired = 1;
    headerVars.opcode = 0;
    headerVars.isResponse = 0;
    headerVars.responseCode = 0;
    headerVars.checkingDisabled = 0;
    headerVars.authenticatedData = 0;
    headerVars.reserved = 0;
    headerVars.recursionAvailable = 0;
    headerVars.truncation = 0;
    headerVars.authoritative = 0;

    dns::DnsQuestion question;
    question.name = domain.toString().toStdString();
    question.type = recordType;
    question.cls = recordClass;

    dns::DnsMessage message;
    message.dnsHead = headerVars;
    message.questions.push_back(question);

    return buildDnsQueryPacket(message);
}

template <typename T, typename T2>
QByteArray DnsParser::vectorToQByteArray(const T &data)
{
    return QByteArray(
        reinterpret_cast<const char *>(data.data()),
        static_cast<qsizetype>(data.size() * sizeof(std::byte)));
}

std::vector<std::byte> DnsParser::QByteArrayToVector(QByteArrayView packet)
{
    return std::vector<std::byte>(
        reinterpret_cast<const std::byte *>(packet.data()),
        reinterpret_cast<const std::byte *>(packet.data() + packet.size()));
}

QHostAddress DnsParser::vectorToQHostAddress(const std::array<std::byte, 16> &data)
{
    // to Q_IPV6ADDR
    Q_IPV6ADDR ipv6;
    for (int i = 0; i < 16; ++i)
    {
        ipv6[i] = static_cast<quint8>(data[i]);
    }
    return QHostAddress(ipv6);
}
