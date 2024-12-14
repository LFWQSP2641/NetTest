#include "UdpServer.h"

UdpServer::UdpServer(QObject *parent)
    : QObject{ parent },
      m_socket{ new QUdpSocket(this) }
{
}

UdpServer::~UdpServer()
{
    if (m_socket->isOpen())
    {
        m_socket->close();
    }
}

void UdpServer::startServer(quint16 port)
{
    if (m_socket->bind(QHostAddress::Any, port))
    {
        connect(m_socket, &QUdpSocket::readyRead, this, &UdpServer::onReadyRead);
        emit tip(QObject::tr("Server started on port: ").append(QString::number(port)));
    }
    else
    {
        emit error(m_socket->errorString());
    }
}

void UdpServer::stopServer()
{
    if (m_socket)
    {
        m_socket->close();
        emit tip(QObject::tr("Server stopped"));
    }
}

void UdpServer::onReadyRead()
{
    while (m_socket->hasPendingDatagrams())
    {
        QByteArray data;
        data.resize(m_socket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        m_socket->readDatagram(data.data(), data.size(), &sender, &senderPort);
        emit tip(QObject::tr("Received data from ").append(sender.toString()).append(QStringLiteral(":")).append(QString::number(senderPort)));
        emit receivedData(QString(data), data.toHex(), data);
    }
}
