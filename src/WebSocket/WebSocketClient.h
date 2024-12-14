#pragma once

#include <QByteArray>
#include <QObject>
#include <QUrl>
#include <QWebSocket>

class WebSocketClient : public QObject
{
    Q_OBJECT

public:
    explicit WebSocketClient(QObject *parent = nullptr);
    ~WebSocketClient();

public slots:
    void connectToServer(const QString &address, quint16 port); // 用于连接服务器
    void sendData(const QString &message);
    void disconnectFromServer();

protected slots:
    void onConnected();
    void onDisconnected();
    void onTextMessageReceived(const QString &message);
    void onErrorOccurred();

protected:
    QWebSocket m_webSocket;
    QUrl m_url;

signals:
    void error(const QString &errorMessage);
    void tip(const QString &tipMessage);
    void receivedData(const QString &str, const QString &hexEncode, const QByteArray &data);
};
