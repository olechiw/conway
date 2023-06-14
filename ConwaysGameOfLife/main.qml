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
                id: enableAutomaticView
                checked: true
                text: qsTr("Set View Automatically")
            }
            Label {
                text: "View X"
            }
            TextField {
                    enabled: !enableAutomaticView.checked
                    validator: IntValidator {}
                    text: conwayCanvas.userViewX
                    id: userViewXField
                    onEditingFinished: () => conwayCanvas.userViewX = +(userViewXField.text)
            }
            Label {
                text: "View Y"
            }
            TextField {
                enabled: !enableAutomaticView.checked
                validator: IntValidator {}
                text: conwayCanvas.userViewY
                id: userViewYField
                onEditingFinished: () => conwayCanvas.userViewY = +(userViewYField.text)
            }
            Label {
                text: "View Width"
            }
            TextField {
                enabled: !enableAutomaticView.checked
                validator: IntValidator {}
                text: conwayCanvas.userViewMinimumWidth
                id: userViewWidthField
                onEditingFinished: () => conwayCanvas.userViewMinimumWidth = +(userViewWidthField.text)
            }
            Label {
                text: "View Height"
            }
            TextField {
                enabled: !enableAutomaticView.checked
                validator: IntValidator {}
                text: conwayCanvas.userViewMinimumHeight
                id: userViewHeightField
                onEditingFinished: () => conwayCanvas.userViewMinimumHeight = +(userViewHeightField.text)
            }
        }

    }

        // Row 1, Column 2
    header: GridLayout {
            Layout.margins: 4
            columns: 4
            width: parent.width

            Label {
                Layout.minimumWidth: mainWindow.width / 4
                text: Math.floor(ApplicationModel.currentFps) + " FPS"
            }
            Label {
                Layout.minimumWidth: mainWindow.width / 4
                text: ApplicationModel.currentPopulation + " Living Cells"
            }
            Label {
                Layout.minimumWidth: mainWindow.width / 4
                text: ApplicationModel.generation + " Generations"
            }
            Label {
                Layout.minimumWidth: mainWindow.width / 4
                text: Math.round(ApplicationModel.generationsPerSecond * 100) / 100 + " Generations/sec"
            }
        }
    ConwayCanvas {
        objectName: "conwayCanvas"
        id: conwayCanvas
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        Layout.columnSpan: 2
        drawGridLines: drawGridLines.checked
        enableUserViewPosition: !enableAutomaticView.checked
        enableUserViewDimensions: !enableAutomaticView.checked
        focus: true
        Keys.onPressed: (event) => {
            if (enableAutomaticView.checked) return
            if (event.key == Qt.Key_Left) {
                conwayCanvas.userViewX -= 1
            } else if (event.key == Qt.Key_Right) {
                conwayCanvas.userViewX += 1
            } else if (event.key == Qt.Key_Up) {
                conwayCanvas.userViewY -= 1
            } else if (event.key == Qt.Key_Down) {
                conwayCanvas.userViewY += 1
            } else if (event.key == Qt.Key_Minus && conwayCanvas.userViewMinimumHeight > 3 && conwayCanvas.userViewMinimumWidth > 3) {
                conwayCanvas.userViewMinimumHeight += 1
                conwayCanvas.userViewMinimumWidth += 1
            } else if (event.key == Qt.Key_Plus && conwayCanvas.userViewMinimumHeight > 3 && conwayCanvas.userViewMinimumWidth > 3) {
                conwayCanvas.userViewMinimumHeight -= 1
                conwayCanvas.userViewMinimumWidth -= 1
            }
        }
    }
}
