#include "src/Global.h"
#include "src/Settings.h"
#include "src/WebSocket/WebSocketClient.h"
#include "src/WebSocket/WebSocketServer.h"
#include "src/dns/DnsQuery.h"
#include "src/tcp/TcpClient.h"
#include "src/tcp/TcpServer.h"
#include "src/udp/UdpClient.h"
#include "src/udp/UdpServer.h"

#include <QFont>
#include <QGuiApplication>
#include <QNetworkProxy>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Global::initOnce();
    Settings::initOnce();
    auto settings(Settings::getInstance());

    QFont appFont;
    if (settings->fontPointSize() < 1 || settings->font().isEmpty())
    {
        settings->setFontPointSize(app.font().pointSize());
        settings->setFont(app.font().family());
        settings->saveSettings();
    }
    else
    {
        appFont.setFamily(settings->font());
        appFont.setPointSize(settings->fontPointSize());
        app.setFont(appFont);
    }

    if (qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE"))
        QQuickStyle::setStyle(settings->style());

    // If this is the first time we're running the application,
    // we need to set a style in the settings so that the QML
    // can find it in the list of built-in styles.
    if (settings->style().isEmpty())
        settings->setStyle(QQuickStyle::name());

    QNetworkProxyFactory::setUseSystemConfiguration(false);
    if (settings->isProxyAvailable())
    {
        QNetworkProxy proxy;
        proxy.setType(QNetworkProxy::Socks5Proxy);
        proxy.setHostName(settings->proxyHost());
        proxy.setPort(settings->proxyPort());
        QNetworkProxy::setApplicationProxy(proxy);
    }

    // 获取系统语言
    QString systemLanguage = QLocale::system().name();
    qDebug() << "System Language:" << systemLanguage;

    // 创建翻译器
    QTranslator translator;

    // 如果语言是中文
    if (systemLanguage == QStringLiteral("zh_CN"))
    {
        bool loaded = translator.load(":/NetTest_zh_CN.qm");
        qDebug() << "Translation loaded:" << loaded;

        if (loaded)
        {
            app.installTranslator(&translator);
        }
    }

    qmlRegisterSingletonInstance("NetTest", 1, 0, "Settings", settings);

    qmlRegisterType<TcpClient>("NetTest", 1, 0, "TcpClient");
    qmlRegisterType<TcpServer>("NetTest", 1, 0, "TcpServer");
    qmlRegisterType<UdpClient>("NetTest", 1, 0, "UdpClient");
    qmlRegisterType<UdpServer>("NetTest", 1, 0, "UdpServer");
    qmlRegisterType<WebSocketClient>("NetTest", 1, 0, "WebSocketClient");
    qmlRegisterType<WebSocketServer>("NetTest", 1, 0, "WebSocketServer");
    qmlRegisterType<DnsQuery>("NetTest", 1, 0, "DnsQuery");

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []()
    {
        QCoreApplication::exit(-1);
    },
        Qt::QueuedConnection);

    QStringList builtInStyles = {QStringLiteral("Basic"), QStringLiteral("Fusion"), QStringLiteral("Imagine"), QStringLiteral("Material"), QStringLiteral("Universal")};
#if defined(Q_OS_MACOS)
    builtInStyles << QStringLiteral("macOS");
    builtInStyles << QStringLiteral("iOS");
#elif defined(Q_OS_IOS)
    builtInStyles << QStringLiteral("iOS");
#elif defined(Q_OS_WINDOWS)
    builtInStyles << QStringLiteral("Windows");
#endif

    engine.setInitialProperties({
        {QStringLiteral("builtInStyles"), builtInStyles}
    });

    engine.loadFromModule("NetTest", "Main");

    return app.exec();
}
