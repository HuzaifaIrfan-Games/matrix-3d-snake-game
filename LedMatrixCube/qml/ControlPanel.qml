import QtQuick
import QtQuick.Controls

// Control panel
Rectangle {
    id: controlPanel
    width: 300
    height: parent.height
    color: "#80000000"
    radius: 10
    anchors.right: parent.right
    // anchors.bottom: parent.bottom
    anchors.margins: 20

    property int game_tick_ms: 1000

    property var game_info: "Game Info"
    property var game_score: 0
    property var snake_length: 0

    Column {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        Label {
            text: "Developed by Huzaifa Irfan"
            color: "white"
            font.pixelSize: 14
            font.italic: true
            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: Qt.openUrlExternally("https://huzaifairfan.com")
            }
        }

        Button {
            text: "Random Colors"
            width: parent.width
            onClicked: controlPanel.randomizeColors()
        }
        Row {
            width: parent.width

            Button {
                text: "Turn On"
                onClicked: controlPanel.turnOnLights()
            }

            Button {
                text: "Turn Off"
                onClicked: controlPanel.turnOffLights()
            }
        }
        Label {
            text: "Camera:"
            font.bold: true
            color: "white"
        }

        Label {
            text: "WASD RF: Move\nLeft Click + Drag: Look"
            color: "white"
        }

        // Camera position and rotation info
        Label {
            text: "X: " + camera.x.toFixed(1) + " Y: " + camera.y.toFixed(1) + " Z: " + camera.z.toFixed(1)
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
                mainWindow.resetCameraDefaults();
            }
        }

        Label {
            text: "Game:"
            font.bold: true
            color: "white"
        }

        Label {
            text: "Enter/Period: Start/Stop"
            color: "white"
        }

        Label {
            text: "5123 74 Snake Move"
            color: "white"
        }

        Label {
            text: "Info: " + controlPanel.game_info
            color: "white"
        }
        Label {
            text: "Score: " + controlPanel.game_score
            color: "white"
        }
        Label {
            text: "Snake Length: " + controlPanel.snake_length
            color: "white"
        }
        

        Label {
            text: "Game Tick Delay (ms):"
            color: "white"
        }
        SpinBox {
            from: 100
            to: 5000
            stepSize: 100
            value: controlPanel.game_tick_ms
            onValueChanged: controlPanel.game_tick_ms = value
            width: parent.width
        }


    }

    function randomizeColors() {
        var colors3drgb = [];
        for (var x = 0; x < ledMatrixCube.matrixSize; x++) {
            colors3drgb[x] = [];
            for (var y = 0; y < ledMatrixCube.matrixSize; y++) {
                colors3drgb[x][y] = [];
                for (var z = 0; z < ledMatrixCube.matrixSize; z++) {
                    // Generate random colors for each LED
                    colors3drgb[x][y][z] = [Math.random(), Math.random(), Math.random()];
                }
            }
        }
        ledMatrixCube.setLedsColors(colors3drgb);
    }

    function turnOnLights() {
        var colors3drgb = [];
        for (var x = 0; x < ledMatrixCube.matrixSize; x++) {
            colors3drgb[x] = [];
            for (var y = 0; y < ledMatrixCube.matrixSize; y++) {
                colors3drgb[x][y] = [];
                for (var z = 0; z < ledMatrixCube.matrixSize; z++) {
                    colors3drgb[x][y][z] = [1, 1, 1];
                }
            }
        }
        ledMatrixCube.setLedsColors(colors3drgb);
    }

    function turnOffLights() {
        var colors3drgb = [];
        for (var x = 0; x < ledMatrixCube.matrixSize; x++) {
            colors3drgb[x] = [];
            for (var y = 0; y < ledMatrixCube.matrixSize; y++) {
                colors3drgb[x][y] = [];
                for (var z = 0; z < ledMatrixCube.matrixSize; z++) {
                    colors3drgb[x][y][z] = [0.0, 0.0, 0.0];
                }
            }
        }
        ledMatrixCube.setLedsColors(colors3drgb);
    }

    function setGameInfo(tmp_game_info, tmp_game_score, tmp_snake_length) {
        controlPanel.game_info = tmp_game_info;
        controlPanel.game_score = tmp_game_score;
        controlPanel.snake_length = tmp_snake_length;
    }
}
