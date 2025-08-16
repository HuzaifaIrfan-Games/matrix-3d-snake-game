  import QtQuick
  import QtQuick.Controls
  // Control panel
    Rectangle {
        width: 300
        height: parent.height
        color: "#80000000"
        radius: 10
        anchors.right: parent.right
        // anchors.bottom: parent.bottom
        anchors.margins: 20

        Column {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 10


            Button {
                text: "Random Colors"
                width: parent.width
                onClicked: randomizeColors()
            }

            Button {
                text: "Turn On"
                width: parent.width
                onClicked: turnOnLights()
            }


            Button {
                text: "Turn Off"
                width: parent.width
                onClicked: turnOffLights()
            }


            Label {
                text: "Camera Controls:"
                font.bold: true
                color: "white"
            }

            Label {
                text: "WASD: Move\nLeft Click + Drag: Look\n"
                color: "white"
            }

            // Camera position and rotation info
            Label {
                text: "Camera Position:"
                font.bold: true
                color: "white"
            }
            Label {
                text: "X: " + camera.x.toFixed(1) +
                      " Y: " + camera.y.toFixed(1) +
                      " Z: " + camera.z.toFixed(1)
                color: "white"
            }
            Label {
                text: "Pitch: " + camera.eulerRotation.x.toFixed(1) + "° Yaw: " + camera.eulerRotation.y.toFixed(1) + "°"
                color: "white"
            }
            Button {
                text: "Reset Camera"
                width: parent.width
                onClicked: {
                    resetCameraDefaults()
                }
            }
        }


    function randomizeColors() {
        var colors = []
        for (var x = 0; x < ledMatrixCube.gridSize; x++) {
            colors[x] = []
            for (var y = 0; y < ledMatrixCube.gridSize; y++) {
                colors[x][y] = []
                for (var z = 0; z < ledMatrixCube.gridSize; z++) {
                    colors[x][y][z] = Qt.rgba(
                        Math.random(),
                        Math.random(),
                        Math.random(),
                        0.5 // Keep some transparency
                    )
                }
            }
        }
        ledMatrixCube.ledColors = colors
    }


    function turnOnLights() {
        var colors = []
        for (var x = 0; x < ledMatrixCube.gridSize; x++) {
            colors[x] = []
            for (var y = 0; y < ledMatrixCube.gridSize; y++) {
                colors[x][y] = []
                for (var z = 0; z < ledMatrixCube.gridSize; z++) {
                    colors[x][y][z] = Qt.rgba(
                        1, 1, 1, 1 // Keep some transparency
                    )
                }
            }
        }
        ledMatrixCube.ledColors = colors
    }


    function turnOffLights() {
        var colors = []
        for (var x = 0; x < ledMatrixCube.gridSize; x++) {
            colors[x] = []
            for (var y = 0; y < ledMatrixCube.gridSize; y++) {
                colors[x][y] = []
                for (var z = 0; z < ledMatrixCube.gridSize; z++) {
                    colors[x][y][z] = Qt.rgba(
                        0.3,0.3,0.3,0.5 // Keep some transparency
                    )
                }
            }
        }
        ledMatrixCube.ledColors = colors
    }
    }


    