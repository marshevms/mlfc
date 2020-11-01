import QtQuick 2.12
import QtCharts 2.3

ChartView{

    property bool isSelected: false
    property int selectedPointIndex: -1

    antialiasing: true
    legend.visible: false

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

    MouseArea{
        anchors.fill: parent

        onPressed: {
            var index = findCloserIndex(scattersSeries, Qt.point(mouse.x, mouse.y), scattersSeries.markerSize / 2)
            if (index >= 0){
                scattersSeries.pointLabelsVisible = true
                select(index)
            }
        }

        onReleased: {
            scattersSeries.pointLabelsVisible = false
            deSelect()
        }

        onPositionChanged: {
            if (isSelected){
                var index = selectedPointIndex
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
