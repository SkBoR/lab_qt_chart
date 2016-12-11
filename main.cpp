#include "mainwindow.h"
#include <QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLogValueAxis>

#include <QRegularExpression>
#include <QDebug>
#include <QFile>
#include <QStringRef>
#include <QList>
#include <QStringList>
#include "parser.h"
#include "transientanalysisentity.h"
#include <QListIterator>

using namespace QtCharts;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    QBrush prush;
    prush.setStyle(Qt::SolidPattern);
    //можно изменять масштаб "сетки"
    QTransform transform;
    transform.scale(2.0,2.0);
    prush.setTransform(transform);



    QRegularExpression regular("(?=\"time\")(?>.*\n){7}", QRegularExpression::MultilineOption);

    QFile file("/home/ssr/Programming/Qt_project/ChartLab/input/inv.tr0");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        //TODO уведомление о не прочитанном файле
        return 1;
    }

    QTextStream in(&file);
    QString result = in.readAll();

    Parser parser;
    TransientAnalysisEntity entity;
    try{
        TransientAnalysisEntity entityObj = parser.parse(result);
        entity = entityObj;
    } catch(ParseException &wrongString){
        qDebug() << wrongString.getMessage();
    }
    qDebug() << entity.getHeaders();

    QHashIterator<QString, QList<QPointF>> iterator(entity.getPoints());
    while(iterator.hasNext()){
        iterator.next();
        qDebug() << iterator.key() << " " << iterator.value().size();
    }

    QStringRef *toParse;
    int indexValues = result.indexOf("VALUE");
    if(indexValues > -1){
        toParse = new QStringRef(&result, indexValues, result.length() - indexValues);
    } else {
        //TODO сделать красиво
        qDebug() <<" jflkdassssssssssssss";
    }

    QChart *chart = new QChart();
    //    chart->addSeries(series);
    chart->legend()->hide();
    chart->setTitle("Logarithmic axis example");




    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QValueAxis *axisX = new QValueAxis;
    axisX->setLabelFormat("%f");
    axisX->setTitleText("Data point");
    axisX->setTickCount(6);
    chart->addAxis(axisX, Qt::AlignBottom);
    //    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%f");
    axisY->setTitleText("Values");
    axisY->setTickCount(6);
    //    axisY->setBase(8);
    chart->addAxis(axisY, Qt::AlignLeft);
    //    series->attachAxis(axisY);

    QHashIterator<QString, QList<QPointF>> iteratorPoint(entity.getPoints());
    while(iteratorPoint.hasNext()){
        iteratorPoint.next();
        qDebug() << iteratorPoint.key() << " " << iteratorPoint.value().size();

        QLineSeries *series = new QLineSeries();
        QListIterator<QPointF> valuesIterator(iterator.value());
        while(valuesIterator.hasNext()){
            series->append(valuesIterator.next());
            //            qDebug()<< "add to series";
        }

        chart->addSeries(series);
        qDebug() << "добавил в chart";
        series->attachAxis(axisX);
        series->attachAxis(axisY);
    }

    chart->setPlotAreaBackgroundBrush(prush);
    chart->setPlotAreaBackgroundVisible(true);

    QMainWindow window;
    window.setCentralWidget(chartView);
    window.resize(400, 300);
    window.show();

    //    MainWindow w;

    //    w.setCentralWidget(chartView);

    //    w.show();

    return a.exec();
}
