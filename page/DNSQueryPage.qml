import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: dnsQueryPage
    required property var dnsQueryObject
    property string type: "DNS"

    ColumnLayout {
        anchors.fill: parent
        Label {
            text: qsTr("DNS Query")
            font.bold: true
        }
        // 域名
        TextField {
            id: domainTextField
            Layout.fillWidth: true
            placeholderText: qsTr("Domain")
        }
        // dns服务器
        RowLayout {
            Layout.fillWidth: true
            TextField {
                id: dnsServerTextField
                Layout.fillWidth: true
                placeholderText: qsTr("DNS Server")
            }
            // 端口
            TextField {
                id: portTextField
                Layout.fillWidth: true
                placeholderText: qsTr("Port")
                text: "53"
                // 限制输入为数字
                validator: IntValidator {}
            }
        }
        // 查询按钮
        Button {
            id: queryButton
            Layout.fillWidth: true
            text: qsTr("Query")
            onClicked: {
                if (dnsQueryPage.dnsQueryObject !== null)
                {
                    dnsQueryPage.dnsQueryObject.query(domainTextField.text, dnsServerTextField.text, parseInt(portTextField.text))
                }
                else
                {
                    console.log("DNS Query object is null")
                }
            }
        }
        // 结果
        Flickable {
            id: flickable
            Layout.fillHeight: true
            Layout.fillWidth: true
            clip: true
            contentHeight: resultTextArea.height
            TextArea {
                id: resultTextArea
                width: parent.width
                placeholderText: "received message and debug log"
                readOnly: true
                wrapMode: TextEdit.Wrap

                function log(text) {
                    resultTextArea.output(Qt.formatDateTime(new Date(), "hh:mm:ss.zzz") + " " + text)
                }

                function output(text) {
                    resultTextArea.append(text)
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
        resultTextArea.log(qsTr("error:") + message)
    }
    function outputTipMessage(message)
    {
        resultTextArea.log(qsTr("Tip:") + message)
    }
    function outputReceivedMessage(message)
    {
        resultTextArea.log(qsTr("Received:") + message)
    }

    Connections {
        target: dnsQueryPage.dnsQueryObject
        function onError(error) {
            console.log(dnsQueryPage.type + " Server Error: " + error)
            dnsQueryPage.outputErrorMessage(error)
        }
        function onTip(tip) {
            console.log(dnsQueryPage.type + " Server Tip: " + tip)
            dnsQueryPage.outputTipMessage(tip)
        }
        // function onReceivedData(str, hexEncode) {
        //     let message = str + "\n" + qsTr("hex encode: ") + hexEncode
        //     console.log(dnsQueryPage.type + " Server Received: " + message)
        //     dnsQueryPage.outputReceivedMessage(message)
        // }
        function onLookupFinished(domain, ip, timeMs) {
            let message = qsTr("Lookup domain: ") + domain + "\n" + qsTr("IP: ") + ip + "\n" + qsTr("Time: ") + timeMs + "ms"
            console.log(dnsQueryPage.type + " Server Lookup Finished: " + message)
            resultTextArea.output(message)
        }
    }
}
