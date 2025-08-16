

#include "LedMatrixCube.hpp"
#include "Matrix3DSnakeGame/Matrix3DSnakeGame.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QColor>
#include <QVariant>
#include <QVariantList>
#include <QRandomGenerator>

class LedCubeController : public QObject {
    Q_OBJECT
public:
    explicit LedCubeController(QObject *parent = nullptr) : QObject(parent) {}
    
    Q_INVOKABLE void setColors3D(const QVariantList &colors3d) {
        emit colors3DChanged(colors3d);
    }

signals:
    void colors3DChanged(const QVariantList &colors3d);
};

// Create a 4D QVariantList: [x][y][z][c] where c=0(R),1(G),2(B)
QVariantList createSampleColorArray(int size) {
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
    
    // Register our controller
    LedCubeController ledController;
    engine.rootContext()->setContextProperty("ledController", &ledController);
    
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
    QVariantList colors3d = createSampleColorArray(5);

    // Set colors from C++ (two equivalent ways)
    
    // Method 1: Direct call through exposed API
    QMetaObject::invokeMethod(ledCubeApi, "setColors",
        Q_ARG(QVariant, QVariant::fromValue(colors3d)));
    
    // Method 2: Via signal/slot connection
    QObject::connect(&ledController, &LedCubeController::colors3DChanged,
        [rootObject](const QVariantList &colors) {
            QMetaObject::invokeMethod(rootObject, "setColors",
                Q_ARG(QVariant, QVariant::fromValue(colors)));
        });
    
    // You can trigger updates later using:
    ledController.setColors3D(colors3d);

    return app.exec();
}


#include "LedMatrixCube.moc"  