#pragma once

#include <QObject>
#include <QUdpSocket>

class UdpServer : public QObject
{
    Q_OBJECT

public:
    explicit UdpServer(QObject *parent = nullptr);
    ~UdpServer();

public slots:
    void startServer(quint16 port);
    void stopServer();

protected slots:
    void onReadyRead();

protected:
    QUdpSocket *m_socket;
signals:
    void error(const QString &errorMessage);
    void tip(const QString &tipMessage);
    void receivedData(const QString &str, const QString &hexEncode, const QByteArray &data);
};
