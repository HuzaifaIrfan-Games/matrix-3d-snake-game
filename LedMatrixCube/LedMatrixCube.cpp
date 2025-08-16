
#include "UserInputController.hpp"
#include "LedMatrixCube.hpp"
#include "Matrix3DSnakeGame/Matrix3DSnakeGame.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QColor>
#include <QVariant>
#include <QVariantList>
#include <QRandomGenerator>

// Create a 4D QVariantList: [x][y][z][c] where c=0(R),1(G),2(B)
QVariantList createSampleLEDMatrixColorsFlattenArray(int size) {
    QVariantList colorArray;
    for (int x = 0; x < size; x++) {
        QVariantList xList;
        for (int y = 0; y < size; y++) {
            QVariantList yList;
            for (int z = 0; z < size; z++) {
                QVariantList colorChannels;
                for (int c = 0; c < 3; c++) {
                    colorChannels.append(0.1 + QRandomGenerator::global()->generateDouble() * (1.0 - 0.1));
                }
                yList.append(colorChannels);
            }
            xList.append(yList);
        }
        colorArray.append(xList);
    }

    qDebug() << "Generated 4D color array with size:" << colorArray.size() << "x" << colorArray[0].toList().size() << "x3";
    return colorArray;
}





int LedMatrixCubeMain(int argc, char *argv[]) {
    start_game();
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // Register the controller
    UserInputController userInputController;
    engine.rootContext()->setContextProperty("userInputController", &userInputController);

    // Load QML
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("matrix_3d_snake_game", "LedMatrixCubeWindow");

    // Get reference to the QML API
    QObject *rootObject = engine.rootObjects().first();
    QObject *ledCubeApi = rootObject->property("ledCubeApi").value<QObject*>();

    // Create sample 3D color array (5x5x5)
    QVariantList ledMatrixColorsFlatten = createSampleLEDMatrixColorsFlattenArray(5);
    // Direct call through exposed API
    QMetaObject::invokeMethod(ledCubeApi, "setLEDMatrixColorsFlatten",
        Q_ARG(QVariant, QVariant::fromValue(ledMatrixColorsFlatten)));
    

    



    return app.exec();
}


#include "LedMatrixCube.moc"  