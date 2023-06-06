import QtQuick 2.9
import QtQuick.Window 2.2
import Conway 1.0

Window {
    visible: true
    width: 1000
    height: 1000
    title: qsTr("Conway's Game of Life")

    ConwayCanvas {
        objectName: "conwayCanvas"
        width: 1000; height: 1000;
        x: 0; y: 0;
    }
}
