
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
#include <QTimer>

SnakeGame snakeGame = newSnakeGame();

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


QVariantList getLEDMatrixColorsBufferFlatten(){

    QVariantList bufferList;
    for (int x = 0; x < MATRIX_SIZE; x++) {
        for (int y = 0; y < MATRIX_SIZE; y++) {
            for (int z = 0; z < MATRIX_SIZE; z++) {
                ColorRGB color = snakeGame.ledMatrixColorsBuffer[x][y][z];
                float r = color.r / 255.0f;
                float g = color.g / 255.0f;
                float b = color.b / 255.0f;

                QVariantList colorChannels;
                colorChannels.append(r);
                colorChannels.append(g);
                colorChannels.append(b);
                bufferList.append(colorChannels);
            }
        }
    }
    return bufferList;
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


    // QVariantList ledMatrix3DBufferFlatten = getLEDMatrix3DBufferFlatten();
    // // Direct call through exposed API
    // QMetaObject::invokeMethod(ledCubeApi, "setLEDMatrixColorsFlatten",
    //     Q_ARG(QVariant, QVariant::fromValue(ledMatrix3DBufferFlatten)));


    // Connect UserInputController signals to game logic
    QObject::connect(&userInputController, &UserInputController::gameResetPressed, [&]() {
        // qDebug() << "Game reset requested";
        snakeGame.reset(&snakeGame);
    });

    QObject::connect(&userInputController, &UserInputController::gameSnakeDirectionChanged, [&](SnakeDirection direction) {
        // qDebug() << "Game snake direction changed to:" << direction;
        snakeGame.change_direction(&snakeGame, direction);
    });



    // Create a QTimer for game ticks
    QTimer *gameTickTimer = new QTimer(&app);
    QObject::connect(gameTickTimer, &QTimer::timeout, [&]() {
        snakeGame.loop_tick(&snakeGame); // Advance game logic
        QVariantList ledMatrixColorsBufferFlatten = getLEDMatrixColorsBufferFlatten();
        QMetaObject::invokeMethod(ledCubeApi, "setLEDMatrixColorsFlatten",
            Q_ARG(QVariant, QVariant::fromValue(ledMatrixColorsBufferFlatten)));
    });
    gameTickTimer->start(1000); // Tick every 150 ms




    return app.exec();
}


#include "LedMatrixCube.moc"  