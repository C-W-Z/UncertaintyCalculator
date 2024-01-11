#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QFont font;
    font.setFamily("Microsoft JhengHei");
    font.setPointSize(10);
    qApp->setFont(font);
    w.setFixedSize(396,533);
    w.setWindowTitle("Uncertainty Calculator v1.3");
    w.setWindowIcon(QIcon(":/icon_256x256.ico"));
    w.show();
    return a.exec();
}

/* https://www.av8n.com/physics/uncertainty-calculator.html */
