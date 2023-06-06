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

    Meter {
        id: fpsMeter
    }
    Meter {
        id: simulationMeter
        objectName: "simulationMeter"
    }

    Component.onCompleted: {
        mainWindow.beforeRendering.connect(fpsMeter.increment);
    }

    GridLayout {
        columns: 2
        anchors.fill: parent
        flow: GridLayout.TopToBottom

        RowLayout {
            StandardLabel {
                Layout.leftMargin: 2
                Layout.alignment: Qt.AlignLeft
                Layout.fillWidth: true
                id: fpsMeterLabel

                Connections {
                    target: fpsMeter
                    function onMeterUpdated(value) {
                        fpsMeterLabel.text = Math.floor(value) + " FPS"
                    }
                }
            }
            StandardLabel {
                Layout.leftMargin: 5
                Layout.alignment: Qt.AlignRight
                id: simulationMeterLabel
                Connections {
                    target: simulationMeter
                    function onMeterUpdated(value) {
                        simulationMeterLabel.text = Math.floor(value) + " Steps per Second"
                    }
                }
                text: "Simulations per second"
            }
            
        }


        ConwayCanvas {
            objectName: "conwayCanvas"
            id: conwayCanvas
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
