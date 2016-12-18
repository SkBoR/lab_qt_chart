#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDIalog::AboutDIalog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDIalog)
{
    ui->setupUi(this);
}

AboutDIalog::~AboutDIalog()
{
    delete ui;
}
