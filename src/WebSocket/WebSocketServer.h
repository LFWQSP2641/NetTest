#pragma once

#include <QObject>
#include <QWebSocketServer>

class WebSocketServer : public QObject
{
    Q_OBJECT

public:
    explicit WebSocketServer(QObject *parent = nullptr);
    ~WebSocketServer();

public slots:
    void startServer(quint16 port);
    void stopServer();

protected slots:
    void onNewConnection();
    void onMessageReceived(const QString &message);
    void onSocketDisconnected();

protected:
    QWebSocketServer *m_server;
    QMap<QWebSocket *, QString> m_clients;

signals:
    void error(const QString &errorMessage);
    void tip(const QString &tipMessage);
    void receivedData(const QString &str, const QString &hexEncode, const QByteArray &data);
};
