import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Window {
    id: window

    width: 640
    height: 480
    visible: true
    title: qsTr("Hasher UI Application")

    minimumWidth: 640
    minimumHeight: 480

    Item {
        anchors.fill: parent

        Rectangle {
            anchors.fill: parent
            color: "#777666"
        }

        TextArea  {
            id: text

            anchors.top: parent.top
            anchors.topMargin: 15
            anchors.horizontalCenter: parent.horizontalCenter

            width: 500
            height: 150

            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 22
            color: "orange"

            placeholderText: qsTr("Your text here")

            background: Rectangle {
                   implicitWidth: 200
                   implicitHeight: 40
                   color: "transparent"
                   border.color: "orange"
            }
        }

        GridLayout {
            id: gridLayout

            anchors.left: text.left
            anchors.top: text.bottom
            anchors.topMargin: 15

            columns: 2
            rows: 2

            AlgorithRadioButton { id: md5;    text: "MD5" }
            AlgorithRadioButton { id: sha256; text: "SHA-256" }
            AlgorithRadioButton { id: sha384; text: "SHA-384" }
            AlgorithRadioButton { id: sha512; text: "SHA-512" }
        }

        Button {
            id: actionButton

            anchors.left: gridLayout.right
            anchors.leftMargin: 15
            anchors.top: text.bottom
            anchors.topMargin: 15

            text: "Do hash!"

            width: text.x + text.width - actionButton.x
            height: hash.y - actionButton.y - 15

            font.pixelSize: 22
        }

        TextField {
            id: hash

            readOnly: true

            anchors.top: gridLayout.bottom
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter

            width: 500
            height: 50

            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 22
            color: "orange"

            background: Rectangle {
                   implicitWidth: 200
                   implicitHeight: 40
                   color: "transparent"
                   border.color: "orange"
            }

            text: controller.hashString
        }

    }
}
