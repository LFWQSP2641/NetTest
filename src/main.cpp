#include "src/WebSocket/WebSocketClient.h"
#include "src/WebSocket/WebSocketServer.h"
#include "src/dns/DnsQuery.h"
#include "src/tcp/TcpClient.h"
#include "src/tcp/TcpServer.h"
#include "src/udp/UdpClient.h"
#include "src/udp/UdpServer.h"

#include <QGuiApplication>
#include <QNetworkProxy>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QNetworkProxyFactory::setUseSystemConfiguration(false);

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
        { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    QStringList builtInStyles = { QStringLiteral("Basic"), QStringLiteral("Fusion"),
                                  QStringLiteral("Imagine"), QStringLiteral("Material"), QStringLiteral("Universal") };
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
