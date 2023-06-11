#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QThread>
#include <QQuickWindow>
#include <QElapsedTimer>
#include <QQuickStyle>

#include "ConwayGame.h"
#include "ConwayCanvas.h"
#include "ConwayWorker.h"
#include "Meter.h"
#include "ApplicationModel.h"

template<typename T>
static T findChild(const QString& name, const QQmlApplicationEngine &engine) {
    if (engine.rootObjects().first()->objectName() == name) {
        return qobject_cast<T>(engine.rootObjects().first());
    }
    return engine.rootObjects().first()->findChild<T>(name);
}

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    qmlRegisterType<ConwayCanvas>("Conway", 1, 0, "ConwayCanvas");
    qmlRegisterType<Meter>("Conway", 1, 0, "Meter");
    ApplicationModel* appModel = new ApplicationModel;
    qmlRegisterSingletonInstance<ApplicationModel>("Conway", 1, 0, "ApplicationModel", appModel);

    QGuiApplication app(argc, argv);

    QQuickStyle::setStyle("Basic");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    
    if (engine.rootObjects().isEmpty())
        return -1;
    
    ConwayCanvas* canvas = findChild<ConwayCanvas*>("conwayCanvas", engine);
    QQuickWindow* window = findChild<QQuickWindow*>("mainWindow", engine);

    ConwayGame game;
    game.setAlive(0, -1);
    game.setAlive(1, -1);
    game.setAlive(-1, 0);
    game.setAlive(0, 0);
    game.setAlive(0, 1);

    QThread* thread = new QThread;
    ConwayWorker* worker = new ConwayWorker(game, appModel, thread);
    thread->start();

    // Bind rendering - order matters this should have priority
    QObject::connect(worker, &ConwayWorker::setGameState, canvas, &ConwayCanvas::setGameState);
    worker->reset();

    // Maybe move the meters to the worker?
    // Bind Simulation Meter
    Meter* simulationMeter = new Meter;
    QObject::connect(worker, &ConwayWorker::setGameState, simulationMeter, &Meter::increment);
    QObject::connect(simulationMeter, &Meter::meterUpdated, appModel, &ApplicationModel::setGenerationsPerSecond);

    // Bind FPS Meter
    Meter* fpsMeter = new Meter;
    QObject::connect(window, &QQuickWindow::beforeRendering, fpsMeter, &Meter::increment);
    QObject::connect(fpsMeter, &Meter::meterUpdated, appModel, &ApplicationModel::setCurrentFps);

    // We currently only update the canvas from the worker, this will update regularly anyway i.e. to draw gridlines
    QTimer* minimumFramerateTimer = new QTimer;
    constexpr int REFRESH_RATE_MS = 8;
    QObject::connect(minimumFramerateTimer, &QTimer::timeout, canvas, &ConwayCanvas::update);
    minimumFramerateTimer->setInterval(REFRESH_RATE_MS);
    minimumFramerateTimer->start();
    
    return app.exec();
}