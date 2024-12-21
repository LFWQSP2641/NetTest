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
        INTERNET = 1,       /* Internet	    	RFC-1035 */
        CSNET = 2,          /* CSNET (obsolete)    	RFC-1035 */
        CHAOS = 3,          /* CHAOS		RFC-1035 */
        HESIOD = 4,         /* Hesiod		RFC-1035 */
        NONE = 254,         /* 			RFC-2136 */
        ANY = 255,          /* All classes		RFC-1035 */
        UknownClass = 65280 /* Unknown class 	RFC-2929 */
    };
    Q_ENUM(RecordClass)

    DnsQuery::RecordType getRecordType() const;
    void setRecordType(DnsQuery::RecordType newRecordType);

    DnsQuery::RecordClass getRecordClass() const;
    void setRecordClass(DnsQuery::RecordClass newRecordClass);

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

    DnsQuery::RecordType recordType = DnsQuery::RecordType::A;
    DnsQuery::RecordClass recordClass = DnsQuery::RecordClass::INTERNET;

signals:
    void error(const QString &errorMessage);
    void tip(const QString &tipMessage);
    void receivedData(const QString &str, const QString &hexEncode, const QByteArray &data);
    void lookupFinished(const QString &domain, const QString result, quint64 elapsedTime);
    void recordTypeChanged();
    void recordClassChanged();

private:
    Q_PROPERTY(DnsQuery::RecordType recordType READ getRecordType WRITE setRecordType NOTIFY recordTypeChanged FINAL)
    Q_PROPERTY(DnsQuery::RecordClass recordClass READ getRecordClass WRITE setRecordClass NOTIFY recordClassChanged FINAL)
};
