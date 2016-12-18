#include "detaildialog.h"
#include "ui_detaildialog.h"

DetailDialog::DetailDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DetailDialog)
{
    ui->setupUi(this);
}

DetailDialog::DetailDialog(QWidget *parent, TransientAnalysisEntity &entity) :QDialog(parent), ui(new Ui::DetailDialog)
{
    ui->setupUi(this);
    int count = 0;
    qDebug() << entity.getHeaders();
    ui->tableWidget->setRowCount(entity.getHeaders().size());

    foreach (QString chartName, entity.getHeaders()) {
        ui->tableWidget->setItem(count, 0, new QTableWidgetItem(chartName));
        QTableWidgetItem *colorItem = new QTableWidgetItem;
        QBrush color(entity.getChartColor(chartName));
        colorItem->setBackground(color);

        ui->tableWidget->setItem(count, 1, colorItem);

        int numberPoints = 0;
        if(entity.getPoints().contains(chartName)){
            numberPoints = entity.getPoints().value(chartName).size();
        }
        ui->tableWidget->setItem(count, 2, new QTableWidgetItem(QString::number(numberPoints)));

        float max = std::numeric_limits<float>::min(),
                min = std::numeric_limits<float>::max();
        if (numberPoints > 0){
            QList<QPointF> points = entity.getPoints().value(chartName);
            foreach( QPointF point, points){
                max = point.y() > max ? point.y() : max;
                min = point.y() < min ? point.y() : min;
            }
        }
        ui->tableWidget->setItem(count, 3, new QTableWidgetItem(numberPoints > 0 ? QString::number(max) : "NaN"));
        ui->tableWidget->setItem(count, 4, new QTableWidgetItem(numberPoints > 0 ? QString::number(min) : "NaN"));

        count++;
    }
}

DetailDialog::~DetailDialog()
{
    delete ui;
}
