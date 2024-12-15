import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: serverPage

    required property var serverObject
    property string type: ""

    ColumnLayout {
        anchors.fill: parent
        Label {
            text: serverPage.type + qsTr(" Server")
            font.bold: true
        }
        // 端口
        TextField {
            id: portTextField
            Layout.fillWidth: true
            placeholderText: qsTr("Port")
            // 限制输入为数字
            validator: IntValidator {}
        }
        Button {
            id: startServerButton
            Layout.fillWidth: true
            text: qsTr("Start Server")
            onClicked: {
                if (serverPage.serverObject !== null)
                {
                    serverPage.serverObject.startServer(parseInt(portTextField.text))
                }
                else
                {
                    console.log("Server object is null")
                }
            }
        }
        Button {
            id: stopServerButton
            Layout.fillWidth: true
            text: qsTr("Stop Server")
            onClicked: {
                if (serverPage.serverObject !== null)
                {
                    serverPage.serverObject.stopServer()
                }
                else
                {
                    console.log("Server object is null")
                }
            }
        }

        Flickable {
            id: flickable
            Layout.fillHeight: true
            Layout.fillWidth: true
            clip: true
            contentHeight: serverTextArea.height
            TextArea {
                id: serverTextArea
                width: parent.width
                placeholderText: "received message and debug log"
                readOnly: true
                wrapMode: TextEdit.Wrap

                function log(text) {
                    serverTextArea.output(Qt.formatDateTime(new Date(), "hh:mm:ss.zzz") + " " + text)
                }

                function output(text) {
                    serverTextArea.append(text)
                    let contentY = flickable.contentHeight - flickable.height
                    flickable.contentY = contentY > 0 ? contentY : 0
                }
            }
            // onContentHeightChanged: {
            //     console.log("contentHeight:" + contentHeight)
            //     Qt.callLater(() => contentY = contentHeight - height)
            // }
            // onContentYChanged: {
            //     console.log("contentY: " + contentY)
            //     Qt.callLater(() => contentY = contentHeight - height)
            // }
        }
    }


    function outputErrorMessage(message)
    {
        serverTextArea.log(qsTr("error:") + message)
    }
    function outputTipMessage(message)
    {
        serverTextArea.log(qsTr("Tip:") + message)
    }
    function outputReceivedMessage(message)
    {
        serverTextArea.log(qsTr("Received:") + message)
    }

    Connections {
        target: serverPage.serverObject
        function onError(error) {
            console.log(serverPage.type + " Server Error: " + error)
            serverPage.outputErrorMessage(error)
        }
        function onTip(tip) {
            console.log(serverPage.type + " Server Tip: " + tip)
            serverPage.outputTipMessage(tip)
        }
        function onReceivedData(str, hexEncode) {
            let message = str + "\n" + qsTr("hex encode: ") + hexEncode
            console.log(serverPage.type + " Server Received: " + message)
            serverPage.outputReceivedMessage(message)
        }
    }
}
