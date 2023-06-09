import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Layouts
import QtQuick.Controls
// import QtQuick.Controls.Material
import Conway 1.0

Window {
    visible: true
    width: 1000
    height: 1000
    title: qsTr("Conway's Game of Life")
    objectName: "mainWindow"

    GridLayout {
        columns: 2
        anchors.fill: parent
        flow: GridLayout.TopToBottom

        RowLayout {
            Layout.topMargin: 3
            Layout.bottomMargin: 0
            Layout.rightMargin: 3
            Layout.leftMargin: 3
            ColumnLayout {
                Layout.alignment: Qt.AlignLeft
                StandardLabel {
                    text: "Population: " + ApplicationModel.currentPopulation
                }

                StandardLabel {
                    Layout.alignment: Qt.AlignLeft
                    id: fpsMeterLabel
                    text: Math.floor(ApplicationModel.currentFps) + " FPS"
                }
            }

            
            RowLayout {
                Layout.alignment: Qt.AlignCenter
                Button {
                    text: "Restart"
                    Layout.alignment: Qt.AlignBottom
                    id: restartButton
                }
                Button {
                    text: (ApplicationModel.paused) ? "Play" : "Pause";
                    id: pauseButton
                    Layout.alignment: Qt.AlignBottom
                    onClicked: ApplicationModel.paused = !ApplicationModel.paused;
                }
                ColumnLayout {
                    StandardLabel {
                        text: "Minimum Simulation Time (ms)"
                    }
                    SpinBox {
                        from: 0
                        to: 1000
                        editable: true
                        value: ApplicationModel.simulationDelayMs
                        id: simulationSpinner
                        Layout.alignment: Qt.AlignCenter
                        onValueModified: () => ApplicationModel.simulationDelayMs = simulationSpinner.value
                    }
                }
            }
            
            StandardLabel {
                Layout.alignment: Qt.AlignRight
                id: simulationMeterLabel
                text: Math.floor(ApplicationModel.currentSimulationsPerSecond) + " Steps per second"
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
