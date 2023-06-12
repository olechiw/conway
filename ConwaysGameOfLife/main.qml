import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Layouts
import QtQuick.Controls
import Conway 1.0

Window {
    visible: true
    width: 1000
    height: 1100
    title: qsTr("Conway's Game of Life")
    objectName: "mainWindow"

    GridLayout {
        rows: 2
        columns: 2
        anchors.fill: parent
        focus: true
        Keys.onPressed: (event) => {
            if (event.key == Qt.Key_Left) {
                conwayCanvas.userViewX += 1
            } else if (event.key == Qt.Key_Right) {
                conwayCanvas.userViewX -= 1
            } else if (event.key == Qt.Key_Up) {
                conwayCanvas.userViewY += 1
            } else if (event.key == Qt.Key_Down) {
                conwayCanvas.userViewY -= 1
            }
        }
        // Row 1, Column 1
        RowLayout {
            Layout.margins: 4
            ColumnLayout {
                Label {
                    text: "Generation Lifespan (ms)"
                }
                SpinBox {
                    from: 0
                    to: 1000
                    editable: true
                    value: ApplicationModel.generationDurMs
                    id: simulationSpinner
                    Layout.alignment: Qt.AlignCenter
                    onValueModified: () => ApplicationModel.generationDurMs = simulationSpinner.value
                }
            }
            Button {
                text: "Restart"
                Layout.alignment: Qt.AlignBottom
                id: restartButton
                onClicked: ApplicationModel.restart()
            }
            Button {
                text: "Advance One Generation"
                onClicked: ApplicationModel.advanceOneGeneration()
                Layout.alignment: Qt.AlignBottom
            }
            Button {
                text: (ApplicationModel.paused) ? "Play" : "Pause";
                id: pauseButton
                Layout.alignment: Qt.AlignBottom
                onClicked: ApplicationModel.paused = !ApplicationModel.paused;
            }
            CheckBox {
                text: "Draw Grid Lines"
                Layout.alignment: Qt.AlignBottom
                checked: false
                id: drawGridLines
            }
        }

        // Row 1, Column 2
        ColumnLayout {
            Layout.alignment: Qt.AlignRight
            Layout.margins: 4

            Label {
                Layout.alignment: Qt.AlignRight
                text: Math.floor(ApplicationModel.currentFps) + " FPS"
            }
            Label {
                Layout.alignment: Qt.AlignRight
                text: ApplicationModel.currentPopulation + " Living Cells"
            }
            Label {
                Layout.alignment: Qt.AlignRight

                text: ApplicationModel.generation + " Generations"
            }
            Label {
                Layout.alignment: Qt.AlignRight

                id: simulationMeterLabel
                text: Math.round(ApplicationModel.generationsPerSecond * 100) / 100 + " Generations/sec"
            }
        }

        ConwayCanvas {
            objectName: "conwayCanvas"
            id: conwayCanvas
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.columnSpan: 2
            drawGridLines: drawGridLines.checked
            enableUserView: true
        }
    }
}
