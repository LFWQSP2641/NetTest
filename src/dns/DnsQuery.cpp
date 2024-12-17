#include "DnsQuery.h"

#include "DnsParser.h"

#include <QDateTime>
#include <QRandomGenerator>

DnsQuery::DnsQuery(QObject *parent)
    : QObject{ parent },
      m_socket{ new QUdpSocket{ this } }
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
    m_port = port;
    m_domain = domain;
    m_startTime = QDateTime::currentMSecsSinceEpoch();
    if (dnsServer != m_dnsServer)
    {
        if (!m_dnsServer.isEmpty())
        {
            m_socket->close();
        }
        m_socket->connectToHost(dnsServer, port);
        m_dnsServer = dnsServer;
    }
    m_socket->write(DnsParser::createDnsQueryPacket(domain));
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
        emit receivedData(data);
        const auto response = DnsParser::parseDnsResponsePacket(data);
        emit lookupFinished(m_domain, response.answers.isEmpty() ? "" : response.answers.first().value, elapsedTime);
    }
}
