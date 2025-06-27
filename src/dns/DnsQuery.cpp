#include "DnsQuery.h"

#include "DnsParser.h"

#include <QDateTime>

DnsQuery::DnsQuery(QObject *parent)
    : QObject(parent),
      m_socket(new QUdpSocket(this)),
      m_port(0),
      m_startTime(0)
{
    connect(m_socket, &QUdpSocket::readyRead, this, &DnsQuery::onReadyRead);
}

DnsQuery::~DnsQuery()
{
    if (m_socket->isOpen())
    {
        m_socket->close();
    }
}

void DnsQuery::query(const QString &domain, const QString &dnsServer, quint16 port)
{
    m_domain = domain;
    m_startTime = QDateTime::currentMSecsSinceEpoch();

    if (m_socket->state() != QAbstractSocket::BoundState)
    {
        if (!m_socket->bind(QHostAddress::AnyIPv4, 0)) // 自动分配本地端口
        {
            emit error(QObject::tr("Failed to bind UDP socket: %1").arg(m_socket->errorString()));
            return;
        }
    }

    m_dnsServer = dnsServer;
    m_port = port;

    const QByteArray queryPacket = DnsParser::buildDnsQueryPacket(domain, recordType, recordClass);
    const QHostAddress dnsAddress(dnsServer);

    qint64 sent = m_socket->writeDatagram(queryPacket, dnsAddress, port);
    if (sent == -1)
    {
        emit error(QObject::tr("Failed to send DNS query: %1").arg(m_socket->errorString()));
    }
}

void DnsQuery::onReadyRead()
{
    while (m_socket->hasPendingDatagrams())
    {
        const quint64 currentTime = QDateTime::currentMSecsSinceEpoch();
        const quint64 elapsedTime = currentTime - m_startTime;

        QByteArray data;
        data.resize(int(m_socket->pendingDatagramSize()));

        QHostAddress sender;
        quint16 senderPort;
        m_socket->readDatagram(data.data(), data.size(), &sender, &senderPort);

        emit tip(QObject::tr("Received data from %1:%2").arg(sender.toString()).arg(senderPort));
        emit receivedData(QString::fromUtf8(data), data.toHex(), data);

        const auto response = DnsParser::parseDnsResponsePacket(data);
        if (!response)
        {
            emit error(QObject::tr("Failed to parse DNS response"));
            return;
        }

        emit lookupFinished(m_domain, DnsParser::dnsMessageToString(response.value()), elapsedTime);
    }
}

DnsQuery::RecordType DnsQuery::getRecordType() const
{
    return recordType;
}

void DnsQuery::setRecordType(DnsQuery::RecordType newRecordType)
{
    if (recordType == newRecordType)
        return;

    recordType = newRecordType;
    emit recordTypeChanged();
}

DnsQuery::RecordClass DnsQuery::getRecordClass() const
{
    return recordClass;
}

void DnsQuery::setRecordClass(DnsQuery::RecordClass newRecordClass)
{
    if (recordClass == newRecordClass)
        return;

    recordClass = newRecordClass;
    emit recordClassChanged();
}
