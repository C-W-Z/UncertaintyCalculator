#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::Frame)
{
    ui->setupUi(this);
}

About::~About()
{
    delete ui;
}
