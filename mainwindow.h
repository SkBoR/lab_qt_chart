#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
#include <QFileDialog>

#include <QtWidgets>
#include <QVBoxLayout>


using namespace QtCharts;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    TransientAnalysisEntity *entity = NULL;
    QColor *colorPreset[5];
    QString readFile(QString file);
    TransientAnalysisEntity parseData(QString toParse);

    QBrush getBackgroundBrush();
    QValueAxis *getAxisX();
    QValueAxis *getAxisY();
    QChart *getChart();
    void drawScatter();
    void drawCombine();

private slots:
    void on_openFIleAction_triggered();
    void on_combinerAction_triggered();
    void on_scatterAction_triggered();
};

#endif // MAINWINDOW_H
