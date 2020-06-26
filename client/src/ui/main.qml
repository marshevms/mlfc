import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.12
import QtQuick.Controls.Styles 1.4

import ServerStates 1.0


ApplicationWindow{
    id: window
    width: 500
    height: 320

    title: qsTr("MSI Laptop Fun Control")
    visible: true

    Connections
    {
        target: client
        onErrorOccurred: {
            errorMessageBox.setInformativeText(error)
            errorMessageBox.open()
        }
    }

    MessageDialog
    {
        id: errorMessageBox
        modality: Qt.WindowModal
        title: qsTr("An error occured")
        text: qsTr("An error occured")
        icon: StandardIcon.Critical
    }


    GridLayout{
        rows: 1
        columns: 2

        anchors.fill: parent

        //Temperature
        ColumnLayout{
            Layout.margins: 10
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.fillWidth: true

            Label{
                text: qsTr("Temperature")
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            }

            Frame{
                Layout.fillWidth: true

                GridLayout{
                    rows:2
                    columns: 3

                    anchors.fill: parent

                    Label{
                        text: "CPU"
                    }

                    ProgressBar{
                        from: 0
                        to: 100

                        value: cpu.temp

                        Layout.fillWidth: true
                    }

                    Label{
                        text: cpu.temp + " \xB0C"
                    }

                    Label{
                        text: "GPU"
                    }

                    ProgressBar{
                        from: 0
                        to: 100

                        value: gpu.temp
                        Layout.fillWidth: true
                    }

                    Label{
                        text: gpu.temp + " \xB0C"
                    }


                } // GridLayout
            } // Frame

        } // ColumnLayout Temperature

        //Fun speed
        ColumnLayout{
            Layout.margins: 10
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.fillWidth: true

            Label{
                text: qsTr("Fun Speed")
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            }

            Frame{
                Layout.fillWidth: true

                GridLayout{
                    rows:2
                    columns: 2

                    anchors.fill: parent

                    Label{
                        text: "CPU"
                    }

                    Label{
                        text: cpu.fanRPM + " RPM"
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    }

                    Label{
                        text: "GPU"
                    }

                    Label{
                        text: gpu.fanRPM + " RPM"
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    }


                } // GridLayout
            } // Frame

        } // ColumnLayout Fun Speed

    } // GridLayout

    footer: RowLayout{

        spacing: 5

        Item{
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        Button{
            text: qsTr("Try Restart Service")

            Layout.alignment: Qt.AlignRight

            visible: {
                switch(client.serverState)
                {
                case ServerStates.None:
                    false
                    break
                case ServerStates.Starting:
                    false
                    break
                case ServerStates.Working:
                    false
                    break
                case ServerStates.Stopped:
                    true
                    break
                }
            }
            onClicked: client.tryStartServer()
        }
    }
}
