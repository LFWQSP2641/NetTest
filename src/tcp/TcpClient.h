#pragma once

#include <QObject>
#include <QTcpSocket>

class TcpClient : public QObject
{
    Q_OBJECT

public:
    explicit TcpClient(QObject *parent = nullptr);
    ~TcpClient();
public slots:
    void connectToServer(const QString &address, quint16 port);
    void disconnectFromServer();
    void sendData(const QByteArray &data);

protected slots:
    void onReadyRead();
    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError socketError);

protected:
    QTcpSocket *m_socket;

signals:
    void error(const QString &errorMessage);
    void tip(const QString &tipMessage);
    void receivedData(const QString &str, const QString &hexEncode, const QByteArray &data);
};
