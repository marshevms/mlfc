import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.12
import QtQuick.Controls.Styles 1.4
import QtCharts 2.3

import EnumerationStorage 1.0
import FanModeModel 1.0


ApplicationWindow{
    id: window
    height: 540
    width: 700

    minimumHeight: 540
    minimumWidth: 700

    title: qsTr("MSI Laptop Fan Control")
    visible: true

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
            chartView.updateChart()
        }
        function onFanSpeedsChanged(){
            if (editOrSaveChartBtn.currentState  === editOrSaveChartBtn.editingState)
                return
            chartView.updateChart()
        }
    }

    Connections{
        target: gpu
        function onTempsChanged(){
            if (editOrSaveChartBtn.currentState  === editOrSaveChartBtn.editingState)
                return
            chartView.updateChart()
        }
        function onFanSpeedsChanged(){
            if (editOrSaveChartBtn.currentState  === editOrSaveChartBtn.editingState)
                return
            chartView.updateChart()
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
            Layout.columnSpan: 2

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
            Layout.columnSpan: 2

            textRole: "textRole"
            valueRole: "valueRole"
            model: fanModeModel
            currentIndex: client.fanMode
            onActivated: {
                client.setFanMode(currentValue)
            }
        }

        Switch{
            id: coolerBoost

            Layout.fillWidth: true
            Layout.columnSpan: 2

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

        ChartView{

            property bool isSelected: false
            property int selectedPointIndex: -1

            id: chartView

            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.rowSpan: 2
            Layout.columnSpan: 3

            antialiasing: true
            legend.visible: false

            enabled: {
                if (editOrSaveChartBtn.currentState === editOrSaveChartBtn.editingState){
                    scattersSeries.color = "#99ca53"
                    lineSeries.color = "#209fdf"
                }else{
                    scattersSeries.color = "gray"
                    lineSeries.color = "gray"
                    updateChart()
                }
                if (client.fanMode === EnumerationStorage.FanMode.Advanced){
                    return true
                } else{
                    scattersSeries.color = "gray"
                    lineSeries.color = "gray"
                    updateChart()
                    return false
                }
            }

            ValueAxis{
                id: valueAxisX
                min: -5
                max: 105

                tickCount: 12
                minorTickCount: 1

                labelFormat: "%d&deg;"
                titleText: "Temperature &deg;C"
            }

            ValueAxis{
                id: valueAxisY
                min: -5
                max: 105

                tickCount: 12
                minorTickCount: 1

                labelFormat: "%d%"
                titleText: "Fan speed %"
            }

            LineSeries{
                id: lineSeries
                axisX: valueAxisX
                axisY: valueAxisY

                width: 2.5
            }

            ScatterSeries{
                id: scattersSeries
                axisX: valueAxisX
                axisY: valueAxisY

                markerSize: 17
            }

            Component.onCompleted: {
                updateChart()
            }

            MouseArea{
                anchors.fill: chartView

                onPressed: {
                    if (editOrSaveChartBtn.currentState !== editOrSaveChartBtn.editingState)
                        return

                    var index = chartView.findCloserIndex(scattersSeries, Qt.point(mouse.x, mouse.y), scattersSeries.markerSize / 2)
                    if (index >= 0){
                        scattersSeries.pointLabelsVisible = true
                        chartView.select(index)
                    }
                }

                onReleased: {
                    chartView.deSelect()
                }

                onPositionChanged: {
                    if (chartView.isSelected){
                        var index = chartView.selectedPointIndex
                        var point = chartView.mapToValue(Qt.point(mouse.x, mouse.y))
                        chartView.repaintPoint(index, point)
                    }
                }
            }

            function repaintPoint(index, point){

                normalizePoint(point)
                limitBoundaries(index, point)

                scattersSeries.replace(scattersSeries.at(index).x,
                                      scattersSeries.at(index).y,
                                      point.x,
                                      point.y)
                lineSeries.replace(lineSeries.at(index + 1).x,
                                   lineSeries.at(index + 1).y,
                                   point.x,
                                   point.y)

                if (index === 0){
                    lineSeries.replace(lineSeries.at(0).x,
                                       lineSeries.at(0).y,
                                       valueAxisX.min,
                                       point.y)
                }

                if (index === scattersSeries.count - 1){
                    lineSeries.replace(lineSeries.at(index + 2).x,
                                       lineSeries.at(index + 2).y,
                                       valueAxisX.max,
                                       point.y)
                }


            }

            function findCloserIndex(series, target, range){
                for (var i = 0; i < scattersSeries.count; ++i){
                    var point = mapToPosition(scattersSeries.at(i))
                    var distance = Math.sqrt((Math.pow(point.x - target.x, 2)) +
                              (Math.pow(point.y - target.y, 2)))
                    if (distance <= range){
                        return i
                    }
                }
                return -1
            }

            function select(index){
                isSelected = true
                selectedPointIndex = index
            }

            function deSelect(){
                isSelected = false
                selectedPointIndex = -1
            }

            function limitBoundaries(index, point){

                var nextPoint;
                if (index !== scattersSeries.count - 1)
                {
                    nextPoint = scattersSeries.at(index + 1)
                    if (point.x > nextPoint.x){
                        point.x = nextPoint.x
                    }
                    if (point.y > nextPoint.y){
                        point.y = nextPoint.y
                    }
                }

                if (index >= 1)
                {
                    nextPoint = scattersSeries.at(index - 1)
                    if (point.x < nextPoint.x){
                        point.x = nextPoint.x
                    }
                    if (point.y < nextPoint.y){
                        point.y = nextPoint.y
                    }
                }

                if (index === 0){
                    point.x = 0
                }

                switch(true){
                case point.x > 100:
                    point.x = 100
                    break
                case point.x < 0:
                    point.x = 0
                    break
                }

                switch(true){
                case point.y > 100:
                    point.y = 100
                    break
                case point.y < 0:
                    point.y = 0
                    break
                }
            }

            function normalizePoint(point){
                point.x = Math.round(point.x)
                point.y = Math.round(point.y)
            }

            function paint(temps, fanSpeeds){
                scattersSeries.clear()
                lineSeries.clear()

                if (fanSpeeds.length === 0){
                    console.log("ERROR: fan speeds length is ", 0)
                    return
                }
                if (temps.length === 0){
                    console.log("ERROR: temps length is ", 0)
                    return
                }

                lineSeries.append(valueAxisX.min, fanSpeeds[0])
                lineSeries.append(0, fanSpeeds[0])
                scattersSeries.append(0, fanSpeeds[0])

                for (var i = 1, j = 0; i < fanSpeeds.length && j < temps.length; ++i, ++j){
                    scattersSeries.append(temps[j], fanSpeeds[i])
                    lineSeries.append(temps[j], fanSpeeds[i])
                }
                lineSeries.append(valueAxisX.max, fanSpeeds[fanSpeeds.length - 1])
            }

            function updateChart(){
                if (client.chartValues === EnumerationStorage.ChartValues.CPU)
                    paint(cpu.temps, cpu.fanSpeeds)
                else if (client.chartValues === EnumerationStorage.ChartValues.GPU)
                    paint(gpu.temps, gpu.fanSpeeds)
            }
        }

        ButtonGroup {id: radioGroup}

        ColumnLayout {
            Layout.fillHeight: true
            Layout.rowSpan: 2

            ColumnLayout {
                RadioButton {
                    checked: true
                    text: qsTr("CPU")
                    ButtonGroup.group: radioGroup

                    onClicked: {
                        client.chartValues = EnumerationStorage.ChartValues.CPU
                        chartView.updateChart()
                    }

                }
                RadioButton {
                    text: qsTr("GPU")
                    ButtonGroup.group: radioGroup


                    onClicked: {
                        client.chartValues = EnumerationStorage.ChartValues.GPU
                        chartView.updateChart()
                    }
                }
            }

            Button{
                id: editOrSaveChartBtn

                property bool editingState: false
                property bool savedState: true
                property bool currentState: savedState

                enabled: chartView.enabled

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
                        let values = []
                        for (let i = 0; i < scattersSeries.count; ++i){
                            values.push(scattersSeries.at(i))
                        }

                        console.log("INFO: ", values)
                        client.saveChartValues(values)
                    }

                    currentState = !currentState
                }
            }
        }

//        Item{
//            Layout.fillHeight: true
//            Layout.fillWidth: true

//            visible: {
//                if (client.fanMode === EnumerationStorage.FanMode.Advanced)
//                    false
//                else
//                    true
//            }
//        }

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
            onClicked: client.init()
        }
    }
}
