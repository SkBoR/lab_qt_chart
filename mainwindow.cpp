#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->colorPreset[0]= new QColor(Qt::red);
    this->colorPreset[1]= new QColor(Qt::green);
    this->colorPreset[2]= new QColor(Qt::blue);
    this->colorPreset[3]= new QColor(Qt::yellow);
    this->colorPreset[4]= new QColor(Qt::white);

    QString file("/home/ssr/Programming/Qt_project/ChartLab/input/inv.tr0");
    qDebug() << "open file" << file;
    QString result = readFile(file);
    TransientAnalysisEntity entity =  parseData(result);
    this->entity = &entity;
    on_combinerAction_triggered();
//    on_scatterAction_triggered();
}

MainWindow::~MainWindow()  
{
    delete ui;
}

QString MainWindow::readFile(QString pathToFile)
{
    if(pathToFile.isEmpty()){
        QMessageBox::warning(this, QString("Путь до файла пуст"), QString("Ссылка на файл пустая"), QMessageBox::Ok);
    }
    QFile file(pathToFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::warning(this, QString("Ошибка чтения файла"),
                             QString("По не ясной мне причине файл прочитать не удалось, уж прости"), QMessageBox::Ok);
    }

    QTextStream in(&file);
    return in.readAll();
}

TransientAnalysisEntity MainWindow::parseData(QString toParse)
{
    Parser parser;
    TransientAnalysisEntity entity;
    try{
        TransientAnalysisEntity entityObj = parser.parse(toParse);
        entity = entityObj;
    } catch(ParseException &wrongString){
        QMessageBox::warning(this, QString("Ошибка парсинга"), QString(wrongString.getMessage()), QMessageBox::Ok);
        qDebug() << wrongString.getMessage();
    }
    this->entity = &entity;
    return entity;
}

QBrush MainWindow::getBackgroundBrush()
{
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    //можно изменять масштаб "сетки", если она будет =)
    QTransform transform;
    transform.scale(2.0,2.0);
    brush.setTransform(transform);
    return brush;
}

QValueAxis *MainWindow::getAxisX()
{
    QValueAxis *axisX = new QValueAxis;
    axisX->setLabelFormat("%.3e");
    axisX->setTitleText(entity->getSweep());
    return axisX;
}

QValueAxis *MainWindow::getAxisY()
{
    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%.2");
    axisY->setTitleText("Values");
    return axisY;
}

QChart *MainWindow::getChart()
{
    QChart *chart = new QChart();
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setTitle("Logarithmic axis example");
    return chart;
}

void MainWindow::on_openFIleAction_triggered()
{
    QString file = QFileDialog::getOpenFileName(this,
                                                tr("Open file"), "/home/ssr", tr("Tr0 Files (*.tr0)"));
    qDebug() << "open file" << file;
    QString result = readFile(file);
    TransientAnalysisEntity entity =  parseData(result);
    this->entity = &entity;
    on_combinerAction_triggered();
}

void MainWindow::on_combinerAction_triggered()
{
    if(entity == NULL){
        return;
    }

    QChart *chart = getChart();

    QValueAxis *axisX = getAxisX();
    chart->addAxis(axisX, Qt::AlignBottom);

    QValueAxis *axisY = getAxisY();
    chart->addAxis(axisY, Qt::AlignLeft);

    int i = 0;
    float maxValue = std::numeric_limits<float>::min(),
            minValue = std::numeric_limits<float>::max();

    foreach (QString chartName, entity->getHeaders()) {
        QList<QPointF> points = entity->getPoints().value(chartName);
        qDebug() << chartName << " " << points.size();
        QLineSeries *series = new  QLineSeries();

        foreach (QPointF point, points) {
            maxValue = maxValue < point.y() ? point.y() : maxValue;
            minValue = minValue > point.y() ? point.y() : minValue;
            series->append(point);
        }
        series->setName(chartName);
        if(i < 5){
            series->setColor(*colorPreset[i++]);
        }

        chart->addSeries(series);
        chart->setAxisX(axisX, series);
        chart->setAxisY(axisY, series);
    }
    double delta = maxValue - minValue;
    axisY->setRange(minValue - delta * 0.08, maxValue + delta * 0.08);

    chart->setPlotAreaBackgroundBrush(getBackgroundBrush());
    chart->setPlotAreaBackgroundVisible(true);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(chartView);
    QWidget *widget = new QWidget();
    widget->setLayout(layout);
    this->setCentralWidget(widget);
}

void MainWindow::on_scatterAction_triggered()
{
    if(entity == NULL){
        return;
    }

    int i = 0;

    QWidget *widget = new QWidget();
    QScrollArea *scroll = new QScrollArea;
    QVBoxLayout *layout = new QVBoxLayout;
    widget->setLayout(layout);

    scroll->setWidget(widget);

//    scroll->setWidgetResizable(true);

    foreach (QString chartName, entity->getHeaders()) {
        QChart *chart = getChart();

        QValueAxis *axisX = getAxisX();
        chart->addAxis(axisX, Qt::AlignBottom);

        QValueAxis *axisY = getAxisY();
        chart->addAxis(axisY, Qt::AlignLeft);

        float maxValue = std::numeric_limits<float>::min(),
                minValue = std::numeric_limits<float>::max();

        QList<QPointF> points = entity->getPoints().value(chartName);
        qDebug() << chartName << " " << points.size();
        QLineSeries *series = new  QLineSeries();

        foreach (QPointF point, points) {
            maxValue = maxValue < point.y() ? point.y() : maxValue;
            minValue = minValue > point.y() ? point.y() : minValue;
            series->append(point);
        }
        series->setName(chartName);
        if(i < 5){
            series->setColor(*colorPreset[i++]);
        }

        chart->addSeries(series);
        chart->setAxisX(axisX, series);
        chart->setAxisY(axisY, series);
        double delta = maxValue - minValue;
        axisY->setRange(minValue - delta * 0.08, maxValue + delta * 0.08);

        chart->setPlotAreaBackgroundBrush(getBackgroundBrush());
        chart->setPlotAreaBackgroundVisible(true);

        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        layout->addWidget(chartView);
    }


//    widget->setLayout(scrollArea);
//    scrollArea->setWidget(layout);
    this->setCentralWidget(widget);
}
