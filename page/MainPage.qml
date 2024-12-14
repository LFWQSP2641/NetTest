pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Layouts
import NetTest

Item {
    id: mainPage
    required property var stackViewObject
    RowLayout {
        anchors.fill: parent

        // 左侧是服务器
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            // TCP服务器
            TextAreaButton {
                Layout.fillWidth: true
                Layout.fillHeight: true
                title: qsTr("TCP Server")
                function clickFunction() {
                    mainPage.stackViewObject.push(tcpServerComponent)
                }
            }

            // UDP服务器
            TextAreaButton {
                Layout.fillWidth: true
                Layout.fillHeight: true
                title: qsTr("UDP Server")
                function clickFunction() {
                    mainPage.stackViewObject.push(udpServerComponent)
                }
            }

            // WebSocket服务器
            TextAreaButton {
                Layout.fillWidth: true
                Layout.fillHeight: true
                title: qsTr("WebSocket Server")
                function clickFunction() {
                    mainPage.stackViewObject.push(webSocketServerComponent)
                }
            }
        }

        // 右侧是客户端
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            // TCP客户端
            TextAreaButton {
                Layout.fillWidth: true
                Layout.fillHeight: true
                title: qsTr("TCP Client")
                function clickFunction() {
                    mainPage.stackViewObject.push(tcpClientComponent)
                }
            }

            // UDP客户端
            TextAreaButton {
                Layout.fillWidth: true
                Layout.fillHeight: true
                title: qsTr("UDP Client")
                function clickFunction() {
                    mainPage.stackViewObject.push(udpClientComponent)
                }
            }

            // WebSocket客户端
            TextAreaButton {
                Layout.fillWidth: true
                Layout.fillHeight: true
                title: qsTr("WebSocket Client")
                function clickFunction() {
                    mainPage.stackViewObject.push(webSocketClientComponent)
                }
            }
        }
    }
    Component {
        id: tcpServerComponent
        ServerPage {
            id: tcpServer
            serverObject: TcpServer {
                onError: function(error) {
                    console.log("TCP Server Error: " + error)
                    tcpServer.outputErrorMessage(error)
                }
                onTip: function(tip) {
                    console.log("TCP Server Tip: " + tip)
                    tcpServer.outputTipMessage(tip)
                }
                onReceivedData: function(str, hexEncode) {
                    console.log("TCP Server Received: " + str + "\n"
                                + hexEncode)
                    tcpServer.outputReceivedMessage(str + "\n" + qsTr("hex encode: ") + hexEncode)
                }
            }
            type: "TCP"
        }
    }
    Component {
        id: udpServerComponent
        ServerPage {
            id: udpServer
            serverObject: UdpServer {
                onError: function(error) {
                    console.log("UDP Server Error: " + error)
                    udpServer.outputErrorMessage(error)
                }
                onTip: function(tip) {
                    console.log("UDP Server Tip: " + tip)
                    udpServer.outputTipMessage(tip)
                }
                onReceivedData: function(str, hexEncode) {
                    console.log("UDP Server Received: " + str + "\n"
                                + hexEncode)
                    udpServer.outputReceivedMessage(str + "\n" + qsTr("hex encode: ") + hexEncode)
                }
            }
            type: "UDP"
        }
    }
    Component {
        id: webSocketServerComponent
        ServerPage {
            id: webSocketServer
            serverObject: WebSocketServer {
                onError: function(error) {
                    console.log("WebSocket Server Error: " + error)
                    webSocketServer.outputErrorMessage(error)
                }
                onTip: function(tip) {
                    console.log("WebSocket Server Tip: " + tip)
                    webSocketServer.outputTipMessage(tip)
                }
                onReceivedData: function(str, hexEncode) {
                    console.log("WebSocket Server Received: " + str + "\n"
                                + hexEncode)
                    webSocketServer.outputReceivedMessage(str + "\n" + qsTr("hex encode: ") + hexEncode)
                }
            }
            type: "WebSocket"
        }
    }
    Component {
        id: tcpClientComponent
        ClientPage {
            id: tcpClient
            clientObject: TcpClient {
                onError: function(error) {
                    console.log("TCP Client Error: " + error)
                    tcpClient.outputErrorMessage(error)
                }
                onTip: function(tip) {
                    console.log("TCP Client Tip: " + tip)
                    tcpClient.outputTipMessage(tip)
                }
                onReceivedData: function(str, hexEncode) {
                    console.log("TCP Client Received: " + str + "\n"
                                + hexEncode)
                    tcpClient.outputReceivedMessage(str + "\n" + qsTr("hex encode: ") + hexEncode)
                }
            }
            type: "TCP"
        }
    }
    Component {
        id: udpClientComponent
        ClientPage {
            id: udpClient
            clientObject: UdpClient {
                onError: function(error) {
                    console.log("Udp Client Error: " + error)
                    udpClient.outputErrorMessage(error)
                }
                onTip: function(tip) {
                    console.log("Udp Client Tip: " + tip)
                    udpClient.outputTipMessage(tip)
                }
                onReceivedData: function(str, hexEncode) {
                    console.log("UDP Client Received: " + str + "\n"
                                + hexEncode)
                    udpClient.outputReceivedMessage(str + "\n" + qsTr("hex encode: ") + hexEncode)
                }
            }
            type: "UDP"
        }
    }
    Component {
        id: webSocketClientComponent
        ClientPage {
            id: webSocketClient
            clientObject: WebSocketClient {
                onError: function(error) {
                    console.log("WebSocket Client Error: " + error)
                    webSocketClient.outputErrorMessage(error)
                }
                onTip: function(tip) {
                    console.log("WebSocket Client Tip: " + tip)
                    webSocketClient.outputTipMessage(tip)
                }
                onReceivedData: function(str, hexEncode) {
                    console.log("WebSocket Client Received: " + str + "\n"
                                + hexEncode)
                    webSocketClient.outputReceivedMessage(str + "\n" + qsTr("hex encode: ") + hexEncode)
                }
            }
            type: "WebSocket"
        }
    }
}
