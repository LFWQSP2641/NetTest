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

    QString font() const;
    void setFont(const QString &newFont);

    int fontPointSize() const;
    void setFontPointSize(int newFontPointSize);

    QString style() const;
    void setStyle(const QString &newStyle);

public slots:
    void saveSettings() const;

protected:
    QString m_font;
    int m_fontPointSize;
    QString m_style;
signals:
    void fontChanged();
    void fontPointSizeChanged();
    void styleChanged();

private:
    Q_PROPERTY(QString font READ font WRITE setFont NOTIFY fontChanged FINAL)
    Q_PROPERTY(int fontPointSize READ fontPointSize WRITE setFontPointSize NOTIFY fontPointSizeChanged FINAL)
    Q_PROPERTY(QString style READ style WRITE setStyle NOTIFY styleChanged FINAL)
};
