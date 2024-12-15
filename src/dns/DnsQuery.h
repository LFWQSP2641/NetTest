#pragma once

#include <QHostAddress>
#include <QObject>
#include <QUdpSocket>

class DnsQuery : public QObject
{
    Q_OBJECT

public:
    explicit DnsQuery(QObject *parent = nullptr);
    ~DnsQuery();

public slots:
    void query(const QString &domain, const QString &dnsServer = QStringLiteral("1.1.1.1"), quint16 port = 53);

protected slots:
    void onReadyRead();

protected:
    QUdpSocket *m_socket;
    QString m_dnsServer;
    quint16 m_port;
    QString m_domain;
    quint64 m_startTime;

signals:
    void error(const QString &errorMessage);
    void tip(const QString &tipMessage);
    void receivedData(const QString &data);
    void lookupFinished(const QString &domain, const QString &ip, int timeMs);
};
