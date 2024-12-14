#include "WebSocketServer.h"

#include <QDebug>
#include <QMap>
#include <QWebSocket>

WebSocketServer::WebSocketServer(QObject *parent)
    : QObject(parent), m_server(nullptr)
{
}

WebSocketServer::~WebSocketServer()
{
    stopServer();
}

void WebSocketServer::startServer(quint16 port)
{
    if (m_server)
    {
        emit error(QObject::tr("WebSocket server is already running."));
        return;
    }

    m_server = new QWebSocketServer(QStringLiteral("WebSocket Server"),
                                    QWebSocketServer::NonSecureMode, this);

    if (!m_server->listen(QHostAddress::Any, port))
    {
        emit error(QObject::tr("Failed to start WebSocket server."));
        return;
    }

    connect(m_server, &QWebSocketServer::newConnection, this, &WebSocketServer::onNewConnection);
    emit tip(QObject::tr("WebSocket server started on port: ").append(QString::number(port)));
}

void WebSocketServer::stopServer()
{
    if (m_server)
    {
        m_server->close();
        emit tip(QObject::tr("WebSocket server stopped."));
        // Clean up client connections before deleting the server
        qDeleteAll(m_clients.keys());
        m_clients.clear();
        m_server->deleteLater();
        m_server = nullptr;
    }
}

void WebSocketServer::onNewConnection()
{
    QWebSocket *client = m_server->nextPendingConnection();
    connect(client, &QWebSocket::disconnected, this, &WebSocketServer::onSocketDisconnected);
    connect(client, &QWebSocket::textMessageReceived, this, &WebSocketServer::onMessageReceived);

    m_clients[client] = client->peerAddress().toString();
    emit tip(QObject::tr("New connection from: ").append(m_clients[client]));
}

void WebSocketServer::onMessageReceived(const QString &message)
{
    emit receivedData(message, message.toUtf8().toHex(), message.toUtf8());
}

void WebSocketServer::onSocketDisconnected()
{
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());
    if (client)
    {
        emit tip(QObject::tr("Client disconnected: ").append(m_clients[client]));
        m_clients.remove(client);
        client->deleteLater();
    }
}
