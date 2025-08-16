import QtQuick
import QtQuick3D

// LED Cube
// LED Cube with Point Lights
Node {
    id: ledMatrixCube

    property int gridSize: 5
    property real ledSpacing: 100

    // 3D color array initialization
    property var ledColors: initialize3DColorArray()

    function initialize3DColorArray() {
        var arr = [];
        for (var x = 0; x < ledMatrixCube.gridSize; x++) {
            arr[x] = [];
            for (var y = 0; y < ledMatrixCube.gridSize; y++) {
                arr[x][y] = [];
                for (var z = 0; z < ledMatrixCube.gridSize; z++) {
                    arr[x][y][z] = Qt.rgba(0.2, 0.2, 0.2, 0.3); // Default color
                }
            }
        }
        return arr;
    }

    function setColors(colors3drgb) {
        // console.log("Received colors3d:", colors3d);
        var colors = [];
        for (var x = 0; x < ledMatrixCube.gridSize; x++) {
            colors[x] = [];
            for (var y = 0; y < ledMatrixCube.gridSize; y++) {
                colors[x][y] = [];
                for (var z = 0; z < ledMatrixCube.gridSize; z++) {
                    // colors3drgb[x][y][z] = [r, g, b]
                    var rgb = colors3drgb[x][y][z];
                    if (!rgb || rgb.length !== 3) {
                        console.warn("Invalid RGB data at", x, y, z, ":", rgb);
                        rgb = [0.3, 0.3, 0.3]; // Default color if invalid
                    }
                    var r = rgb[0];
                    var g = rgb[1];
                    var b = rgb[2];
                    if (r < 0.3) {
                        r = 0.3;
                    }
                    if (g < 0.3) {
                        g = 0.3 ;
                    }
                    if (b < 0.3) {
                        b = 0.3;
                    }
                    colors[x][y][z] = Qt.rgba(r, g, b, 0.6);
                }
            }
        }
        ledColors = colors;
    }

    Repeater3D {
        model: ledMatrixCube.gridSize // X axis
        delegate: Node {
            property int xPos: index

            Repeater3D {
                model: ledMatrixCube.gridSize // Y axis
                delegate: Node {
                    property int yPos: index

                    Repeater3D {
                        model: ledMatrixCube.gridSize // Z axis
                        delegate: Node {
                            property int zPos: index
                            property vector3d ledPosition: Qt.vector3d((xPos - (ledMatrixCube.gridSize - 1) / 2) * ledMatrixCube.ledSpacing, (yPos - (ledMatrixCube.gridSize - 1) / 2) * ledMatrixCube.ledSpacing, (zPos - (ledMatrixCube.gridSize - 1) / 2) * ledMatrixCube.ledSpacing)

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
