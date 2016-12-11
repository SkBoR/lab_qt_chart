#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QBrush prush;
    prush.setStyle(Qt::SolidPattern);
    //можно изменять масштаб "сетки"
    QTransform transform;
    transform.scale(2.0,2.0);
    prush.setTransform(transform);

    QFile file("/home/ssr/Programming/Qt_project/ChartLab/input/inv.tr0");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        //TODO уведомление о не прочитанном файле
//        return 1;
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

    QChart *chart = new QChart();
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setTitle("Logarithmic axis example");

    QValueAxis *axisX = new QValueAxis;
    axisX->setLabelFormat("%.3e");
    axisX->setTitleText(entity.getSweep());
//    axisX->setTickCount(6);
    chart->addAxis(axisX, Qt::AlignBottom);

    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%f");
    axisY->setTitleText("Values");
//    axisY->setTickCount(6);
    //    axisY->setBase(8);
    chart->addAxis(axisY, Qt::AlignLeft);

     QColor *test[5];// = new QColor[5];
     test[0]= new QColor(Qt::red);
     test[1]= new QColor(Qt::red);
     test[2]= new QColor(Qt::blue);
     test[3]= new QColor(Qt::yellow);
     test[4]= new QColor(Qt::white);
     int i = 0;
     //TODO а нужно ли?
    float maxValue = std::numeric_limits<float>::min(),
            minValue = std::numeric_limits<float>::max();

    foreach (QString chartName, entity.getPoints().keys()) {
        QList<QPointF> points = entity.getPoints().value(chartName);
        qDebug() << chartName << " " << points.size();
        QLineSeries *series = new  QLineSeries();

        foreach (QPointF point, points) {
            series->append(point);
        }
        series->setName(chartName);
        if(i < 5){
            series->setColor(*test[i++]);
        }

        chart->addSeries(series);
        chart->setAxisX(axisX, series);
        chart->setAxisY(axisY, series);
    }

    chart->setPlotAreaBackgroundBrush(prush);
    chart->setPlotAreaBackgroundVisible(true);


    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

//    ui->stackedWidget->addWidget(chartView);
//    ui->graphicsView->se

    QVBoxLayout *layout = new QVBoxLayout(ui->page);
    layout->addWidget(chartView);
    ui->page->show();
//    ui

//    ui->mainToolBar->hide();
//    ui->stackedWidget->addWidget(chartView);

//    QMainWindow window;
//    window.setCentralWidget(chartView);
//    window.resize(400, 300);
//    window.show();

}

MainWindow::~MainWindow()  
{
    delete ui;
}

void MainWindow::on_openFIleAction_triggered()
{
    QString file = QFileDialog::getOpenFileName(this,
        tr("Open file"), "/home/ssr", tr("Tr0 Files (*.tr0)"));
    qDebug() << file;
}
