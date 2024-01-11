#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setFixedSize(371,440);
    w.setWindowTitle("測量不確定度Calculator v1.1");
    w.setWindowIcon(QIcon(":/icon_256x256.ico"));
    w.show();
    return a.exec();
}
