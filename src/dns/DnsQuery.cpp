#include "DnsQuery.h"

#include "DnsParser.h"

#include <QDateTime>

DnsQuery::DnsQuery(QObject *parent)
    : QObject {parent},
      m_socket {new QUdpSocket {this}},
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
    if (dnsServer != m_dnsServer || port != m_port)
    {
        if (!m_dnsServer.isEmpty())
        {
            m_socket->close();
        }
        m_socket->connectToHost(dnsServer, port);
        m_dnsServer = dnsServer;
        m_port = port;
    }
    m_socket->write(DnsParser::buildDnsQueryPacket(domain, recordType, recordClass));
}

void DnsQuery::onReadyRead()
{
    while (m_socket->hasPendingDatagrams())
    {
        const quint64 currentTime = QDateTime::currentMSecsSinceEpoch();
        const quint64 elapsedTime = currentTime - m_startTime;
        QByteArray data;
        data.resize(m_socket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        m_socket->readDatagram(data.data(), data.size(), &sender, &senderPort);
        emit tip(QObject::tr("Received data from ").append(sender.toString()).append(QStringLiteral(":")).append(QString::number(senderPort)));
        emit receivedData(QString(data), data.toHex(), data);
        const auto response = DnsParser::parseDnsResponsePacket(data);
        if (!response)
        {
            emit error(QObject::tr("Failed to parse DNS response"));
            return;
        }
        emit lookupFinished(m_domain, DnsParser::dnsMessageToString(response.value()), elapsedTime);
    }
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
