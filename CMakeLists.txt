cmake_minimum_required(VERSION 3.16)

project(NetTest VERSION 0.3.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Quick QuickControls2 LinguistTools Network WebSockets)

qt_standard_project_setup(
    REQUIRES 6.5
    I18N_TRANSLATED_LANGUAGES zh_CN
)

qt_add_translations(${PROJECT_NAME}
    RESOURCE_PREFIX "/"
    TS_FILE_DIR translations
)

qt_add_executable(${PROJECT_NAME}
    src/main.cpp
    src/Global.h src/Global.cpp
)

qt_add_qml_module(${PROJECT_NAME}
    URI NetTest
    VERSION 1.0
    SOURCES src/tcp/TcpServer.h src/tcp/TcpServer.cpp
    SOURCES src/tcp/TcpClient.h src/tcp/TcpClient.cpp
    SOURCES src/udp/UdpServer.h src/udp/UdpServer.cpp
    SOURCES src/udp/UdpClient.h src/udp/UdpClient.cpp
    SOURCES src/dns/DnsQuery.h src/dns/DnsQuery.cpp
    SOURCES src/dns/DnsParser.h src/dns/DnsParser.cpp
    SOURCES src/WebSocket/WebSocketServer.h src/WebSocket/WebSocketServer.cpp
    SOURCES src/WebSocket/WebSocketClient.h src/WebSocket/WebSocketClient.cpp
    QML_FILES page/Main.qml page/TextAreaButton.qml
    QML_FILES page/ClientPage.qml page/ServerPage.qml
    QML_FILES page/MainPage.qml page/DNSQueryPage.qml
    RESOURCES android/AndroidManifest.xml android/build.gradle android/res/values/libs.xml android/res/xml/qtprovider_paths.xml android/gradle/wrapper/gradle-wrapper.jar android/gradle/wrapper/gradle-wrapper.properties android/gradle.properties android/gradlew android/gradlew.bat
    SOURCES src/Settings.h src/Settings.cpp
    SOURCES 3rd/dnspacket/dns.hpp
)

qt_add_resources(${PROJECT_NAME} "qml"
    PREFIX "/"
    FILES "page/icon/angle-left.svg" "page/icon/menu-burger.svg" "page/icon/menu-dots-vertical.svg"
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER com.LFWQSP2641.${PROJECT_NAME}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    ANDROID_VERSION_NAME ${CMAKE_PROJECT_VERSION}
    ANDROID_VERSION_CODE 5
    QT_ANDROID_PACKAGE_NAME com.LFWQSP2641.${PROJECT_NAME}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE Qt6::Core Qt6::Quick Qt6::Network Qt6::WebSockets Qt6::QuickControls2
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
