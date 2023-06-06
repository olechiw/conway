import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Layouts
import QtQuick.Controls
import Conway 1.0

Window {
    visible: true
    width: 1000
    height: 1000
    title: qsTr("Conway's Game of Life")
    objectName: "mainWindow"
    id: mainWindow

    FPSCounter {
        id: fpsCounter
    }

    Component.onCompleted: {
        fpsCounterLabel.text = "loading"
        mainWindow.beforeRendering.connect(fpsCounter.logFrame);
        fpsCounterLabel.text = "connected"
        console.log("Connected")
    }

    GridLayout {
        columns: 2
        anchors.fill: parent
        flow: GridLayout.TopToBottom

        RowLayout {

            Label {
                text: "FPS: "
                font.pixelSize: 26
                color: "black"
                Layout.leftMargin: 2
                Layout.rightMargin: 0
            }
            Label {
                text: "60"
                font.pixelSize: 26
                color: "black"
                Layout.leftMargin: 0
                id: fpsCounterLabel


                Connections {
                    target: fpsCounter
                    function onFpsUpdated(value) {
                        fpsCounterLabel.text = value
                    }
                }
            }
        }


        ConwayCanvas {
            objectName: "conwayCanvas"
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
