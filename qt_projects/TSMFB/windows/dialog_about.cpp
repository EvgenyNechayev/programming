#include "dialog_about.h"
#include "ui_dialog_about.h"

dialogAbout::dialogAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogAbout)
{
    ui->setupUi(this);
}

dialogAbout::~dialogAbout()
{
    delete ui;
}
