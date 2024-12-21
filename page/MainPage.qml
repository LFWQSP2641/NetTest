import QtQuick
import QtQuick.Layouts

Item {
    id: mainPage
    signal clicked(int id)
    GridLayout {
        anchors.fill: parent

        rows: 3
        columns: 2
        rowSpacing: 10
        columnSpacing: 10

        // TCP服务器
        TextAreaButton {
            Layout.row: 0
            Layout.column: 0
            Layout.fillWidth: true
            Layout.fillHeight: true
            title: qsTr("TCP Server")
            function clickFunction() {
                mainPage.clicked(1)
            }
        }

        // UDP服务器
        TextAreaButton {
            Layout.row: 1
            Layout.column: 0
            Layout.fillWidth: true
            Layout.fillHeight: true
            title: qsTr("UDP Server")
            function clickFunction() {
                mainPage.clicked(2)
            }
        }

        // WebSocket服务器
        TextAreaButton {
            Layout.row: 2
            Layout.column: 0
            Layout.fillWidth: true
            Layout.fillHeight: true
            title: qsTr("WebSocket Server")
            function clickFunction() {
                mainPage.clicked(3)
            }
        }

        // TCP客户端
        TextAreaButton {
            Layout.row: 0
            Layout.column: 1
            Layout.fillWidth: true
            Layout.fillHeight: true
            title: qsTr("TCP Client")
            function clickFunction() {
                mainPage.clicked(4)
            }
        }

        // UDP客户端
        TextAreaButton {
            Layout.row: 1
            Layout.column: 1
            Layout.fillWidth: true
            Layout.fillHeight: true
            title: qsTr("UDP Client")
            function clickFunction() {
                mainPage.clicked(5)
            }
        }

        // WebSocket客户端
        TextAreaButton {
            Layout.row: 2
            Layout.column: 1
            Layout.fillWidth: true
            Layout.fillHeight: true
            title: qsTr("WebSocket Client")
            function clickFunction() {
                mainPage.clicked(6)
            }
        }

        // DNS查询
        TextAreaButton {
            Layout.row: 3
            Layout.column: 0
            Layout.columnSpan: 2
            Layout.fillWidth: true
            Layout.fillHeight: true
            title: qsTr("DNS Query")
            function clickFunction() {
                mainPage.clicked(7)
            }
        }
    }
}
