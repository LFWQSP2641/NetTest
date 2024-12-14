#pragma once

#include <QObject>
#include <QUdpSocket>

class UdpClient : public QObject
{
    Q_OBJECT

public:
    explicit UdpClient(QObject *parent = nullptr);
    ~UdpClient();

public slots:
    void connectToServer(const QString &address, quint16 port);
    void disconnectFromServer();
    void sendData(const QByteArray &data);

protected slots:
    void onReadyRead();

protected:
    QUdpSocket *m_socket;

signals:
    void error(const QString &errorMessage);
    void tip(const QString &tipMessage);
    void receivedData(const QString &str, const QString &hexEncode, const QByteArray &data);
};
