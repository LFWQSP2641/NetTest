import QtQuick

Rectangle {
    id: root
    // 圆角
    radius: 10

    // 标题
    property string title: "TextAreaButton"
    // 描述（可选）
    property string description: ""

    Text {
        id: titleText
        anchors {
            top: root.top
            left: root.left
            right: root.right
            margins: 10
        }
        text: root.title
        // 字体大小
        // font.pixelSize: root.height / 5
        font.bold: true
        wrapMode: Text.Wrap
    }
    Text {
        id: descriptionText
        anchors {
            top: titleText.bottom
            left: root.left
            right: root.right
            margins: 10
        }
        text: root.description
        // 字体大小
        // font.pixelSize: root.height / 9
        wrapMode: Text.Wrap
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            console.log("Clicked " + root.title)
            root.clickFunction()
        }
    }

    function clickFunction() {
        console.log("Click function not implemented")
    }
}
