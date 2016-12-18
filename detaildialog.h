#ifndef DETAILDIALOG_H
#define DETAILDIALOG_H

#include <QDialog>
#include <transientanalysisentity.h>
#include <QList>
#include <QStringList>
#include <QDebug>

namespace Ui {
class DetailDialog;
}

struct DetailEntity{
    QString name;
    QColor color;
    int numberOfPoints;
    double maxValue;
    double minValue;
};

class DetailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DetailDialog(QWidget *parent = 0);
    DetailDialog(QWidget *parent, TransientAnalysisEntity &entity);
    ~DetailDialog();

private:
    Ui::DetailDialog *ui;
};

#endif // DETAILDIALOG_H
