#include "TcpServer.h"

#include <QTcpSocket>

TcpServer::TcpServer(QObject *parent)
    : QTcpServer(parent)
{
}

void TcpServer::startServer(quint16 port)
{
    if (!listen(QHostAddress::Any, port))
    {
        emit error(errorString());
    }
    else
    {
        emit tip(QObject::tr("Server started on port: ").append(QString::number(port)));
    }
}

void TcpServer::stopServer()
{
    for (QTcpSocket *client : m_clients)
    {
        client->disconnectFromHost();
        client->deleteLater();
    }
    close();
    emit tip(QObject::tr("Server stopped"));
}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket *clientSocket = new QTcpSocket(this);

    if (clientSocket->setSocketDescriptor(socketDescriptor))
    {
        m_clients.append(clientSocket);
        emit tip(QObject::tr("New client connected: ").append(clientSocket->peerAddress().toString()).append(QStringLiteral(":")).append(QString::number(clientSocket->peerPort())));

        // 连接信号槽
        connect(clientSocket, &QTcpSocket::readyRead, this, &TcpServer::onClientReadyRead);
        connect(clientSocket, &QTcpSocket::disconnected, this, &TcpServer::onClientDisconnected);
    }
    else
    {
        delete clientSocket;
    }
}

void TcpServer::onClientDisconnected()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    if (clientSocket)
    {
        emit tip(QObject::tr("Client disconnected: ").append(clientSocket->peerAddress().toString()).append(QStringLiteral(":")).append(QString::number(clientSocket->peerPort())));
        disconnect(clientSocket, &QTcpSocket::readyRead, this, &TcpServer::onClientReadyRead);
        disconnect(clientSocket, &QTcpSocket::disconnected, this, &TcpServer::onClientDisconnected);
        m_clients.removeAll(clientSocket);
        clientSocket->deleteLater();
    }
}

void TcpServer::onClientReadyRead()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    if (clientSocket)
    {
        const auto data(clientSocket->readAll());
        qDebug() << "Received data from client: " << clientSocket->peerAddress().toString() << ":" << clientSocket->peerPort();
        qDebug() << QString(data);
        emit receivedData(QString(data), data.toHex(), data);
    }
}
