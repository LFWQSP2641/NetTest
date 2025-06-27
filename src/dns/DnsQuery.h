#pragma once

#include <QHostAddress>
#include <QObject>
#include <QUdpSocket>

class DnsQuery : public QObject
{
    Q_OBJECT
    Q_PROPERTY(DnsQuery::RecordType recordType READ getRecordType WRITE setRecordType NOTIFY recordTypeChanged FINAL)
    Q_PROPERTY(DnsQuery::RecordClass recordClass READ getRecordClass WRITE setRecordClass NOTIFY recordClassChanged FINAL)

public:
    explicit DnsQuery(QObject *parent = nullptr);
    ~DnsQuery();

    enum RecordType : uint16_t
    {
        A = 1,
        NS = 2,
        CNAME = 5,
        SOA = 6,
        PTR = 12,
        MX = 15,
        TXT = 16,
        AAAA = 28,
        SPF = 99,
        UknownType = 65280
    };
    Q_ENUM(RecordType)

    enum RecordClass : uint16_t
    {
        INTERNET = 1,
        CSNET = 2,
        CHAOS = 3,
        HESIOD = 4,
        NONE = 254,
        ANY = 255,
        UknownClass = 65280
    };
    Q_ENUM(RecordClass)

    RecordType getRecordType() const;
    void setRecordType(RecordType newRecordType);

    RecordClass getRecordClass() const;
    void setRecordClass(RecordClass newRecordClass);

public slots:
    void query(const QString &domain, const QString &dnsServer = QStringLiteral("1.1.1.1"), quint16 port = 53);

protected slots:
    void onReadyRead();

signals:
    void error(const QString &errorMessage);
    void tip(const QString &tipMessage);
    void receivedData(const QString &str, const QString &hexEncode, const QByteArray &data);
    void lookupFinished(const QString &domain, const QString &result, quint64 elapsedTime);
    void recordTypeChanged();
    void recordClassChanged();

private:
    QUdpSocket *m_socket;
    QString m_dnsServer;
    quint16 m_port;
    QString m_domain;
    quint64 m_startTime;

    RecordType recordType = A;
    RecordClass recordClass = INTERNET;
};
