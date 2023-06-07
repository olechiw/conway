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
    game->setAlive(1, 0);
    game->setAlive(2, 0);
    game->setAlive(0, 1);
    game->setAlive(1, 1);
    game->setAlive(1, 2);

    QThread* thread = new QThread;
    ConwayWorker* worker = new ConwayWorker(game, thread);
    thread->start();

    // Bind rendering
    QObject::connect(worker, &ConwayWorker::renderFrame, canvas, &ConwayCanvas::renderFrame);

    // Bind Simulation Meter
    Meter* simulationMeter = findChild<Meter*>("simulationMeter", engine);
    QObject::connect(worker, &ConwayWorker::gameUpdated, simulationMeter, &Meter::increment);

    // Bind FPS Meter
    QQuickWindow* window = findChild<QQuickWindow*>("mainWindow", engine);
    Meter* fpsMeter = findChild<Meter*>("fpsMeter", engine);
    QObject::connect(window, &QQuickWindow::beforeRendering, fpsMeter, &Meter::increment);

    // Bind config signals to worker
    QObject::connect(appModel, &ApplicationModel::simulationDelayChanged, worker, &ConwayWorker::setDelayMilliseconds);
    QObject::connect(appModel, &ApplicationModel::pausedChanged, worker, &ConwayWorker::setPaused);

    
    return app.exec();
}
