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

            text: controller.originalText

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

            ButtonGroup {
                id: algorithmsGroup

                onCheckedButtonChanged: {
                    if (md5.checked) controller.algorithmType = 0;
                    if (sha256.checked) controller.algorithmType = 1;
                    if (sha384.checked) controller.algorithmType = 2;
                    if (sha512.checked) controller.algorithmType = 3;
                }
            }

            AlgorithRadioButton { id: md5;    text: "MD5";     ButtonGroup.group: algorithmsGroup }
            AlgorithRadioButton { id: sha256; text: "SHA-256"; ButtonGroup.group: algorithmsGroup }
            AlgorithRadioButton { id: sha384; text: "SHA-384"; ButtonGroup.group: algorithmsGroup }
            AlgorithRadioButton { id: sha512; text: "SHA-512"; ButtonGroup.group: algorithmsGroup }
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

            onClicked: controller.doHash(controller.algorithmType, text.text)
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

        Button {
            id: requestHistoryButton

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: hash.bottom
            anchors.topMargin: 15

            text: "Get history!"

            width: text.x + text.width - actionButton.x
            height: hash.y - actionButton.y - 15

            font.pixelSize: 22

            onClicked: controller.getHistory()
        }

    }
}
