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
#include "GameStatsReporter.h"

template<typename T>
static T findChild(const QString& name, const QQmlApplicationEngine &engine) {
    if (engine.rootObjects().first()->objectName() == name) {
        return qobject_cast<T>(engine.rootObjects().first());
    }
    return engine.rootObjects().first()->findChild<T>(name);
}

ConwayGame getDefaultGame()
{
    ConwayGame game;
    game.setAlive(0, -1);
    game.setAlive(1, -1);
    game.setAlive(-1, 0);
    game.setAlive(0, 0);
    game.setAlive(0, 1);
    return game;
}

int main(int argc, char *argv[])
{
    ApplicationModel* appModel = new ApplicationModel;

    qmlRegisterType<ConwayCanvas>("Conway", 1, 0, "ConwayCanvas");
    qmlRegisterType<Meter>("Conway", 1, 0, "Meter");
    qmlRegisterSingletonInstance<ApplicationModel>("Conway", 1, 0, "ApplicationModel", appModel);

    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Basic");
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    
    QThread* workerThread = new QThread;
    ConwayWorker* worker = new ConwayWorker(getDefaultGame(), appModel, workerThread);

    // Bind state requesters
    ConwayCanvas* canvas = findChild<ConwayCanvas*>("conwayCanvas", engine);
    QObject::connect(canvas, &ConwayCanvas::requestLatestState, worker, &ConwayWorker::getState, Qt::DirectConnection);

    GameStatsReporter* reporter = new GameStatsReporter(appModel);
    QObject::connect(reporter, &GameStatsReporter::requestLatestState, worker, &ConwayWorker::getState, Qt::DirectConnection);

    // Bind click handler
    QObject::connect(canvas, &ConwayCanvas::onClicked, worker, &ConwayWorker::setAlive);

    // Bind Fps Meter
    QQuickWindow* window = findChild<QQuickWindow*>("mainWindow", engine);
    QObject::connect(window, &QQuickWindow::beforeRendering, appModel, &ApplicationModel::incrementFps);

    workerThread->start();
    return app.exec();
}

/*
* Snapshotting
* Ability to zoom in/out, reworking the concept of "size" to be less jank (part of the ApplicationModel?)
* Evaluate different simulation algorithms
* Pan around? (arrow keys?) + reset view
* Menubar with hotkeys to make controls more organized
* Check all integer overflows
* Wrapping behavior
*/