import QtQuick
import QtQuick3D
import QtQuick.Controls

Window {
    id: mainWindow
    width: 1200
    height: 800
    visible: true
    title: "matrix_3d_snake_game"

    property int gridSize: 5
    property real ledSpacing: 50
    property var ledColors: []
    property real moveSpeed: 15.0
    property real lookSpeed: 0.5
    property vector3d cameraPosition: Qt.vector3d(250, 250, 200)
    property real yaw: 46.5    // Left/right rotation
    property real pitch: -39.0   // Up/down rotation
    property vector2d lastMousePos: Qt.vector2d(width/2, height/2)
    property bool mouseLookEnabled: false

    // Movement states
    property bool moveForward: false
    property bool moveBackward: false
    property bool moveLeft: false
    property bool moveRight: false
    property bool moveUp: false
    property bool moveDown: false

    Component.onCompleted: {
        initializeColors()
        // Set initial mouse position
        mouseArea.mouseX = width/2
        mouseArea.mouseY = height/2
    }

    function initializeColors() {
        var colors = []
        for (var x = 0; x < gridSize; x++) {
            colors[x] = []
            for (var y = 0; y < gridSize; y++) {
                colors[x][y] = []
                for (var z = 0; z < gridSize; z++) {
                    colors[x][y][z] = Qt.rgba(0.8, 0.8, 0.8, 0.3)
                }
            }
        }
        ledColors = colors
    }

    // Game loop for smooth movement
    Timer {
        interval: 16 // ~60fps
        running: true
        repeat: true
        onTriggered: updateCameraPosition()
    }

    function updateCameraPosition() {
        // Calculate forward vector based on yaw and pitch
        var forward = Qt.vector3d(
            -Math.sin(yaw * Math.PI / 180) * Math.cos(pitch * Math.PI / 180),
            Math.sin(pitch * Math.PI / 180),
            -Math.cos(yaw * Math.PI / 180) * Math.cos(pitch * Math.PI / 180)
        ).normalized()

        // Calculate right vector (always horizontal)
        var right = Qt.vector3d(
            Math.cos(yaw * Math.PI / 180),
            0,
            -Math.sin(yaw * Math.PI / 180)
        ).normalized()

        var movement = Qt.vector3d(0, 0, 0)

        if (moveForward) movement = movement.plus(forward.times(moveSpeed))
        if (moveBackward) movement = movement.minus(forward.times(moveSpeed))
        if (moveRight) movement = movement.plus(right.times(moveSpeed))
        if (moveLeft) movement = movement.minus(right.times(moveSpeed))
        if (moveUp) movement.y += moveSpeed
        if (moveDown) movement.y -= moveSpeed

        cameraPosition = cameraPosition.plus(movement)
    }

    // Main focus item for keyboard events
    Item {
        id: keyHandler
        anchors.fill: parent
        focus: true

        Keys.onPressed: (event) => {
            switch(event.key) {
                case Qt.Key_W: moveForward = true; break
                case Qt.Key_S: moveBackward = true; break
                case Qt.Key_A: moveLeft = true; break
                case Qt.Key_D: moveRight = true; break
                case Qt.Key_Space: moveUp = true; break
                case Qt.Key_Shift: moveDown = true; break
                case Qt.Key_Escape:
                    mouseLookEnabled = false
                    cursorShape = Qt.ArrowCursor
                    break
            }
        }

        Keys.onReleased: (event) => {
            switch(event.key) {
                case Qt.Key_W: moveForward = false; break
                case Qt.Key_S: moveBackward = false; break
                case Qt.Key_A: moveLeft = false; break
                case Qt.Key_D: moveRight = false; break
                case Qt.Key_Space: moveUp = false; break
                case Qt.Key_Shift: moveDown = false; break
            }
        }
    }

    // Mouse area for mouse look
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        // cursorShape: mouseLookEnabled ? Qt.BlankCursor : Qt.ArrowCursor
        acceptedButtons: Qt.RightButton

        onPressed: (mouse) => {
            if (mouse.button === Qt.RightButton) {
                mouseLookEnabled = true
                lastMousePos = Qt.vector2d(mouse.x, mouse.y)
                // cursorShape = Qt.BlankCursor
                keyHandler.forceActiveFocus()
            }
        }

        onPositionChanged: (mouse) => {
            if (mouseLookEnabled) {
                var delta = Qt.vector2d(mouse.x - lastMousePos.x, mouse.y - lastMousePos.y)
                yaw -= delta.x * lookSpeed
                pitch = Math.min(89, Math.max(-89, pitch - delta.y * lookSpeed))
                lastMousePos = Qt.vector2d(mouse.x, mouse.y)

                // Keep mouse centered during mouse look
                if (Math.abs(mouse.x - width/2) > 50 || Math.abs(mouse.y - height/2) > 50) {
                    mouseArea.mouseX = width/2
                    mouseArea.mouseY = height/2
                    lastMousePos = Qt.vector2d(width/2, height/2)
                }
            }
        }
    }

    View3D {
        id: view3d
        anchors.fill: parent

        environment: SceneEnvironment {
            clearColor: "#111111"
            backgroundMode: SceneEnvironment.Color
            antialiasingMode: SceneEnvironment.MSAA
            antialiasingQuality: SceneEnvironment.High
        }

        PerspectiveCamera {
            id: camera
            position: cameraPosition
            eulerRotation: Qt.vector3d(pitch, yaw, 0)
        }

        DirectionalLight {
            eulerRotation.x: -30
            eulerRotation.y: -70
            brightness: 1
            ambientColor: Qt.rgba(0.2, 0.2, 0.2, 1.0)
        }

        // LED Cube
        Node {
            Repeater3D {
                model: gridSize // X axis
                delegate: Node {
                    property int xPos: index

                    Repeater3D {
                        model: gridSize // Y axis
                        delegate: Node {
                            property int yPos: index

                            Repeater3D {
                                model: gridSize // Z axis
                                delegate: Model {
                                    property int zPos: index

                                    position: Qt.vector3d(
                                        (xPos - (gridSize-1)/2) * ledSpacing,
                                        (yPos - (gridSize-1)/2) * ledSpacing,
                                        (zPos - (gridSize-1)/2) * ledSpacing
                                    )
                                    source: "#Sphere"
                                    scale: Qt.vector3d(0.4, 0.4, 0.4)

                                    materials: PrincipledMaterial {
                                        baseColor: ledColors[xPos] && ledColors[xPos][yPos] && ledColors[xPos][yPos][zPos]
                                                ? ledColors[xPos][yPos][zPos]
                                                : Qt.rgba(1, 1, 1, 0.3)
                                        opacity: ledColors[xPos][yPos][zPos].a
                                        alphaMode: PrincipledMaterial.Blend
                                        emissiveFactor: 1.0
                                        roughness: 0.0
                                        metalness: 0.0
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

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

            Label {
                text: "Camera Controls:"
                font.bold: true
                color: "white"
            }

            Label {
                text: "WASD: Move\nSpace: Move Up\nShift: Move Down\nRight Click + Drag: Look\nESC: Release Mouse"
                color: "white"
            }

            // Camera position and rotation info
            Label {
                text: "Camera Position:"
                font.bold: true
                color: "white"
            }
            Label {
                text: "X: " + cameraPosition.x.toFixed(1) +
                      "\nY: " + cameraPosition.y.toFixed(1) +
                      "\nZ: " + cameraPosition.z.toFixed(1)
                color: "white"
            }
            Label {
                text: "Pitch: " + pitch.toFixed(1) + "°\nYaw: " + yaw.toFixed(1) + "°"
                color: "white"
            }
            Button {
                text: "Reset Camera"
                width: parent.width
                onClicked: {
                    cameraPosition = Qt.vector3d(250, 250, 200)
                    yaw = 46.5
                    pitch = -39.0
                }
            }
        }
    }

    function randomizeColors() {
        var colors = []
        for (var x = 0; x < gridSize; x++) {
            colors[x] = []
            for (var y = 0; y < gridSize; y++) {
                colors[x][y] = []
                for (var z = 0; z < gridSize; z++) {
                    colors[x][y][z] = Qt.rgba(
                        Math.random(),
                        Math.random(),
                        Math.random(),
                        Math.random() * 0.5 + 0.3 // Keep some transparency
                    )
                }
            }
        }
        ledColors = colors
    }
}
