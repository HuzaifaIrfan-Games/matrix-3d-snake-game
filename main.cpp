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

// Modify the createSampleColorArray function to ensure bright colors:
QVariantList createSampleColorArray(int size) {
    QVariantList xArray;
    for (int x = 0; x < size; x++) {
        QVariantList yArray;
        for (int y = 0; y < size; y++) {
            QVariantList zArray;
            for (int z = 0; z < size; z++) {
                // Create more vibrant colors for better visibility
                QColor color;
                // Use random bright colors for better visibility
                color.setRedF(QRandomGenerator::global()->generateDouble());
                color.setGreenF(QRandomGenerator::global()->generateDouble());
                color.setBlueF(QRandomGenerator::global()->generateDouble());
                color.setAlphaF(QRandomGenerator::global()->generateDouble() * 0.5 + 0.3); // Keep some transparency

                QVariantList colorMap;
                colorMap.append(color.redF());
                colorMap.append(color.greenF());
                colorMap.append(color.blueF());
                colorMap.append(color.alphaF());

                
                // // Convert to QVariantMap to ensure proper QML interpretation
                // QVariantMap colorMap;
                // colorMap["r"] = color.redF();
                // colorMap["g"] = color.greenF();
                // colorMap["b"] = color.blueF();
                // colorMap["a"] = color.alphaF();
                
                zArray.append(colorMap);
            }
            yArray.append(zArray);
        }
        xArray.append(yArray);
    }
    
    // Debug output to verify array structure
    qDebug() << "Generated color array with size:" 
             << xArray.size();
    return xArray;
}

int main(int argc, char *argv[]) {
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
    engine.loadFromModule("matrix_3d_snake_game", "Main");

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

#include "main.moc"