import QtQuick 2.0
import Qt.labs.controls 1.0

Slider {
    value: 0.5
    Rectangle {
        anchors.fill: track
        color: "transparent"
        border.color: "red"
    }
}