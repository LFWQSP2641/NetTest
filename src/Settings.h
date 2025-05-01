#pragma once

#include <QObject>

// 单例类
class Settings : public QObject
{
    Q_OBJECT
    explicit Settings(QObject *parent = nullptr);
    static Settings *instance;

public:
    Settings(const Settings &) = delete;
    Settings &operator=(const Settings &) = delete;
    ~Settings();
    static void initOnce();
    static Settings *getInstance();
    static void destroyInstance();

    Q_INVOKABLE bool isProxyAvailable() const;

    QString font() const;
    void setFont(const QString &newFont);

    int fontPointSize() const;
    void setFontPointSize(int newFontPointSize);

    QString style() const;
    void setStyle(const QString &newStyle);

    QString proxyHost() const;
    void setProxyHost(const QString &newProxyHost);

    int proxyPort() const;
    void setProxyPort(int newProxyPort);

public slots:
    void saveSettings() const;

protected:
    QString m_font;
    int m_fontPointSize;
    QString m_style;

    QString m_proxyHost;
    int m_proxyPort;
signals:
    void fontChanged();
    void fontPointSizeChanged();
    void styleChanged();
    void proxyHostChanged();
    void proxyPortChanged();

private:
    Q_PROPERTY(QString font READ font WRITE setFont NOTIFY fontChanged FINAL)
    Q_PROPERTY(int fontPointSize READ fontPointSize WRITE setFontPointSize NOTIFY fontPointSizeChanged FINAL)
    Q_PROPERTY(QString style READ style WRITE setStyle NOTIFY styleChanged FINAL)
    Q_PROPERTY(QString proxyHost READ proxyHost WRITE setProxyHost NOTIFY proxyHostChanged FINAL)
    Q_PROPERTY(int proxyPort READ proxyPort WRITE setProxyPort NOTIFY proxyPortChanged FINAL)
};
