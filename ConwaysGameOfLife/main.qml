import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.platform as QtLabs
import Conway 1.0

ApplicationWindow {
    visible: true
    width: 1000
    height: 1100
    title: qsTr("Conway's Game of Life")
    objectName: "mainWindow"
    id: mainWindow

    QtLabs.MenuBar {
        QtLabs.Menu {
            id: fileMenu
            title: qsTr("&File")
            QtLabs.MenuItem {
                text: qsTr("&Quit")
                shortcut: "Ctrl+Shift+Q"
                onTriggered: mainWindow.close()
            }
        }
        QtLabs.Menu {
            title: qsTr("&Simulation")
            QtLabs.MenuItem {
                text: ApplicationModel.paused ? "&Play" : "&Pause"
                shortcut: "Space"
                onTriggered: ApplicationModel.paused = !ApplicationModel.paused;
            }
            QtLabs.MenuItem {
                text: "&Advance"
                shortcut: "Return"
                onTriggered: ApplicationModel.advanceOneGeneration()
            }
            QtLabs.MenuItem {
                text: "&Reset"
                shortcut: "Ctrl+R"
                onTriggered: { 
                    ApplicationModel.restart()
                    ApplicationModel.paused = true
                }
            }
            QtLabs.MenuItem {
                text: "Set &Lifespan"
                shortcut: "Ctrl+L"
                onTriggered: generationLifespanDialog.open()
            }
        }
        QtLabs.Menu {
            title: qsTr("&View")
            QtLabs.MenuItem {
                text: qsTr("&Settings")
                shortcut: "Ctrl+S"
                onTriggered: viewSettingsDialog.open()
            }
            QtLabs.MenuItem {
                text: qsTr("Reset to &Default")
                shortcut: "Ctrl+D"
            }
            QtLabs.MenuItem {
                text: qsTr("Draw &Grid Lines")
                shortcut: "Ctrl+G"
                checkable: true
                checked: false
                id: drawGridLines
            }
        }
    }

    Dialog {
        id: generationLifespanDialog
        anchors.centerIn: parent
        modal: true
        standardButtons: Dialog.Ok
        title: "Generation Lifespan (ms)"
        SpinBox {
            from: 0
            to: 1000
            editable: true
            focus: true
            value: ApplicationModel.generationDurMs
            id: simulationSpinner
            Layout.alignment: Qt.AlignCenter
            onValueModified: () => ApplicationModel.generationDurMs = simulationSpinner.value
            Keys.onPressed: (event) => { if (event.key == Qt.Key_Return) { generationLifespanDialog.close() } }
        }
    }

    Dialog {
        id: viewSettingsDialog
        modal: true
        standardButtons: Dialog.Ok
        title: "View Settings"
        anchors.centerIn: parent

        ColumnLayout {
            CheckBox {
                id: enableAutomaticViewPosition
                checked: true
                text: qsTr("Center View Automatically")
            }
            Label {
                text: "Center X"
            }
            TextField {
                    enabled: !enableAutomaticViewPosition.checked
                    validator: IntValidator {}
                    text: conwayCanvas.userViewX
                    id: userViewXField
                    onEditingFinished: () => conwayCanvas.userViewX = +(userViewXField.text)
            }
            Label {
                text: "Center Y"
            }
            TextField {
                enabled: !enableAutomaticViewPosition.checked
                validator: IntValidator {}
                text: conwayCanvas.userViewY
                id: userViewYField
                onEditingFinished: () => conwayCanvas.userViewY = +(userViewYField.text)
            }
            CheckBox {
                id: enableAutomaticViewDimensions
                checked: true
                text: qsTr("Size View Automatically")
            }
            Label {
                text: "Width"
            }
            TextField {
                enabled: !enableAutomaticViewDimensions.checked
                validator: IntValidator {}
                text: conwayCanvas.userViewWidth
                id: userViewWidthField
                onEditingFinished: () => conwayCanvas.userViewWidth = +(userViewWidthField.text)
            }
            Label {
                text: "Height"
            }
            TextField {
                enabled: !enableAutomaticViewDimensions.checked
                validator: IntValidator {}
                text: conwayCanvas.userViewHeight
                id: userViewHeightField
                onEditingFinished: () => conwayCanvas.userViewHeight = +(userViewHeightField.text)
            }
        }

    }

        // Row 1, Column 2
    header: RowLayout {
            Layout.margins: 4

            Label {
                text: Math.floor(ApplicationModel.currentFps) + " FPS"
            }
            Label {
                text: ApplicationModel.currentPopulation + " Living Cells"
            }
            Label {
                text: ApplicationModel.generation + " Generations"
            }
            Label {
                text: Math.round(ApplicationModel.generationsPerSecond * 100) / 100 + " Generations/sec"
            }
        }
    ConwayCanvas {
        objectName: "conwayCanvas"
        id: conwayCanvas
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        Layout.columnSpan: 2
        drawGridLines: drawGridLines.checked
        enableUserViewPosition: !enableAutomaticViewPosition.checked
        enableUserViewDimensions: !enableAutomaticViewDimensions.checked
        focus: true
        Keys.onPressed: (event) => {
            if (enableAutomaticViewPosition.checked) return
            if (event.key == Qt.Key_Left) {
                conwayCanvas.userViewX -= 1
            } else if (event.key == Qt.Key_Right) {
                conwayCanvas.userViewX += 1
            } else if (event.key == Qt.Key_Up) {
                conwayCanvas.userViewY -= 1
            } else if (event.key == Qt.Key_Down) {
                conwayCanvas.userViewY += 1
            }
        }
    }
}
