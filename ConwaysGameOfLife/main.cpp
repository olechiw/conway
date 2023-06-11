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

    ConwayGame* game = new ConwayGame();
    game->setAlive(0, -1);
    game->setAlive(1, -1);
    game->setAlive(-1, 0);
    game->setAlive(0, 0);
    game->setAlive(0, 1);

    QThread* thread = new QThread;
    ConwayWorker* worker = new ConwayWorker(game, thread);
    thread->start();

    // Bind rendering - order matters this should have priority
    QObject::connect(worker, &ConwayWorker::nextGameState, canvas, &ConwayCanvas::nextGameState);

    // Bind Simulation Meter
    Meter* simulationMeter = new Meter;
    QObject::connect(worker, &ConwayWorker::gameWasUpdated, simulationMeter, &Meter::increment);
    QObject::connect(simulationMeter, &Meter::meterUpdated, appModel, &ApplicationModel::setGenerationsPerSecond);

    // Bind FPS Meter
    QQuickWindow* window = findChild<QQuickWindow*>("mainWindow", engine);
    Meter* fpsMeter = new Meter;
    QObject::connect(window, &QQuickWindow::beforeRendering, fpsMeter, &Meter::increment);
    QObject::connect(fpsMeter, &Meter::meterUpdated, appModel, &ApplicationModel::setCurrentFps);

    // Bind population tracker
    QObject::connect(worker, &ConwayWorker::currentPopulation, appModel, &ApplicationModel::setCurrentPopulation);
    // Bind generations tracker
    QObject::connect(worker, &ConwayWorker::setGenerations, appModel, &ApplicationModel::setGenerations);

    // Bind config signals to worker
    QObject::connect(appModel, &ApplicationModel::generationDurMsChanged, worker, &ConwayWorker::setDelayMilliseconds);
    QObject::connect(appModel, &ApplicationModel::pausedChanged, worker, &ConwayWorker::setPaused);
    QObject::connect(appModel, &ApplicationModel::advanceOneGeneration, worker, &ConwayWorker::advanceOneGeneration);


    // We currently only update the canvas from the worker, this will update regularly anyway i.e. to draw gridlines
    QTimer* minimumFramerateTimer = new QTimer;
    constexpr int REFRESH_RATE_MS = 16;
    QObject::connect(minimumFramerateTimer, &QTimer::timeout, canvas, &ConwayCanvas::update);
    minimumFramerateTimer->setInterval(REFRESH_RATE_MS);
    minimumFramerateTimer->start();
    
    return app.exec();
}
