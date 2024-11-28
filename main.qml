import QtQuick 2.15
import QtQuick.Window 2.15
import ColorLinesModel
import QtQuick.Controls

Window {
    id: window
    width: 1280
    height: 720
    visible: true
    title: qsTr("Hello World")

    onClosing: (close) => {
        clm.save()
    }

    QtObject {
        id: struct

        property bool select: false
        property real xStartMove: 0
        property real yStartMove: 0
    }

    Rectangle {
        id: backgroundColor
        width: parent.width
        height: parent.height
        color: "black"
    }

    TableView {
        id: board
        anchors.centerIn: parent
        height: parent.height
        width: height
        rowSpacing: 3
        columnSpacing: rowSpacing

        model: ColorLinesModel {
            id: clm
            onScoreChanged: score.text = "Счет: " + field.model.score;
            onGameOverChanged: {
                field.enabled = false
                gameOver.visible = true
            }
        }

        delegate: ColorLinesCell {
            id: cell
            implicitHeight: parent.height * 0.1
            implicitWidth: implicitHeight
        }
    }

    TableView {
        id: field
        anchors.centerIn: parent
        height: parent.height
        width: height
        rowSpacing: 3
        columnSpacing: rowSpacing

        model: clm

        delegate: MouseArea {
            id: rootBall
            implicitHeight: parent.height * 0.1
            implicitWidth: implicitHeight

            onClicked: {
                if (struct.select) {
                    if (clm.ballMove(row, column)) {
                        moveXanimation.from = struct.xStartMove;
                        moveXanimation.to   = rootBall.x;
                        moveYanimation.from = struct.yStartMove;
                        moveYanimation.to   = rootBall.y;

                        moveAnimation.start();

                        struct.select = false;
                    } else {
                        struct.select = clm.selectBall(row, column)
                        ball.selected = model.selected
                    }
                } else {
                    struct.select = clm.selectBall(row, column);
                    ball.selected = model.selected
                    if (struct.select) {
                        struct.xStartMove  = rootBall.x;
                        struct.yStartMove = rootBall.y;
                    }
                }
            }

            ColorLinesBall {
                id: ball
                height: parent.implicitHeight * 0.8
                width: height
                anchors.centerIn: parent

                color: model.color
                state: model.visible

                ParallelAnimation {
                    id: moveAnimation

                    NumberAnimation {
                        id: moveXanimation
                        target: rootBall; property: "x"
                        duration: 1000
                    }

                    NumberAnimation {
                        id: moveYanimation
                        target: rootBall; property: "y"
                        duration: 1000
                    }

                    onStopped: clm.update()
                }
            }
        }
    }

    ColorLinesScore {
        id: score
        width: parent.width * 0.12
        height: parent.height * 0.1
        text: "Счет: 0"
        anchors {
            top: parent.top
            right: parent.right
            topMargin: height * 0.25
            rightMargin: width * 0.25
        }

        onTextChanged: {
            textAnimation.start()
        }

        SequentialAnimation {
            id: textAnimation

            ScaleAnimator {
                target: score
                from: score.textScale
                to: score.textScale * 1.5
                duration: 250
            }

            ScaleAnimator {
                target: score
                from: score.textScale * 1.5
                to: score.textScale
                duration: 250
            }
        }
    }

    Button {
        id: newGame
        width: parent.width * 0.12
        height: parent.height * 0.1
        anchors {
            top: parent.top
            topMargin: height * 0.25
            left: parent.left
            leftMargin: width * 0.25
        }
        text: "Новая игра"
        font.pixelSize: 22
        onClicked: {
            clm.reset()
        }
        background: Rectangle {
            color:  "#A8A8A8"
        }
    }

    Rectangle {
        id: gameOver
        width: board.width
        height: board.height
        anchors.centerIn: parent
        color: "black"
        opacity: 0.5
        visible: false
        Label {
            text: "Игра окончена"
            font.pointSize: 100
            style: Text.Outline
            styleColor: "black"
            anchors.centerIn: parent
            color: "white"
            font.bold: true
        }
    }
}
