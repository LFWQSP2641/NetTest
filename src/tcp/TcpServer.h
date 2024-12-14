#pragma once

#include <QTcpServer>

class TcpServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit TcpServer(QObject *parent = nullptr);
public slots:
    void startServer(quint16 port);
    void stopServer();

protected:
    void incomingConnection(qintptr socketDescriptor) override;
protected slots:
    void onClientDisconnected();
    void onClientReadyRead();
signals:
    void receivedData(const QString &str, const QString &hexEncode, const QByteArray &data);
    void error(const QString &errorMessage);
    void tip(const QString &tipMessage);
};
