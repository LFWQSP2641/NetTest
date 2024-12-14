#include "DnsParser.h"

#include <QDebug>
#include <QObject>
#include <QRandomGenerator>
#include <QUrl>

QString DnsParser::parseDnsResponsePacket(const QByteArray &data)
{
    if (data.size() < 12)
    {
        return QObject::tr("Invalid DNS response packet");
    }

    // Read header fields
    quint16 transactionId = readUInt16(data, 0);
    quint16 flags = readUInt16(data, 2);
    quint16 questions = readUInt16(data, 4);
    quint16 answerRRs = readUInt16(data, 6);
    quint16 authorityRRs = readUInt16(data, 8);
    quint16 additionalRRs = readUInt16(data, 10);

    QString result = QObject::tr("Transaction ID: %1\n"
                                 "Flags: %2\n"
                                 "Questions: %3\n"
                                 "Answer RRs: %4\n"
                                 "Authority RRs: %5\n"
                                 "Additional RRs: %6\n")
                         .arg(transactionId)
                         .arg(flags, 4, 16, QLatin1Char('0'))
                         .arg(questions)
                         .arg(answerRRs)
                         .arg(authorityRRs)
                         .arg(additionalRRs);

    // Simple parsing of question section
    int pos = 12;
    for (int i = 0; i < questions; ++i)
    {
        if (pos >= data.size())
        {
            return QObject::tr("Malformed DNS response packet: Question section truncated"); // 更具体的错误信息
        }

        QString domainName;
        while (true)
        {
            if (pos >= data.size())
            {
                return QObject::tr("Malformed DNS response packet: Domain name truncated");
            }
            quint8 length = static_cast<quint8>(data[pos]);
            pos++;

            if (length == 0)
            {
                break;
            }
            else if ((length & 0xC0) == 0xC0)
            { // 检查是否为压缩指针
                // TODO: 实现压缩指针的处理
                return QObject::tr("DNS message compression is not supported yet.");
            }
            else
            {
                if (pos + length > data.size())
                {
                    return QObject::tr("Malformed DNS response packet: Domain label truncated");
                }
                domainName.append(QString::fromLatin1(data.mid(pos, length)));
                domainName.append(".");
                pos += length;
            }
        }
        domainName.chop(1);

        if (pos + 4 > data.size())
        {
            return QObject::tr("Malformed DNS response packet: Question type/class truncated");
        }
        pos += 4;

        result += QObject::tr("Question %1: Domain Name: %2\n").arg(i + 1).arg(domainName);
    }

    // Additional parsing can be added here for answers, authorities, etc.

    return result;
}

QByteArray DnsParser::createDnsQueryPacket(const QString &domain)
{
    QByteArray packet;

    // 生成随机 Transaction ID
    const quint16 transactionId = QRandomGenerator::global()->generate() & 0xFFFF;

    QDataStream stream(&packet, QDataStream::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);

    // Transaction ID
    stream << transactionId;

    // Flags (标准查询)
    stream << quint16{ 0x0100 };

    // Questions (1 个问题)
    stream << quint16{ 1 };

    // Answer RRs, Authority RRs, Additional RRs (都设置为 0)
    stream << quint16{ 0 } << quint16{ 0 } << quint16{ 0 };

    // QNAME (使用 Punycode 编码主机名)
    QUrl url("http://" + domain); // 使用 QUrl 处理域名
    if (!url.isValid())
    {
        qWarning() << QObject::tr("Invalid domain name:") << domain;
        return QByteArray();
    }

    QString punycodeHost = url.host(QUrl::FullyEncoded); // 获取 Punycode 编码后的主机名

    if (!writeQName(stream, punycodeHost))
    {
        qWarning() << QObject::tr("Failed to write QNAME");
        return QByteArray();
    }

    // QTYPE (A 记录)
    stream << quint16{ 0x0001 };

    // QCLASS (IN，表示 Internet)
    stream << quint16{ 0x0001 };

    return packet;
}

quint16 DnsParser::readUInt16(const QByteArray &data, int offset)
{
    if (offset + 1 >= data.size())
    {
        qWarning() << QObject::tr("Offset out of bounds");
        return 0;
    }
    return (static_cast<quint16>(data[offset]) << 8) | static_cast<quint16>(data[offset + 1]);
}

bool DnsParser::writeQName(QDataStream &stream, const QString &domain)
{
    const QStringList parts = domain.split('.', Qt::SkipEmptyParts);
    for (const QString &part : parts)
    {
        if (part.isEmpty())
        {
            return false; // 域名部分不能为空
        }
        QByteArray encodedPart = part.toLatin1(); // 使用 Latin1 编码，更符合 DNS 协议
        if (encodedPart.size() > 255)
        {
            qWarning() << QObject::tr("Domain part too long: ") << part;
            return false;
        }
        stream << quint8(encodedPart.size());
        stream.writeRawData(encodedPart.constData(), encodedPart.size());
    }
    stream << quint8(0);
    return true;
}
