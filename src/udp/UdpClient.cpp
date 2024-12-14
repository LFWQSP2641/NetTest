#include "UdpClient.h"

UdpClient::UdpClient(QObject *parent)
    : QObject{ parent },
      m_socket{ new QUdpSocket{ this } }
{
    connect(m_socket, &QUdpSocket::readyRead, this, &UdpClient::onReadyRead);
}

UdpClient::~UdpClient()
{
    if (m_socket->isOpen())
    {
        m_socket->close();
    }
}

void UdpClient::connectToServer(const QString &address, quint16 port)
{
    if (m_socket->state() != QAbstractSocket::BoundState)
    {
        m_socket->bind(QHostAddress::Any, 0);
    }
    m_socket->connectToHost(address, port);
}

void UdpClient::disconnectFromServer()
{
    m_socket->disconnectFromHost();
}

void UdpClient::sendData(const QByteArray &data)
{
    m_socket->write(data);
}

void UdpClient::onReadyRead()
{
    while (m_socket->hasPendingDatagrams())
    {
        QByteArray data;
        data.resize(m_socket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        m_socket->readDatagram(data.data(), data.size(), &sender, &senderPort);
        emit receivedData(QString(data), data.toHex(), data);
    }
}
