import QtQuick
import QtQuick.Controls

ApplicationWindow {
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
        icon.source: "qrc:/page/icon/angle-left.svg"
        onTriggered: {
            if (stackView.depth > 1)
            {
                stackView.pop()
            }
        }
    }

    header: ToolBar {
        id: headerToolBar
        height: stackView.depth > 1 ? implicitHeight : 0
        Behavior on height {
            NumberAnimation {
                duration: 100
                easing.type: Easing.InOutQuad
            }
        }
        ToolButton {
            id: navigateBackButton
            action: navigateBackAction
            anchors.left: parent.left
            anchors {top: parent.top; left: parent.left; bottom: parent.bottom}
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: MainPage { stackViewObject: stackView }
    }
}
