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

    QRegExp regexp("(?=\"time\")(?>.*\n){7}(?=\"time\")");


    QRegularExpression regular("(?=\"time\")(?>.*\n){7}", QRegularExpression::MultilineOption);

    QFile file("/home/ssr/Programming/Qt_project/ChartLab/input/inv.tr0");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        //TODO уведомление о не прочитанном файле
        return 1;
    }

    QTextStream in(&file);
    QString result = in.readAll();

    Parser parser;
    parser.parse(result);


    QStringRef *toParse;
    int indexValues = result.indexOf("VALUE");
    if(indexValues > -1){
        toParse = new QStringRef(&result, indexValues, result.length() - indexValues);
    } else {
        //TODO сделать красиво
        return 1;
    }

    QRegularExpressionMatchIterator match = regular.globalMatch(*toParse);


    QLineSeries *series = new QLineSeries();
//        *series << QPointF(1, 1) << QPointF(2, 73) << QPointF(3, 268) << QPointF(4, 17) << QPointF(5, 4325) << QPointF(6, 723);


    //TODO сделать уведомление о том что, что-то не распарсилось
    while(match.hasNext()){
        QRegularExpressionMatch localMatch = match.next();
        if(localMatch.hasMatch()){
            QString captured = localMatch.captured(0);

            QStringList list = captured.split("\n");

            QString timeValue = list.at(0).split(" ").at(1);
            double time = timeValue.toDouble();

            // TODO сделать для множества точек (не хардкод кол-во)
            QString values = list.at(3);
            double value = values.toDouble();
//            *series << QPointF()
                       series->append((float)time, (float)value);
//            qDebug() << "time: " << time;
//            qDebug() << "value: " << value;
//            qDebug() << captured;
        }
    }

    //qDebug() << series->points();




    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->legend()->hide();
    chart->setTitle("Logarithmic axis example");

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QValueAxis *axisX = new QValueAxis;
    axisX->setLabelFormat("%f");
    axisX->setTitleText("Data point");
    axisX->setTickCount(6);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%f");
    axisY->setTitleText("Values");
    axisY->setTickCount(6);
    //    axisY->setBase(8);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

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
