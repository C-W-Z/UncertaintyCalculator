#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtMath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->inputTextEdit->setPlainText("");
}

MainWindow::~MainWindow()
{
    delete ui;
}

int showD = 10;
double average = 0;
double MStandard_Deviation = 0;
double Standard_Deviation = 0;
double Type_A_uncertainty = 0;
double Type_B_uncertainty = 0;
double Type_C_uncertainty = 0;

void MainWindow::on_EnterButton_clicked()
{
    QString content = ui->inputTextEdit->toPlainText();
    QString minimumValue = ui->minimumValue->text();
    QStringList list = content.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);

    double *data = new double[list.count()];
    average = 0;
    MStandard_Deviation = 0;
    Standard_Deviation = 0;
    Type_A_uncertainty = 0;
    Type_B_uncertainty = minimumValue.toDouble() / 2 * qSqrt(3);
    Type_C_uncertainty = 0;

    for(int i=0;i<list.count();i++)
    {
        data[i] += list[i].toDouble();
        average += data[i];
    }
    average /= list.count();
    for(int i=0;i<list.count();i++)
    {
        MStandard_Deviation += pow(data[i] - average, 2);
    }
    Standard_Deviation = MStandard_Deviation / (list.count() - 1);
    MStandard_Deviation /= list.count();
    Standard_Deviation = qSqrt(Standard_Deviation);
    MStandard_Deviation = qSqrt(MStandard_Deviation);
    Type_A_uncertainty = MStandard_Deviation / qSqrt(list.count());

    Type_C_uncertainty = qSqrt(pow(Type_A_uncertainty, 2) + pow(Type_B_uncertainty, 2));

    QString str[6];
    str[0] = QString::number(average, 'd', showD);
    str[1] = QString::number(MStandard_Deviation, 'd', showD);
    str[2] = QString::number(Standard_Deviation, 'd', showD);
    str[3] = QString::number(Type_A_uncertainty, 'd', showD);
    str[4] = QString::number(Type_B_uncertainty, 'd', showD);
    str[5] = QString::number(Type_C_uncertainty, 'd', showD);
    ui->calculateResult->setPlainText("平均：            　   " + str[0] + "\n" + "母體標準差：  　 " + str[1] + "\n" + "標準差：    　       " + str[2] + "\n" + "A類不確定度：    " + str[3] + "\n" + "B類不確定度：    " + str[4] + "\n" + "C類不確定度：    " + str[5] + "\n" + "計算結果：    　   " + str[0] + " ± " +str[5]);

    delete[] data;
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    showD = arg1;
}
