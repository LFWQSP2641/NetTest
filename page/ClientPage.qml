import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: clientPage

    property var clientObject: null
    property string type: ""

    ColumnLayout {
        anchors.fill: parent

        Label {
            text: clientPage.type + qsTr(" Client")
            font.bold: true
        }

        // IP
        TextField {
            id: ipTextField
            Layout.fillWidth: true
            placeholderText: qsTr("IP")
        }
        // 端口
        TextField {
            id: portTextField
            Layout.fillWidth: true
            placeholderText: qsTr("Port")
            // 限制输入为数字
            validator: IntValidator {}
        }
        // 发送消息
        Flickable {
            Layout.fillHeight: true
            Layout.fillWidth: true
            clip: true
            contentHeight: messageTextField.height
            TextArea {
                id: messageTextField
                width: parent.width
                placeholderText: "send message"
                wrapMode: TextEdit.Wrap
            }
        }

        // 连接服务器按钮
        Button {
            id: connectServerButton
            Layout.fillWidth: true
            text: qsTr("Connect Server")
            onClicked: {
                if (clientPage.clientObject !== null)
                {
                    clientPage.clientObject.connectToServer(ipTextField.text, parseInt(portTextField.text))
                }
                else
                {
                    console.log("Client object is null")
                }
            }
        }

        // 发送按钮
        Button {
            id: sendDataButton
            Layout.fillWidth: true
            text: qsTr("Send Message")
            onClicked: {
                if (clientPage.clientObject !== null)
                {
                    clientPage.clientObject.sendData(messageTextField.text)
                }
                else
                {
                    console.log("Client object is null")
                }
            }
        }

        // 调试信息 & 接收消息
        Flickable {
            id: flickable
            Layout.fillHeight: true
            Layout.fillWidth: true
            clip: true
            contentHeight: clientTextArea.height
            TextArea {
                id: clientTextArea
                width: parent.width
                placeholderText: "received message and debug log"
                readOnly: true
                wrapMode: TextEdit.Wrap

                function log(text) {
                    clientTextArea.output(Qt.formatDateTime(new Date(), "hh:mm:ss.zzz") + " " + text)
                }

                function output(text) {
                    clientTextArea.append(text)
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
        clientTextArea.log(qsTr("error:") + message)
    }
    function outputTipMessage(message)
    {
        clientTextArea.log(qsTr("Tip:") + message)
    }
    function outputReceivedMessage(message)
    {
        clientTextArea.log(qsTr("Received:") + message)
    }
}
