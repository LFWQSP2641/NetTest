#include "Settings.h"

#include "Global.h"

#include <QJsonDocument>
#include <QJsonObject>

Settings *Settings::instance = nullptr;

Settings::Settings(QObject *parent)
    : QObject{ parent }
{
    // 读取设置
    QFile file(Global::configPath().append(QStringLiteral("/settings.json")));
    if (file.open(QIODevice::ReadOnly))
    {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonObject settings = doc.object();
        m_font = settings.value(QStringLiteral("font")).toString();
        m_fontPointSize = settings.value(QStringLiteral("fontPointSize")).toInt();
        m_style = settings.value(QStringLiteral("style")).toString();
    }
    file.close();
}

Settings::~Settings()
{
    saveSettings();
}

void Settings::initOnce()
{
    if (instance == nullptr)
    {
        instance = new Settings();
    }
}

Settings *Settings::getInstance()
{
    return instance;
}

void Settings::destroyInstance()
{
    if (instance != nullptr)
    {
        delete instance;
        instance = nullptr;
    }
}

QString Settings::font() const
{
    return m_font;
}

void Settings::setFont(const QString &newFont)
{
    if (m_font == newFont)
        return;
    m_font = newFont;
    emit fontChanged();
}

int Settings::fontPointSize() const
{
    return m_fontPointSize;
}

void Settings::setFontPointSize(int newFontPointSize)
{
    if (m_fontPointSize == newFontPointSize)
        return;
    m_fontPointSize = newFontPointSize;
    emit fontPointSizeChanged();
}

QString Settings::style() const
{
    return m_style;
}

void Settings::setStyle(const QString &newStyle)
{
    if (m_style == newStyle)
        return;
    m_style = newStyle;
    emit styleChanged();
}

void Settings::saveSettings() const
{
    // JSON格式保存设置
    QJsonObject settings;
    settings.insert(QStringLiteral("font"), m_font);
    settings.insert(QStringLiteral("fontPointSize"), m_fontPointSize);
    settings.insert(QStringLiteral("style"), m_style);
    QFile file(Global::configPath().append(QStringLiteral("/settings.json")));
    if (file.open(QIODevice::WriteOnly))
    {
        QJsonDocument doc(settings);
        file.write(doc.toJson(QJsonDocument::Compact));
    }
    file.close();
}
