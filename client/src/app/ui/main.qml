import Qt.labs.platform 1.1 as Labs

import QtQuick 2.12
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.12
import QtQuick.Controls.Styles 1.4
import QtCharts 2.3

import EnumerationStorage 1.0
import FanModeModel 1.0

ApplicationWindow{
    property int fontSize: 10

    id: window
    height: 540
    width: 700

    minimumHeight: 540
    minimumWidth: 700

    title: qsTr("MSi Laptop Fan Control")
    visible: true

    onClosing: {
        window.hide()
    }

    Connections
    {
        target: client
        function onErrorOccurred(error) {
            console.log(error)
            errorMessageBox.setInformativeText(error)
            errorMessageBox.open()
        }
    }

    Connections{
        target: cpu
        function onTempsChanged(){
            if (editOrSaveChartBtn.currentState === editOrSaveChartBtn.editingState)
                return
            graphControl.updateGraph()
        }
        function onFanSpeedsChanged(){
            if (editOrSaveChartBtn.currentState  === editOrSaveChartBtn.editingState)
                return
            graphControl.updateGraph()
        }
    }

    Connections{
        target: gpu
        function onTempsChanged(){
            if (editOrSaveChartBtn.currentState  === editOrSaveChartBtn.editingState)
                return
            graphControl.updateGraph()
        }
        function onFanSpeedsChanged(){
            if (editOrSaveChartBtn.currentState  === editOrSaveChartBtn.editingState)
                return
            graphControl.updateGraph()
        }
    }

    FanModeModel{
        id: fanModeModel
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
        columns: 4

        anchors.fill: parent
        //Temperature
        ColumnLayout{
            Layout.margins: 10
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.fillWidth: true
            Layout.columnSpan: 2

            Label{
                text: qsTr("Temperature")
                font.pointSize: fontSize
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
                        font.pointSize: fontSize
                    }

                    ProgressBar{
                        from: 0
                        to: 100

                        value: cpu.temp

                        Layout.fillWidth: true
                    }

                    Label{
                        text: cpu.temp + " \xB0C"
                        font.pointSize: fontSize
                    }

                    Label{
                        text: "GPU"
                        font.pointSize: fontSize
                    }

                    ProgressBar{
                        from: 0
                        to: 100

                        value: gpu.temp
                        Layout.fillWidth: true
                    }

                    Label{
                        text: gpu.temp + " \xB0C"
                        font.pointSize: fontSize
                    }


                } // GridLayout
            } // Frame

        } // ColumnLayout Temperature

        //Fan speed
        ColumnLayout{
            Layout.margins: 10
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.fillWidth: true
            Layout.columnSpan: 2

            Label{
                text: qsTr("Fan Speed")
                font.pointSize: fontSize
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
                        font.pointSize: fontSize
                    }

                    Label{
                        text: cpu.fanRPM + " RPM"
                        font.pointSize: fontSize
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    }

                    Label{
                        text: "GPU"
                        font.pointSize: fontSize
                    }

                    Label{
                        text: gpu.fanRPM + " RPM"
                        font.pointSize: fontSize
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    }


                } // GridLayout
            } // Frame

        } // ColumnLayout Fan Speed

        ComboBox{
            id: fanMode
            font.pointSize: fontSize
            Layout.fillWidth: true
            Layout.columnSpan: 2
            Layout.leftMargin: 10
            Layout.rightMargin: 10

            textRole: "textRole"
            valueRole: "valueRole"
            model: fanModeModel
            currentIndex: client.fanMode
            onActivated: {
                client.onSetFanModeClicked(currentValue)
            }
        }

        Switch{
            id: coolerBoost

            Layout.fillWidth: true
            Layout.columnSpan: 2
            Layout.leftMargin: 10
            Layout.rightMargin: 10

            text: qsTr("Cooler Boost")
            font.pointSize: 15

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
                client.onSetCoolerBoostClicked(coolerBoost.checked? EnumerationStorage.CoolerBoost.ON : EnumerationStorage.CoolerBoost.OFF)
            }
        }

        GraphControl{
            id: graphControl
            fontSize: fontSize

            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.rowSpan: 2
            Layout.columnSpan: 3

            enabled: {
                if (editOrSaveChartBtn.currentState === editOrSaveChartBtn.editingState && client.fanMode === EnumerationStorage.FanMode.Advanced){
                    graphControl.getScatterSeries().color = "#99ca53"
                    graphControl.getLineSeries().color = "#209fdf"

                    graphControl.getScatterSeries().pointLabelsVisible = true

                    return true
                }else{
                    graphControl.getScatterSeries().color = "gray"
                    graphControl.getLineSeries().color = "gray"
                    graphControl.updateGraph()

                    graphControl.getScatterSeries().pointLabelsVisible = false

                    return false
                }
            }

            function paintGraph(temps, fanSpeeds){
                let copyTemps = []

                copyTemps[0] = 0

                for(let key in temps)
                    copyTemps.push(temps[key])

                graphControl.paint(copyTemps, fanSpeeds)
            }

            function updateGraph(){
                if (client.chartValues === EnumerationStorage.ChartValues.CPU)
                    paintGraph(cpu.temps, cpu.fanSpeeds)
                else if (client.chartValues === EnumerationStorage.ChartValues.GPU)
                    paintGraph(gpu.temps, gpu.fanSpeeds)
            }
        }

        ButtonGroup {id: radioGroup}

        ColumnLayout {
            Layout.fillHeight: true
            Layout.rowSpan: 2
            Layout.rightMargin: 10

            ColumnLayout {
                RadioButton {
                    checked: true
                    text: qsTr("CPU")
                    font.pointSize: 15
                    ButtonGroup.group: radioGroup

                    onClicked: {
                        client.chartValues = EnumerationStorage.ChartValues.CPU
                        graphControl.updateGraph()
                    }

                }
                RadioButton {
                    text: qsTr("GPU")
                    font.pointSize: 15
                    ButtonGroup.group: radioGroup


                    onClicked: {
                        client.chartValues = EnumerationStorage.ChartValues.GPU
                        graphControl.updateGraph()
                    }
                }
            }

            Button{
                id: editOrSaveChartBtn
                font.pointSize: fontSize

                property bool editingState: false
                property bool savedState: true
                property bool currentState: savedState

                enabled: client.fanMode === EnumerationStorage.FanMode.Advanced

                text: {
                    if (currentState == savedState){
                        return qsTr("Edit")
                    } else if (currentState == editingState){
                        return qsTr("Save")
                    }
                }

                onClicked: {
                    if (currentState == editingState)
                    {
                        // I Hate Qml
                        // Why I can't just pass qml ScatterSeries to C++ layer
                        let values = []
                        for (let i = 0; i < graphControl.getScatterSeries().count; ++i){
                            values.push(graphControl.getScatterSeries().at(i))
                        }

                        tempsFanSpeeds.setTempsFanSpeeds(values)

                        console.log("INFO: ", values)
                        client.onSaveChartValuesClicked(tempsFanSpeeds, client.chartValues)
                    }

                    currentState = !currentState
                }
            }
        } //ColumnLayout
    } // GridLayout

    footer: RowLayout{

        spacing: 5

        Item{
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        Button{
            text: qsTr("Try Restart Service")
            font.pointSize: fontSize

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
            onClicked: client.init()
        }
    }

    Labs.SystemTrayIcon{
        property string darkIcon: "qrc:/icons/tray/dark_fan.svg"
        property string lightIcon: "qrc:/icons/tray/light_fan.svg"

        id: menu
        visible: true
        tooltip: qsTr("MSi Laptop Fan Control")
        icon.source: {
            switch(client.iconTheme){
            case EnumerationStorage.IconTheme.Unknown:
                return lightIcon
            case EnumerationStorage.IconTheme.Light:
                return lightIcon
            case EnumerationStorage.IconTheme.Dark:
                return darkIcon
            }
        }

        menu: Labs.Menu{
            id: trayMenu

            Labs.MenuItem {
                text: qsTr("Open")
                onTriggered: {
                    window.show()
                    window.raise()
                    window.requestActivate()
                }
            }

            Labs.MenuSeparator{}

            Labs.MenuItem{
                text: qsTr("Light Icon")
                onTriggered: {client.onSetIconThemeClicked(EnumerationStorage.IconTheme.Light)}
            }

            Labs.MenuItem{
                text: qsTr("Dark Icon")
                onTriggered: {client.onSetIconThemeClicked(EnumerationStorage.IconTheme.Dark)}
            }

            Labs.MenuSeparator{}

            Labs.MenuItem {
                text: qsTr("Quit")
                onTriggered: Qt.quit()
            }
        }

        onActivated: {
            switch (reason){
            case Labs.SystemTrayIcon.Trigger:
                if (window.visible && window.active){
                    window.hide()
                } else if (window.visible && !window.active){
                    window.raise()
                    window.requestActivate()
                } else if ((!window.visible && !window.active) || !window.visible){
                    window.show()
                    window.raise()
                    window.requestActivate()
                }
            }
        }
    }
}
