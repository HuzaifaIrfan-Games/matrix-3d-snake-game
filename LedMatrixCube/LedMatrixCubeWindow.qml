import QtQuick
import QtQuick3D
import QtQuick3D.Helpers

Window {
    id: mainWindow
    width: 1200
    height: 800
    // visibility: Window.FullScreen
    visible: true
    title: "matrix_3d_snake_game"
    
    property vector3d defaultCameraPosition: Qt.vector3d(700, 500, 300)
    property real defaultYaw: 55.0    // Left/right rotation
    property real defaultPitch: -30.0   // Up/down rotation

    // Expose API to C++
    property var ledCubeApi: QtObject {

        // Slot to receive 4D color array from C++
        function setLEDMatrixColorsFlatten(ledMatrixColorsFlatten) {
            // console.log(colors3d)
            // Convert flattened RGB array to 4D array [x][y][z][rgba]
            let gridSize = ledMatrixCube.gridSize;
            let ledMatrixColors3D = [];
            let idx = 0;
            for (let x = 0; x < gridSize; ++x) {
                ledMatrixColors3D[x] = [];
                for (let y = 0; y < gridSize; ++y) {
                    ledMatrixColors3D[x][y] = [];
                    for (let z = 0; z < gridSize; ++z) {
                        let r = ledMatrixColorsFlatten[idx++];
                        let g = ledMatrixColorsFlatten[idx++];
                        let b = ledMatrixColorsFlatten[idx++];
                        ledMatrixColors3D[x][y][z] = [r, g, b];
                    }
                }
            }

            ledMatrixCube.setLedsColors(ledMatrixColors3D);
        }

        // For C++ to read
        function getColor(x, y, z) {
            if (x >= 0 && x < ledMatrixCube.gridSize && y >= 0 && y < ledMatrixCube.gridSize && z >= 0 && z < ledMatrixCube.gridSize) {
                return ledMatrixCube.ledColors[x][y][z];
            }
            return Qt.rgba(0, 0, 0, 1);
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
            position: mainWindow.defaultCameraPosition
            eulerRotation: Qt.vector3d(mainWindow.defaultPitch, mainWindow.defaultYaw, 0)
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
                scale: Qt.vector3d(1, 0.05, 0.05)
                position: Qt.vector3d(ledMatrixCube.gridSize * ledMatrixCube.ledSpacing / 2 + 150, 0, 0)
                rotation: Qt.vector3d(90, 90, 90)
                materials: PrincipledMaterial {
                    baseColor: "red"
                    roughness: 0.2
                    metalness: 0.0
                }
            }
            // Y Axis (Green)
            Model {
                source: "#Cube"
                scale: Qt.vector3d(0.05, 1, 0.05)
                position: Qt.vector3d(0, ledMatrixCube.gridSize * ledMatrixCube.ledSpacing / 2 + 150, 0)
                rotation: Qt.vector3d(0, 0, 0)
                materials: PrincipledMaterial {
                    baseColor: "green"
                    roughness: 0.2
                    metalness: 0.0
                }
            }
            // Z Axis (Blue)
            Model {
                source: "#Cube"
                scale: Qt.vector3d(0.05, 0.05, 1)
                position: Qt.vector3d(0, 0, ledMatrixCube.gridSize * ledMatrixCube.ledSpacing / 2 + 150)
                rotation: Qt.vector3d(0, 90, 0)
                materials: PrincipledMaterial {
                    baseColor: "blue"
                    roughness: 0.2
                    metalness: 0.0
                }
            }
        }

        LedMatrixCube {
            id: ledMatrixCube
        }
    }

    WasdController {
        controlledObject: camera
        enabled: true
        acceptedButtons: Qt.Key_W | Qt.Key_A | Qt.Key_S | Qt.Key_D | Qt.Key_R | Qt.Key_F
        Keys.onPressed: {
            userInputController.handleKeyPress(event.key)
        }

        Keys.onReleased: {
            userInputController.handleKeyRelease(event.key)
        }
    }



    function resetCameraDefaults() {
        camera.position = defaultCameraPosition;
        camera.eulerRotation = Qt.vector3d(defaultPitch, defaultYaw, 0);
    }

    ControlPanel {}
}
