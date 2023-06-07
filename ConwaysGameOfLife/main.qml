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

    Meter {
        id: fpsMeter
        objectName: "fpsMeter"
    }
    Meter {
        id: simulationMeter
        objectName: "simulationMeter"
    }

    GridLayout {
        columns: 2
        anchors.fill: parent
        flow: GridLayout.TopToBottom

        RowLayout {
            Layout.topMargin: 3
            Layout.bottomMargin: 0
            Layout.rightMargin: 3
            Layout.leftMargin: 3
            StandardLabel {
                Layout.alignment: Qt.AlignLeft
                id: fpsMeterLabel
                Layout.fillWidth: true
                Connections {
                    target: fpsMeter
                    function onMeterUpdated(value) {
                        fpsMeterLabel.text = Math.floor(value) + " FPS"
                    }
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
                    text: "Play"
                    id: pauseButton
                    Layout.alignment: Qt.AlignBottom
                    onClicked: (value) => {
                        pauseButton.text = (ApplicationModel.paused) ? "Pause" : "Play";
                        ApplicationModel.paused = !ApplicationModel.paused;
                    }
                }
                ColumnLayout {
                    StandardLabel {
                        text: "Minimum Simulation Time (ms)"
                    }
                    SpinBox {
                        from: 0
                        to: 1000
                        value: ApplicationModel.simulationDelayMilliseconds
                        id: simulationSpinner
                        Layout.alignment: Qt.AlignCenter
                        onValueModified: () => ApplicationModel.simulationDelayMilliseconds = simulationSpinner.value
                    }
                }
            }
            
            StandardLabel {
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
