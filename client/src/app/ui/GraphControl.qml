import QtQuick 2.12
import QtCharts 2.3

ChartView{
    property int fontSize: 11


    QtObject{
        id: internal

        property bool isSelected: false
        property int selectedPointIndex: -1
        property int lastSelectedPoint: -1
    }

    antialiasing: true
    legend.visible: false

    ValueAxis{
        id: valueAxisX
        min: 0
        max: 100

        tickCount: 11
        minorTickCount: 1

        labelsFont.pointSize: fontSize

        labelFormat: "%d&deg;"
        titleText: "Temperature &deg;C"
    }

    ValueAxis{
        id: valueAxisY
        min: 0
        max: 100

        tickCount: 11
        minorTickCount: 1

        labelsFont.pointSize: fontSize

        labelFormat: "%d%"
        titleText: "Fan speed %"
    }

    LineSeries{
        id: lineSeries
        axisX: valueAxisX
        axisY: valueAxisY

        width: 2.5
        pointLabelsClipping: false
    }

    ScatterSeries{
        id: scattersSeries
        axisX: valueAxisX
        axisY: valueAxisY

        markerSize: 17

        pointLabelsClipping: false
        pointLabelsFormat: "@xPoint\u00B0C, @yPoint%"
        pointLabelsFont.pointSize: fontSize
    }

    MouseArea{
        anchors.fill: parent

        onPressed: {
            var index = findCloserIndex(scattersSeries, Qt.point(mouse.x, mouse.y), scattersSeries.markerSize / 2)
            if (index >= 0){
                select(index)
            }
        }

        onReleased: {
            deSelect()
        }

        onPositionChanged: {
            if (internal.isSelected){
                var index = internal.selectedPointIndex
                var point = mapToValue(Qt.point(mouse.x, mouse.y))
                repaintPoint(index, point)
            }
        }
    }

    function getScatterSeries(){
        return scattersSeries
    }

    function getLineSeries(){
        return lineSeries
    }

    function repaintPoint(index, point){

        normalizePoint(point)
        limitBoundaries(index, point)

        repaintAbstractSeriesPoint(scattersSeries, index, point)
        repaintAbstractSeriesPoint(lineSeries, index + 1, point)

        if (index === 0){
            repaintAbstractSeriesPoint(lineSeries, index, Qt.point(valueAxisX.min, point.y))
        }

        if (index === scattersSeries.count - 1){
            repaintAbstractSeriesPoint(lineSeries, index + 2, Qt.point(valueAxisX.max, point.y))
        }


    }

    function repaintAbstractSeriesPoint(abstractSeries, index, point){
        abstractSeries.remove(index)
        abstractSeries.insert(index, point.x, point.y)
    }

    function findCloserIndex(series, target, range){
        var arr = []
        var atLeastOne = false

        for (var i = 0; i < scattersSeries.count; ++i){
            var point = mapToPosition(scattersSeries.at(i))
            var distance = Math.sqrt((Math.pow(point.x - target.x, 2)) +
                                     (Math.pow(point.y - target.y, 2)))
            if (distance <= range){
                arr.push(i)
                atLeastOne = true
            } else if (atLeastOne === true){
                if (arr.length === 1){
                    internal.lastSelectedPoint = arr[0]
                    return arr[0]
                }
            }
        }

        if (arr.length > 0){
            if (arr.length === 1){
                internal.lastSelectedPoint = arr[0]
                return arr[0]
            } else if (scattersSeries.at(arr[0]).x === valueAxisX.min){
                internal.lastSelectedPoint = arr[arr.length - 1]
                return arr[arr.length - 1]
            } else if (scattersSeries.at(arr[arr.length - 1]).x === valueAxisX.max){
                internal.lastSelectedPoint = arr[0]
                return arr[0]
            } else {
                if (internal.lastSelectedPoint !== -1 && internal.lastSelectedPoint !== 0 && arr.indexOf(internal.lastSelectedPoint) !== -1){
                    return internal.lastSelectedPoint
                }

                internal.lastSelectedPoint = 0
                return arr[0]
            }
        }

        return -1
    }

    function select(index){
        internal.isSelected = true
        internal.selectedPointIndex = index
    }

    function deSelect(){
        internal.isSelected = false
        internal.selectedPointIndex = -1
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

    function paint(xAxis, yAxis){
        scattersSeries.clear()
        lineSeries.clear()

        if (xAxis.length !== yAxis.length){
            console.log("xAxis.length != yAxis.length")
            return
        }

        if (xAxis.length === 0 || yAxis.length === 0){
            console.log("xAxis.length = 0 || yAxis.length = 0 ")
        }


        lineSeries.append(valueAxisX.min, yAxis[0])

        for (var i = 0; i < yAxis.length; ++i){
            scattersSeries.append(xAxis[i], yAxis[i])
            lineSeries.append(xAxis[i], yAxis[i])
        }

        lineSeries.append(valueAxisX.max, yAxis[yAxis.length - 1])
    }
}
