#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class AboutDIalog;
}

class AboutDIalog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDIalog(QWidget *parent = 0);
    ~AboutDIalog();

private:
    Ui::AboutDIalog *ui;
};

#endif // ABOUTDIALOG_H
