#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QThread>
#include <QQuickWindow>
#include <QElapsedTimer>

#include "ConwayGame.h"
#include "ConwayCanvas.h"
#include "ConwayWorker.h"
#include "FPSCounter.h"

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    qmlRegisterType<ConwayCanvas>("Conway", 1, 0, "ConwayCanvas");
    qmlRegisterType<FPSCounter>("Conway", 1, 0, "FPSCounter");

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    
    if (engine.rootObjects().isEmpty())
        return -1;

    ConwayCanvas* canvas = engine.rootObjects().first()->findChild<ConwayCanvas*>("conwayCanvas");

    ConwayGame* game = new ConwayGame();
    game->setAlive(1, 0);
    game->setAlive(2, 0);
    game->setAlive(0, 1);
    game->setAlive(1, 1);
    game->setAlive(1, 2);

    ConwayWorker* worker = new ConwayWorker(game);
    QObject::connect(worker, &ConwayWorker::gameUpdated, canvas, &ConwayCanvas::gameUpdated);
    worker->start();

    QElapsedTimer timer;
    QQuickWindow* window = qobject_cast<QQuickWindow*>(engine.rootObjects().first());

    /*
    int totalDuration = 0;
    int loops = 0;
    timer.start();
    if (window) {
        QObject::connect(window, &QQuickWindow::beforeRendering, [&]() {
            totalDuration += timer.elapsed();
            ++loops;
            if (totalDuration > 10 * 1000) {
                qDebug() << (1000.0 * loops) / totalDuration << "fps";
                totalDuration = 0;
                loops = 0;
            }
            timer.restart();
        });
    }
    */

    
    return app.exec();
}
