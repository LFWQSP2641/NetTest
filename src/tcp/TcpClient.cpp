#include "TcpClient.h"

TcpClient::TcpClient(QObject *parent)
    : QObject{ parent },
      m_socket{ new QTcpSocket{ this } }
{
    // 连接槽函数
    connect(m_socket, &QTcpSocket::readyRead, this, &TcpClient::onReadyRead);
    connect(m_socket, &QTcpSocket::connected, this, &TcpClient::onConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &TcpClient::onDisconnected);
    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred), this, &TcpClient::onError);
}

TcpClient::~TcpClient()
{
    if (m_socket->isOpen())
    {
        m_socket->close();
    }
}

void TcpClient::connectToServer(const QString &address, quint16 port)
{
    if (m_socket->state() != QAbstractSocket::ConnectedState)
    {
        m_socket->connectToHost(address, port);
    }
}

void TcpClient::disconnectFromServer()
{
    if (m_socket->state() == QAbstractSocket::ConnectedState)
    {
        m_socket->disconnectFromHost();
    }
}

void TcpClient::sendData(const QByteArray &data)
{
    if (m_socket->state() == QAbstractSocket::ConnectedState)
    {
        m_socket->write(data);
    }
    else
    {
        emit error(QObject::tr("Not connected to server"));
    }
}

void TcpClient::onReadyRead()
{
    emit tip(QObject::tr("Received data from server"));
    const auto data(m_socket->readAll());
    emit receivedData(QString(data), data.toHex(), data);
}

void TcpClient::onConnected()
{
    emit tip(QObject::tr("Connected to server"));
}

void TcpClient::onDisconnected()
{
    emit tip(QObject::tr("Disconnected from server"));
}

void TcpClient::onError(QAbstractSocket::SocketError socketError)
{
    emit error(m_socket->errorString());
}
