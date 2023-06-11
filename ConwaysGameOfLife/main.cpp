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

    // Bind game state updates
    QObject::connect(worker, &ConwayWorker::gameStateChanged, canvas, &ConwayCanvas::gameStateChanged);
    worker->reset();

    // Bind Meters
    QObject::connect(worker, &ConwayWorker::gameStateChanged, &appModel->generationsPerSecondMeter, &Meter::increment);
    QObject::connect(window, &QQuickWindow::beforeRendering, &appModel->currentFpsMeter, &Meter::increment);
    
    return app.exec();
}

/*
* A click handler which draws on the game board
* Ability to zoom in/out, reworking the concept of "size" to be less jank (part of the ApplicationModel?)
* Evaluate different simulation algorithms
* Pan around? (arrow keys?) + reset view
* Menubar with hotkeys to make controls more organized
*/