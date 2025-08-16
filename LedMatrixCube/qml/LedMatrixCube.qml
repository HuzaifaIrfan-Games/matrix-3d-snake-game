import QtQuick
import QtQuick3D

// LED Cube
// LED Cube with Point Lights
Node {
    id: ledMatrixCube

    property int matrixSize: 6
    property real ledSpacing: 100

    // 3D color array initialization
    property var ledColors:initialize3DColorArray()

    function initialize3DColorArray() {
        var tmpLedColors = [];
        for (var x = 0; x < ledMatrixCube.matrixSize; x++) {
            var y_arr = [];
            for (var y = 0; y < ledMatrixCube.matrixSize; y++) {
                var z_arr = [];
                for (var z = 0; z < ledMatrixCube.matrixSize; z++) {
                    z_arr.push(Qt.rgba(0.0, 0.0, 0.0, 0.6));
                }
                y_arr.push(z_arr);
            }
            tmpLedColors.push(y_arr);
        }
        return tmpLedColors;
    }



    function setLedColor(x,y,z,r,g,b) {
        if (x >= 0 && x < ledMatrixCube.matrixSize && y >= 0 && y < ledMatrixCube.matrixSize && z >= 0 && z < ledMatrixCube.matrixSize) {

            if (r < 0.3) {
                r = 0.3;
            }
            if (g < 0.3) {
                g = 0.3 ;
            }
            if (b < 0.3) {
                b = 0.3;
            }

            ledMatrixCube.ledColors[x][y][z] = Qt.rgba(r, g, b, 0.6);
            // Debug output to verify color setting
            // console.log("Set LED color at", x, y, z, "to:", r, g, b);
            return;
        }
        console.warn("Invalid LED coordinates:", x, y, z, "for color:", r, g, b);
    }


    function setLedsColors(ledMatrixColors3D) {
        // console.log("Received colors3d:", colors3d);
        for (var x = 0; x < ledMatrixCube.matrixSize; x++) {
            for (var y = 0; y < ledMatrixCube.matrixSize; y++) {
                for (var z = 0; z < ledMatrixCube.matrixSize; z++) {
                    // ledMatrixColors3D[x][y][z] = [r, g, b]
                    var rgb = ledMatrixColors3D[x][y][z];

                    ledMatrixCube.setLedColor(x, y, z, rgb[0], rgb[1], rgb[2]);

                }
            }
        }

        ledMatrixCube.ledColors=ledMatrixCube.ledColors
        
        // console.log("LED colors updated.");
        // console.log("LED colors:", ledMatrixCube.ledColors);

    }

    Repeater3D {
        model: ledMatrixCube.matrixSize // X axis
        delegate: Node {
            property int xPos: index

            Repeater3D {
                model: ledMatrixCube.matrixSize // Y axis
                delegate: Node {
                    property int yPos: index

                    Repeater3D {
                        model: ledMatrixCube.matrixSize // Z axis
                        delegate: Node {
                            property int zPos: index
                            property vector3d ledPosition: Qt.vector3d((xPos - (ledMatrixCube.matrixSize - 1) / 2) * ledMatrixCube.ledSpacing, (yPos - (ledMatrixCube.matrixSize - 1) / 2) * ledMatrixCube.ledSpacing, (zPos - (ledMatrixCube.matrixSize - 1) / 2) * ledMatrixCube.ledSpacing)

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
                                            return ledMatrixCube.ledColors[xPos][yPos][zPos];
                                        } else {
                                            console.warn("Invalid color access at", xPos, yPos, zPos);
                                            return Qt.rgba(1, 0, 0, 1); // Fallback to red if error
                                        }
                                    }
                                    emissiveFactor: 2.0
                                    roughness: 0.0
                                    metalness: 0.0
                                    alphaMode: PrincipledMaterial.Blend
                                    opacity: ledMatrixCube.ledColors[xPos][yPos][zPos].a || 1.0
                                }
                            }

                            // Point Light for this LED
                            PointLight {
                                position: ledPosition
                                color: ledMatrixCube.ledColors[xPos][yPos][zPos]
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
