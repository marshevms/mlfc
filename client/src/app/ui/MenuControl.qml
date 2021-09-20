import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import QtQml 2.12

import EnumerationStorage 1.0

MenuBar{
    property int fontSize: 10

    Connections
    {
        target: client
    }

    Connections
    {
        target: appInfo
    }

    ListModel{
        id: authorModel

        ListElement{
            name: "Marshev M.S."
            role: "developer"
            email: "msmarshev@gmail.com"
        }
    }

    Component{
        id: authorDelegate

        Column{
            Text {
                text: name
                font.bold: true
                font.pointSize: fontSize
            }
            Text {
                text: role
                font.italic: true
                font.pointSize: fontSize
            }

            Rectangle{
                id: outline 
                color: "transparent"
                border.width: 0
                border.color: "#3DAEE9"
                radius: 5
                width: 50; height: 32

                Image{
                anchors.fill: parent

                fillMode: Image.PreserveAspectFit
                source: "qrc:/icons/app/mail-send.svg"
                MouseArea{
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked:{
                        Qt.openUrlExternally("mailto:?to=" + email)
                    }
                    onEntered:{
                        outline.border.width = 2
                    }
                    onExited:{
                        outline.border.width = 0
                    }
                    onPressed:{
                        outline.color = "#3DAEE9"
                    }
                     onReleased:{
                        outline.color = "transparent"
                    }
                }
            }
            }
        }

    }

    Window {
        id: aboutMLFC
        flags: Qt.Dialog | Qt.WindowStaysOnTopHint
        title: qsTr("About MLFC")
        color: "#EFF0F1"
        minimumHeight: columnLayout.implicitHeight
        minimumWidth: columnLayout.implicitWidth

        ColumnLayout{
            id: columnLayout
            anchors.fill: parent

            GridLayout{
                columns: 2
                rows: 4

                Layout.margins: 10

                Image {
                    Layout.rowSpan: 4
                    source: "qrc:/icons/tray/dark_fan.svg"
                    sourceSize.width: 60
                    sourceSize.height: 60
                }

                Text {
                    text: "MSi Laptop Fan Control"
                    font.pointSize: 15
                }


                Text {
                    text: "Version: " + appInfo.version +
                          "\nServer Version: " + client.srvVersion +
                          "\nEC Version: " + client.ecVersion +
                          "\nCommit: " + appInfo.commit
                    font.pointSize: fontSize
                }
            }

            TabBar{
                id: bar

                Layout.fillWidth: true
                Layout.margins: 10

                TabButton {
                    text: qsTr("About")
                }
                TabButton {
                    text: qsTr("Libraries")
                }
                TabButton {
                    text: qsTr("Authors")
                }
            }

            StackLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.leftMargin: 10
                Layout.rightMargin: 10

                currentIndex: bar.currentIndex

                Frame{
                    ColumnLayout{
                        anchors.fill: parent
                        Text{
                             Layout.alignment: Qt.AlignTop

                            wrapMode: Text.Wrap
                            font.pointSize: fontSize
                            text: "GUI For Fan Control"
                        }

                        Text{
                            Layout.alignment: Qt.AlignBottom

                            wrapMode: Text.Wrap
                            font.pointSize: fontSize
                            text: "(C) 2020-2021 Marshev M.S."
                        }
                    }
                }

                Frame{
                    ColumnLayout{
                        anchors.fill: parent
                        Text{
                            Layout.alignment: Qt.AlignTop

                            wrapMode: Text.Wrap
                            font.pointSize: fontSize
                            text: appInfo.usedLibrary
                            onLinkActivated: Qt.openUrlExternally(link)

                        }
                    }
                }

                Frame{
                    ColumnLayout{
                        anchors.fill: parent

                        Text{
                            wrapMode: Text.Wrap
                            font.pointSize: fontSize
                            text: qsTr("Plese use <a href=\"https://github.com/marshevms/mlfc/issues\">mlfc/issues</a>  to report bugs")
                            onLinkActivated: Qt.openUrlExternally(link)
                        }

                        Item{
                            height: 10
                        }

                        ListView{
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            Layout.minimumHeight: 70

                            clip: true
                            flickableDirection: Flickable.VerticalFlick
                            boundsBehavior: Flickable.StopAtBounds
                            ScrollBar.vertical: ScrollBar {}

                            model: authorModel
                            delegate: authorDelegate
                            spacing: 10

                        }
                    }
                }
            }

            Button{
                Layout.margins: 10
                Layout.alignment: Qt.AlignRight

                text: qsTr("close")
                onClicked: aboutMLFC.close()
            }
        }
    }



    Menu{
        title: qsTr("&File")

        MenuSeparator{}

        Action{
            text: qsTr("&Quit")
            onTriggered: Qt.quit()
        }
    }

    Menu{
        title: qsTr("&View")
        Menu{
            title: qsTr("&Icon Theme")
            Action{
                text: qsTr("&Light")
                onTriggered: {client.onSetIconThemeClicked(EnumerationStorage.IconTheme.Light)}
            }
            Action{
                text: qsTr("&Dark")
                onTriggered: {client.onSetIconThemeClicked(EnumerationStorage.IconTheme.Dark)}
            }
        }
    }

    Menu{
        title: qsTr("&Help")
        Action{
            text: qsTr("&About MLFC...")
            icon.source: "qrc:/icons/tray/dark_fan.svg"

            onTriggered: {aboutMLFC.visible = true}
        }
    }
}
