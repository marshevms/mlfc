import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.12
import QtQuick.Controls.Styles 1.4

import EnumerationStorage 1.0


ApplicationWindow{
    id: window
    width: 520
    height: 320

    minimumWidth: 520

    title: qsTr("MSI Laptop Fan Control")
    visible: true

    Connections
    {
        target: client
        function onErrorOccurred(error) {
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

        //Fan speed
        ColumnLayout{
            Layout.margins: 10
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.fillWidth: true

            Label{
                text: qsTr("Fan Speed")
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

        } // ColumnLayout Fan Speed

        ComboBox{
            id: fanMode
            Layout.fillWidth: true
            model: ListModel{
                id: fanModeModel
                ListElement {name: "Auto";}
                ListElement {name: "Basic";}
                ListElement {name: "Advanced";}
            }
            currentIndex: client.fanMode

            onActivated: {
                client.setFanMode(index)
            }
        }

        Switch{
            id: coolerBoost

            Layout.fillWidth: true

            text: qsTr("Cooler Boost")
            font.pointSize: 14

            checked:  {
                switch(client.coolerBoost)
                {
                case EnumerationStorage.CoolerBoost.Unknown:
                    false
                    break
                case EnumerationStorage.CoolerBoost.OFF:
                    false
                    break
                case EnumerationStorage.CoolerBoost.ON:
                    true
                    break
                }
            }
            onReleased: {
               client.setCoollerBoost(coolerBoost.checked? EnumerationStorage.CoolerBoost.ON : EnumerationStorage.CoolerBoost.OFF)
            }
        }

        Item{
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

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
                case EnumerationStorage.ServerStates.Unknown:
                    false
                    break
                case EnumerationStorage.ServerStates.Starting:
                    false
                    break
                case EnumerationStorage.ServerStates.Working:
                    false
                    break
                case EnumerationStorage.ServerStates.Stopped:
                    true
                    break
                }
            }
            onClicked: client.tryStartServer()
        }
    }
}
