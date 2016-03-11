#ifndef DIALOG_ABOUT_H
#define DIALOG_ABOUT_H

#include <QDialog>

namespace Ui {
    class dialogAbout;
}

class dialogAbout : public QDialog
{
    Q_OBJECT

public:
    explicit dialogAbout(QWidget *parent = 0);
    ~dialogAbout();

private:
    Ui::dialogAbout *ui;
};

#endif // DIALOG_ABOUT_H
