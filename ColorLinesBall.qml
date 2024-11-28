import QtQuick

Item {
    property alias color: ball.color
    property bool selected: false

    id: rootBall
    width: 80
    height: width

    Rectangle {
        id: ball
        width: parent.width
        height: parent.height
        radius: width / 2

        states: [
            State {
                name: "visible"
                PropertyChanges {
                    target: rootBall

                }
            },

            State {
                name: "notVisible"
                PropertyChanges {
                    target: rootBall

                }
            }
        ]

        SequentialAnimation {
            id: selectAnimation
            alwaysRunToEnd: true
            loops: Animation.Infinite

            PropertyAnimation {
                target: rootBall; property: "height"
                duration: 400
                from: rootBall.height
                to: rootBall.height * 0.9
            }

            PropertyAnimation {
                target: rootBall; property: "height"
                duration: 200
                from: rootBall.height * 0.9
                to: rootBall.height
            }
        }

        ScaleAnimator {
            id: dying
            alwaysRunToEnd: true
            target: rootBall
            duration: 500
            from: 1
            to: 0
        }

        ScaleAnimator {
            id: spawn
            alwaysRunToEnd: true
            target: rootBall
            duration: 500
            from: 0
            to: 1
        }
    }

    onStateChanged: {
        if (state == "visible") {
            spawn.start()
            visible = true
        } else {
            selectAnimation.stop()
            dying.start()
            if (dying.complete()) {
                visible = false
            }

        }
    }

    onSelectedChanged: {
        if (selected) {
            selectAnimation.start()
        } else {
            selectAnimation.stop()
        }
    }
}


