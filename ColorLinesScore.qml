import QtQuick 2.0
import QtQuick.Controls

Item {
    width: 200
    height: 100
    property alias text: labelScore.text
    property alias textScale: labelScore.scale

    Rectangle {
        width: parent.width
        height: parent.height
        border.width: width * 0.05
        border.color: "#A8A8A8"
        color: "black"
    }

    Rectangle {
        id: leftBorder
        width: parent.width * 0.025
        height: parent.height
        anchors {
            top: parent.top
            left: parent.left
            bottom: parent.bottom
        }
        color: "#D8D8D8"
    }

    Rectangle {
        id: topBorder
        width: parent.width
        height: parent.height * 0.05
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        color: "#D8D8D8"
    }

    Rectangle {
        id: bottomBorder
        width: parent.width
        height: parent.height * 0.05
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }
        color: "#484848"
    }

    Rectangle {
        id: rightBorder
        width: parent.width * 0.025
        height: parent.height
        anchors {
            bottom: parent.bottom
            top: parent.top
            right: parent.right
        }
        color: "#484848"
    }

    Label {
        id: labelScore

        color: "#006400"
        font.pixelSize: 22
        anchors {
            right: parent.right
            verticalCenter: parent.verticalCenter
            margins: 10
        }
    }
}
