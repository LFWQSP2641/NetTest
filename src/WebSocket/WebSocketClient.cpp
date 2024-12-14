#include "WebSocketClient.h"

#include <QDebug>

WebSocketClient::WebSocketClient(QObject *parent)
    : QObject(parent)
{
    connect(&m_webSocket, &QWebSocket::connected, this, &WebSocketClient::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &WebSocketClient::onDisconnected);
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &WebSocketClient::onTextMessageReceived);
    connect(&m_webSocket, &QWebSocket::errorOccurred, this, &WebSocketClient::onErrorOccurred);
}

WebSocketClient::~WebSocketClient()
{
    if (m_webSocket.state() == QAbstractSocket::ConnectedState)
    {
        m_webSocket.close();
    }
}

void WebSocketClient::connectToServer(const QString &address, quint16 port)
{
    QUrl m_url; // 存储 URL
    m_url.setScheme("ws");
    m_url.setHost(address);
    m_url.setPort(port);
    m_webSocket.open(m_url); // 连接到服务器
}

void WebSocketClient::sendData(const QString &message)
{
    if (m_webSocket.state() == QAbstractSocket::ConnectedState)
    {
        m_webSocket.sendTextMessage(message);
        emit tip(QObject::tr("Sent message: ").append(message));
    }
    else
    {
        emit error(QObject::tr("WebSocket is not open. Cannot send message."));
    }
}

void WebSocketClient::disconnectFromServer()
{
    if (m_webSocket.state() == QAbstractSocket::ConnectedState)
    {
        m_webSocket.close();
    }
}

void WebSocketClient::onConnected()
{
    emit tip(QObject::tr("Connected to WebSocket server."));
}

void WebSocketClient::onDisconnected()
{
    emit tip(QObject::tr("Disconnected from WebSocket server."));
}

void WebSocketClient::onTextMessageReceived(const QString &message)
{
    emit receivedData(message, message.toUtf8().toHex(), message.toUtf8());
}

void WebSocketClient::onErrorOccurred()
{
    emit error(QObject::tr("WebSocket error occurred: ").append(m_webSocket.errorString()));
}
