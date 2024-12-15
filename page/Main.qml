pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import NetTest

ApplicationWindow {
    id: window
    width: 640
    height: 480
    visible: true
    title: qsTr("Server and Client Interface")

    required property var builtInStyles

    Shortcut {
        sequences: ["Esc", "Back"]
        enabled: stackView.depth > 1
        onActivated: navigateBackAction.trigger()
    }

    Action {
        id: navigateBackAction
        text: stackView.depth > 1 ? qsTr("Back") : qsTr("Menu")
        icon.source: stackView.depth > 1 ? "qrc:/page/icon/angle-left.svg" : "qrc:/page/icon/menu-burger.svg"
        onTriggered: {
            if (stackView.depth > 1) {
                stackView.pop()
                listView.currentIndex = -1
            } else {
                drawer.open()
            }
        }
    }

    Shortcut {
        sequence: "Menu"
        onActivated: optionsMenuAction.trigger()
    }

    Action {
        id: optionsMenuAction
        icon.source: "qrc:/page/icon/menu-dots-vertical.svg"
        onTriggered: optionsMenu.open()
    }

    header: ToolBar {
        ToolButton {
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.verticalCenter: parent.verticalCenter
            action: navigateBackAction
        }

        Label {
            id: titleLabel
            anchors.centerIn: parent
            text: qsTr("Server and Client Interface")
            font.pixelSize: 20
            elide: Label.ElideRight
        }

        ToolButton {
            action: optionsMenuAction
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.verticalCenter: parent.verticalCenter
            Menu {
                id: optionsMenu
                x: parent.width - width
                transformOrigin: Menu.TopRight

                Action {
                    text: qsTr("Settings")
                    onTriggered: settingsDialog.open()
                }
                Action {
                    text: qsTr("About")
                    onTriggered: aboutDialog.open()
                }
            }
        }
    }

    Dialog {
        id: settingsDialog
        x: Math.round((window.width - width) / 2)
        y: Math.round(window.height / 6)
        width: Math.min(window.width, window.height) / 3 * 2
        modal: true
        focus: true
        title: qsTr("Settings")
        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: {
            Settings.style = styleComboBox.currentText
            Settings.font = fontComboBox.currentText
            Settings.saveSettings()
            settingsDialog.close()
        }
        onRejected: {
            styleComboBox.currentIndex = styleComboBox.styleIndex
            fontComboBox.currentIndex = fontComboBox.fontIndex
            settingsDialog.close()
        }
        contentItem: ColumnLayout {
            id: settingsColumn
            spacing: 20
            GroupBox {
                Layout.fillWidth: true
                title: qsTr("Appearance(required restart)")
                ColumnLayout {
                    width: parent.width
                    height: parent.height
                    RowLayout {
                        Label {
                            text: qsTr("Theme")
                        }
                        ComboBox {
                            id: styleComboBox
                            Layout.fillWidth: true
                            property int styleIndex: -1
                            model: window.builtInStyles
                            Component.onCompleted: {
                                styleIndex = find(Settings.style, Qt.MatchFixedString)
                                if (styleIndex !== -1) {
                                    currentIndex = styleIndex
                                }
                            }
                        }
                    }
                    RowLayout {
                        Label {
                            text: qsTr("Font")
                        }
                        ComboBox {
                            id: fontComboBox
                            Layout.fillWidth: true
                            font: fontComboBox.currentText.length == 0 ? undefined : ({ family: fontComboBox.currentText })
                            property int fontIndex: -1
                            model: Qt.fontFamilies()
                            delegate: ItemDelegate {
                                required property var modelData
                                text: modelData
                                font: modelData
                                width: fontComboBox.width
                                onClicked: {
                                    fontComboBox.popup.close()
                                }
                            }

                            Component.onCompleted: {
                                fontIndex = find(Settings.font, Qt.MatchFixedString)
                                if (fontIndex !== -1) {
                                    currentIndex = fontIndex
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Dialog {
        id: aboutDialog
        modal: true
        focus: true
        title: qsTr("About")
        x: (window.width - width) / 2
        y: window.height / 6
        width: Math.min(window.width, window.height) / 3 * 2
        contentHeight: aboutColumn.height

        Column {
            id: aboutColumn
            spacing: 20

            Label {
                width: aboutDialog.availableWidth
                text: qsTr("NetTest is a network testing tool that provides a simple "
                    + "interface for testing various network protocols.")
                wrapMode: Label.Wrap
                font.pixelSize: 12
            }

            Label {
                width: aboutDialog.availableWidth
                text: "<p>" + qsTr("Project Address:") + "<a href=\"https://github.com/LFWQSP2641/NetTest\">https://github.com/LFWQSP2641/NetTest</a></p>"
                wrapMode: Label.Wrap
                font.pixelSize: 12
            }

            Label {
                width: aboutDialog.availableWidth
                text: "Copyright © 2024 - 2024 LFWQSP2641.All Rights Reserved."
                wrapMode: Label.Wrap
                font.pixelSize: 12
            }
        }
    }

    Drawer {
        id: drawer

        width: listView.maxWidth + 4
        height: window.height
        modal: true
        interactive: stackView.depth === 1

        ListView {
            id: listView

            property double maxWidth: 0

            focus: true
            currentIndex: -1
            anchors.fill: parent

            model: ListModel {
                ListElement { title: qsTr("TCP Server"); idn: 1 }
                ListElement { title: qsTr("TCP Client"); idn: 4 }
                ListElement { title: qsTr("UDP Server"); idn: 2 }
                ListElement { title: qsTr("UDP Client"); idn: 5 }
                ListElement { title: qsTr("WebSocket Server"); idn: 3 }
                ListElement { title: qsTr("WebSocket Client"); idn: 6 }
                ListElement { title: qsTr("DNS Query"); idn: 7 }
            }

            delegate: ItemDelegate {
                id: delegateItem
                text: title
                highlighted: ListView.isCurrentItem

                required property int index
                required property var model
                required property string title
                required property int idn

                onClicked: {
                    parent.currentIndex = index
                    drawer.close()
                    window.stackViewPush(idn)
                }

                Component.onCompleted: {
                    listView.maxWidth = Math.max(delegateItem.width, listView.maxWidth)
                }
            }

            ScrollIndicator.vertical: ScrollIndicator { }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        StackView {
            id: stackView
            Layout.fillWidth: true
            Layout.fillHeight: true
            initialItem: MainPage {
                id: initialPage
                onClicked: function(id) {
                    window.stackViewPush(id)
                }
            }
        }
    }

    function stackViewPush(id) {
        switch(id)
        {
        case 1:
            stackView.push(tcpServerComponent)
            break
        case 2:
            stackView.push(udpServerComponent)
            break
        case 3:
            stackView.push(webSocketServerComponent)
            break
        case 4:
            stackView.push(tcpClientComponent)
            break
        case 5:
            stackView.push(udpClientComponent)
            break
        case 6:
            stackView.push(webSocketClientComponent)
            break
        case 7:
            stackView.push(dnsQueryComponent)
            break
        default:
            console.log("Unknown id")
            break
        }
    }

    Component {
        id: tcpServerComponent
        ServerPage {
            id: tcpServer
            serverObject: TcpServer { }
            type: "TCP"
        }
    }
    Component {
        id: udpServerComponent
        ServerPage {
            id: udpServer
            serverObject: UdpServer { }
            type: "UDP"
        }
    }
    Component {
        id: webSocketServerComponent
        ServerPage {
            id: webSocketServer
            serverObject: WebSocketServer { }
            type: "WebSocket"
        }
    }
    Component {
        id: tcpClientComponent
        ClientPage {
            id: tcpClient
            clientObject: TcpClient { }
            type: "TCP"
        }
    }
    Component {
        id: udpClientComponent
        ClientPage {
            id: udpClient
            clientObject: UdpClient { }
            type: "UDP"
        }
    }
    Component {
        id: webSocketClientComponent
        ClientPage {
            id: webSocketClient
            clientObject: WebSocketClient { }
            type: "WebSocket"
        }
    }
    Component {
        id: dnsQueryComponent
        DNSQueryPage {
            id: dnsQuery
            dnsQueryObject: DnsQuery { }
        }
    }
}
