import QtQuick 2.0

Item {
    width: 80
    height: width

    Rectangle {
        id: backgroundColor
        width: parent.width
        height: parent.height
        color: "#A8A8A8"

        Rectangle {
            id: leftBorder
            width: 3
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
            height: 3
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
            height: 3
            anchors {
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }
            color: "#484848"
        }

        Rectangle {
            id: rightBorder
            width: 3
            height: parent.height
            anchors {
                bottom: parent.bottom
                top: parent.top
                right: parent.right
            }
            color: "#484848"
        }
    }
}
