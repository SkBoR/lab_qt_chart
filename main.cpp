#include "mainwindow.h"
#include <QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLogValueAxis>

using namespace QtCharts;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QLineSeries *series = new QLineSeries();
    *series << QPointF(1, 1) << QPointF(2, 73) << QPointF(3, 268) << QPointF(4, 17) << QPointF(5, 4325) << QPointF(6, 723);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->legend()->hide();
    chart->setTitle("Logarithmic axis example");

    QValueAxis *axisX = new QValueAxis;
    axisX->setTitleText("Data point");
    axisX->setTickCount(6);
    axisX->setLabelFormat("%i");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QLogValueAxis *axisY = new QLogValueAxis;
    axisY->setLabelFormat("%g");
    axisY->setTitleText("Values");
    axisY->setBase(8);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QBrush prush;
    prush.setStyle(Qt::SolidPattern);
    //можно изменять масштаб "сетки"
    QTransform transform;
    transform.scale(2.0,2.0);
    prush.setTransform(transform);


    chart->setPlotAreaBackgroundBrush(prush);
    chart->setPlotAreaBackgroundVisible(true);

    QMainWindow window;
    window.setCentralWidget(chartView);
    window.resize(400, 300);
    window.show();

    chartView->hide();

    //    MainWindow w;

    //    w.setCentralWidget(chartView);

    //    w.show();

    return a.exec();
}
