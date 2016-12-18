#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString file("/home/ssr/Programming/Qt_project/ChartLab/input/inv.tr0");
    qDebug() << "open file" << file;
    QString result = readFile(file);
    if(!result.isEmpty()){
        this->entity = parseData(result);
        drawCombineCharts();
        drawScatterCharts();
    }


    //    on_combinerAction_triggered();
    on_scatterAction_triggered();

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
    axisX->setTitleText(entity.getSweep());
    return axisX;
}

QValueAxis *MainWindow::getAxisY()
{
    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%.3e");
    axisY->setTitleText("Values");
    return axisY;
}

QChart *MainWindow::getChart()
{
    QChart *chart = new QChart();
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setMargins(QMargins(1,1, 1, 1));
    chart->setBackgroundBrush(QBrush(QColor(239,240,241)));
    return chart;
}

void MainWindow::drawScatterCharts()
{
    if(entity.getHeaders().size() == 0){
        return;
    }

    QVBoxLayout *layout = new QVBoxLayout;
    ui->scrollAreaWidgetContents->setLayout(layout);

    foreach (QString chartName, entity.getHeaders()) {
        QChart *chart = getChart();

        QValueAxis *axisX = getAxisX();
        chart->addAxis(axisX, Qt::AlignBottom);

        QValueAxis *axisY = getAxisY();
        chart->addAxis(axisY, Qt::AlignLeft);

        float maxValue = std::numeric_limits<float>::min(),
                minValue = std::numeric_limits<float>::max();

        QList<QPointF> points = entity.getPoints().value(chartName);
        qDebug() << chartName << " " << points.size();
        QLineSeries *series = new  QLineSeries();

        foreach (QPointF point, points) {
            maxValue = maxValue < point.y() ? point.y() : maxValue;
            minValue = minValue > point.y() ? point.y() : minValue;
            series->append(point);
        }
        series->setName(chartName);
        series->setColor(entity.getChartColor(chartName));

        chart->addSeries(series);
        chart->setAxisX(axisX, series);
        //    ui->page->hide();
        chart->setAxisY(axisY, series);
        double delta = maxValue - minValue;
        if(delta < 0.000001){
            delta = 5;
            maxValue = 5;
            minValue = 0;
        }
        axisY->setRange(minValue - delta * 0.08, maxValue + delta * 0.08);

        chart->setPlotAreaBackgroundBrush(getBackgroundBrush());
        chart->setPlotAreaBackgroundVisible(true);


        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        QVBoxLayout *layout2 = new QVBoxLayout;
        layout2->addWidget(chartView);
        layout2->setMargin(0);
        layout2->setSpacing(0);
        QWidget *widget = new QWidget();
        widget->setLayout(layout2);
        widget->setMinimumHeight(800);
        layout->addWidget(widget);
    }
}

void MainWindow::drawCombineCharts()
{
    if(entity.getHeaders().size() == 0){
        return;
    }

    QChart *chart = getChart();

    QValueAxis *axisX = getAxisX();
    chart->addAxis(axisX, Qt::AlignBottom);

    QValueAxis *axisY = getAxisY();
    chart->addAxis(axisY, Qt::AlignLeft);

    float maxValue = std::numeric_limits<float>::min(),
            minValue = std::numeric_limits<float>::max();

    foreach (QString chartName, entity.getHeaders()) {
        QList<QPointF> points = entity.getPoints().value(chartName);
        qDebug() << chartName << " " << points.size();
        QLineSeries *series = new  QLineSeries();

        foreach (QPointF point, points) {
            maxValue = maxValue < point.y() ? point.y() : maxValue;
            minValue = minValue > point.y() ? point.y() : minValue;
            series->append(point);
        }
        series->setName(chartName);

        series->setColor(entity.getChartColor(chartName));

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
    layout->setMargin(0);
    layout->setSpacing(0);
    ui->widget->setLayout(layout);
}

void MainWindow::on_openFIleAction_triggered()
{
    QString file = QFileDialog::getOpenFileName(this,
                                                tr("Open file"), "/home/ssr", tr("Tr0 Files (*.tr0)"));
    qDebug() << "open file" << file;
    QString result = readFile(file);
    this->entity = parseData(result);
    drawCombineCharts();
    drawScatterCharts();
    on_combinerAction_triggered();
}

void MainWindow::on_combinerAction_triggered()
{
    ui->scrollArea->hide();
    ui->widget->show();
}

void MainWindow::on_scatterAction_triggered()
{
    ui->widget->hide();
    ui->scrollArea->show();
}

void MainWindow::on_chartsInfoAction_triggered()
{
    if(this->entity.getHeaders().size() == 0){
        QMessageBox::warning(this, QString("Нет графика"), QString("Сначала загрузите график"), QMessageBox::Ok);
    } else {
        DetailDialog *detail = new DetailDialog(this, entity);
        detail->show();
    }
}

void MainWindow::on_infoAction_triggered()
{
    AboutDIalog *aboutDialog = new AboutDIalog(this);
    aboutDialog->show();
}
