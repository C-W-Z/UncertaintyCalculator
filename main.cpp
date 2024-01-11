#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(371,425);
    w.setWindowTitle("測量不確定度計算機");
    w.setWindowIcon(QIcon(":/icon.ico"));
    w.show();
    return a.exec();
}
