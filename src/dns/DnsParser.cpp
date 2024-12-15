#include "DnsParser.h"

#include <QDebug>
#include <QHostAddress>
#include <QObject>
#include <QRandomGenerator>
#include <QUrl>

DnsResponse DnsParser::parseDnsResponsePacket(const QByteArray &data)
{
    DnsResponse response;

    if (data.size() < 12)
    {
        response.transactionId = QObject::tr("Invalid DNS response packet");
        return response;
    }

    // 读取头部字段
    response.transactionId = QString::number(readUInt16(data, 0));
    response.flags = QString("%1").arg(readUInt16(data, 2), 4, 16, QLatin1Char('0'));
    quint16 questions = readUInt16(data, 4);
    quint16 answerRRs = readUInt16(data, 6);

    // 解析问题部分
    int pos = 12;
    for (int i = 0; i < questions; ++i)
    {
        QString domainName;
        while (true)
        {
            if (pos >= data.size())
                return response;

            quint8 length = static_cast<quint8>(data[pos]);
            pos++;
            if (length == 0)
                break;

            if ((length & 0xC0) == 0xC0)
            {
                quint16 pointer = ((length & 0x3F) << 8) | static_cast<quint8>(data[pos]);
                pos++;
                // 在需要时实现指针解压
                domainName += QObject::tr("Compression not supported");
                break;
            }
            else
            {
                domainName.append(QString::fromLatin1(data.mid(pos, length)));
                domainName.append(".");
                pos += length;
            }
        }
        domainName.chop(1); // 移除最后的点
        response.questions.append(domainName);

        if (pos + 4 > data.size())
            return response;
        pos += 4; // 跳过类型和类
    }

    // 解析回答部分
    for (int i = 0; i < answerRRs; ++i)
    {
        if (pos + 12 > data.size())
            return response;

        QString domainName = QObject::tr("Compressed or raw domain");
        quint16 type = readUInt16(data, pos + 2);
        quint16 dataLength = readUInt16(data, pos + 10);
        pos += 12;

        if (type == 1 && dataLength == 4) // IPv4 地址
        {
            QByteArray rawIpData = data.mid(pos, dataLength);
            QString ipv4Address = QString("%1.%2.%3.%4")
                                      .arg(static_cast<quint8>(rawIpData[0]))
                                      .arg(static_cast<quint8>(rawIpData[1]))
                                      .arg(static_cast<quint8>(rawIpData[2]))
                                      .arg(static_cast<quint8>(rawIpData[3]));
            response.answers.append({ domainName, "A", ipv4Address });
        }
        else
        {
            response.answers.append({ domainName, QString("Type %1").arg(type), QObject::tr("Unsupported or invalid") });
        }

        pos += dataLength;
    }

    return response;
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
        qWarning() << QObject::tr("readUInt16: Offset out of bounds");
        return 0;
    }
    return (static_cast<quint16>(data[offset]) << 8) | static_cast<quint16>(data[offset + 1]);
}

quint16 DnsParser::readUInt32(const QByteArray &data, int offset)
{
    if (offset + 4 > data.size())
    {
        qWarning() << QObject::tr("readUInt32: Offset out of bounds");
        return 0;
    }
    return (static_cast<quint32>(static_cast<quint8>(data[offset])) << 24) |
           (static_cast<quint32>(static_cast<quint8>(data[offset + 1])) << 16) |
           (static_cast<quint32>(static_cast<quint8>(data[offset + 2])) << 8) |
           static_cast<quint32>(static_cast<quint8>(data[offset + 3]));
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
