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
    property real ledSpacing: 100
    property real moveSpeed: 15.0
    property real lookSpeed: 0.5
    property vector3d cameraPosition: Qt.vector3d(700, 500, 300)
    property real yaw: 55.0    // Left/right rotation
    property real pitch: -30.0   // Up/down rotation
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
        // Set initial mouse position
        // mouseArea.mouseX = mainWindow.width/2
        // mouseArea.mouseY = mainWindow.height/2
    }

    // 3D color array initialization
    property var ledColors: initialize3DColorArray()

    function initialize3DColorArray() {
        var arr = []
        for (var x = 0; x < gridSize; x++) {
            arr[x] = []
            for (var y = 0; y < gridSize; y++) {
                arr[x][y] = []
                for (var z = 0; z < gridSize; z++) {
                    arr[x][y][z] = Qt.rgba(0.2, 0.2, 0.2, 0.3) // Default color
                }
            }
        }
        return arr
    }

    // Slot to receive 3D color array from C++
    function setColors(colors3d) {
        var colors = []
        var idx = 0
        for (var x = 0; x < gridSize; x++) {
            colors[x] = []
            for (var y = 0; y < gridSize; y++) {
                colors[x][y] = []
                for (var z = 0; z < gridSize; z++) {
                    // colors3d is a flat array of 500 floats: 125 RGBA tuples
                    var r = colors3d[idx++]
                    var g = colors3d[idx++]
                    var b = colors3d[idx++]
                    var a = colors3d[idx++]
                    colors[x][y][z] = Qt.rgba(r, g, b, a)
                }
            }
        }
        ledColors = colors
    }

    // ... [keep all your existing View3D, Repeater3D, and other code] ...

    // Expose API to C++
    property var ledCubeApi: QtObject {
        // For C++ to call
        function setColors(colors3d) {
            // console.log(colors3d)
            mainWindow.setColors(colors3d)
        }

        // For C++ to read
        function getColor(x, y, z) {
            if (x >= 0 && x < gridSize && 
                y >= 0 && y < gridSize && 
                z >= 0 && z < gridSize) {
                return ledColors[x][y][z]
            }
            return Qt.rgba(0, 0, 0, 1)
        }
    }

    // Game loop for smooth movement
    Timer {
        interval: 16 // ~60fps
        running: true
        repeat: true
        onTriggered: mainWindow.updateCameraPosition()
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

        // Add a PointLight instead of another DirectionalLight or SpotLight
        PointLight {
            position: Qt.vector3d(800, 800, 800)
            color: Qt.rgba(0.8, 0.9, 1.0, 1.0)
            brightness: 2.0
            constantFade: 1.0
            linearFade: 0.0
            quadraticFade: 0.01
            castsShadow: false
        }

        // XYZ Axis visualization
        Node {
            // X Axis (Red)
            Model {
                source: "#Cube"
                scale: Qt.vector3d( 1,0.05, 0.05)
                position: Qt.vector3d(gridSize * ledSpacing / 2 + 150, 0, 0)
                rotation: Qt.vector3d(90, 90, 90)
                materials: PrincipledMaterial {
                    baseColor: "red"
                    emissiveFactor: 1.0
                    roughness: 0.2
                    metalness: 0.0
                }
            }
            // Y Axis (Green)
            Model {
                source: "#Cube"
                scale: Qt.vector3d( 0.05,1, 0.05)
                position: Qt.vector3d(0, gridSize * ledSpacing / 2 + 150, 0)
                rotation: Qt.vector3d(0, 0, 0)
                materials: PrincipledMaterial {
                    baseColor: "green"
                    emissiveFactor: 1.0
                    roughness: 0.2
                    metalness: 0.0
                }
            }
            // Z Axis (Blue)
            Model {
                source: "#Cube"
                scale: Qt.vector3d(0.05, 0.05,1)
                position: Qt.vector3d(0, 0, gridSize * ledSpacing / 2 + 150)
                rotation: Qt.vector3d(0, 90, 0)
                materials: PrincipledMaterial {
                    baseColor: "blue"
                    emissiveFactor: 1.0
                    roughness: 0.2
                    metalness: 0.0
                }
            }
        }

        // LED Cube
        // LED Cube with Point Lights
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
                                delegate: Node {
                                    property int zPos: index
                                    property vector3d ledPosition: Qt.vector3d(
                                        (xPos - (gridSize-1)/2) * ledSpacing,
                                        (yPos - (gridSize-1)/2) * ledSpacing,
                                        (zPos - (gridSize-1)/2) * ledSpacing
                                    )

                                    // The LED Sphere
                                    Model {
                                        position: ledPosition
                                        source: "#Sphere"
                                        scale: Qt.vector3d(0.4, 0.4, 0.4)
                                        
materials: PrincipledMaterial {
    baseColor: {
        if (ledColors && ledColors[xPos] && ledColors[xPos][yPos] && ledColors[xPos][yPos][zPos]) {
            // Debug output to verify colors are received
            // console.log("Color at", xPos, yPos, zPos, ":", ledColors[xPos][yPos][zPos]);
            return ledColors[xPos][yPos][zPos];
        } else {
            console.warn("Invalid color access at", xPos, yPos, zPos);
            return Qt.rgba(1, 0, 0, 1); // Fallback to red if error
        }
    }
    emissiveFactor: 2.0
    roughness: 0.0
    metalness: 0.0
    alphaMode: PrincipledMaterial.Blend
    opacity: ledColors[xPos][yPos][zPos].a || 1.0
}
                                    }

                                    // Point Light for this LED
                                    PointLight {
                                        position: ledPosition
                                        color: ledColors[xPos][yPos][zPos]
                                        brightness: 5
                                        constantFade: 1.0
                                        linearFade: 0.0
                                        quadraticFade: 0.01
                                        castsShadow: false
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
                    cameraPosition = Qt.vector3d(700, 500, 300)
                    yaw = 55.0
                    pitch = -30.0
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
