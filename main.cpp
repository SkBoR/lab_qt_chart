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
#include <QSplineSeries>

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





    QValueAxis *axisX = new QValueAxis;
    axisX->setLabelFormat("%.4e");
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

     QColor *test[5];// = new QColor[5];
//     test[0]= new QColor(QColor::red);
//     test[1]= new QColor(QColor::green);
     test[2]= new QColor(42, 41,51);
     test[3]= new QColor(41, 41,51);
     test[4]= new QColor(41, 41,51);
     int i = 0;

    foreach (QString chartName, entity.getPoints().keys()) {
        QList<QPointF> points = entity.getPoints().value(chartName);
        qDebug() << chartName << " " << points.size();
        QLineSeries *series = new  QLineSeries();

        foreach (QPointF point, points) {
            series->append(point);
        }
        if(i < 5){
//            series->setColor(*test[i++]);
        }

        chart->addSeries(series);
        qDebug() << "добавил в chart" << iterator.key();
        chart->setAxisX(axisX, series);
        chart->setAxisY(axisY, series);
    }

    chart->setPlotAreaBackgroundBrush(prush);
    chart->setPlotAreaBackgroundVisible(true);


    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    QMainWindow window;
    window.setCentralWidget(chartView);
    window.resize(400, 300);
    window.show();

    //    MainWindow w;

    //    w.setCentralWidget(chartView);

    //    w.show();

    return a.exec();
}
