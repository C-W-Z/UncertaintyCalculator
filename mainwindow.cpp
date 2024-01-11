#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtMath>
#include <QVector>
#include <QString>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    f = new Frame;
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
QString content;
QString minValue;
QStringList list;
QString str[7];

void MainWindow::on_EnterButton_clicked()
{
    content = ui->inputTextEdit->toPlainText();
    minValue = ui->minimumValue->text();
    list = content.split(QRegExp("\n"),Qt::SkipEmptyParts);

    QVector<double> data(list.count());
    average = 0;
    MStandard_Deviation = 0;
    Standard_Deviation = 0;
    Type_A_uncertainty = 0;
    Type_B_uncertainty = minValue.toDouble() / (2 * qSqrt(3));
    Type_C_uncertainty = 0;

    for(int i=0;i<data.size();i++)
    {
        data[i] = list[i].toDouble();
        average += data[i];
    }
    average /= data.size();
    for(int i=0;i<data.size();i++)
    {
        MStandard_Deviation += pow(data.at(i) - average, 2);
    }
    Standard_Deviation = MStandard_Deviation / (data.size() - 1);
    MStandard_Deviation /= data.size();
    Standard_Deviation = qSqrt(Standard_Deviation);
    MStandard_Deviation = qSqrt(MStandard_Deviation);
    Type_A_uncertainty = Standard_Deviation / qSqrt(data.size());
    Type_C_uncertainty = qSqrt(pow(Type_A_uncertainty, 2) + pow(Type_B_uncertainty, 2));

    str[0] = QString::number(average, 'd', showD);
    str[1] = QString::number(MStandard_Deviation, 'd', showD);
    str[2] = QString::number(Standard_Deviation, 'd', showD);
    str[3] = QString::number(Type_A_uncertainty, 'd', showD);
    str[4] = QString::number(Type_B_uncertainty, 'd', showD);
    str[5] = QString::number(Type_C_uncertainty, 'd', showD);
    str[6] = QString::number((Type_C_uncertainty / abs(average))*100, 'd', showD);
    ui->calculateResult->setPlainText("算數平均(最佳估計值)：" +str[0]+"\n母體標準差：  　             "+str[1]+"\n(樣本)標準差：    　        "+str[2]+"\nA類(隨機)不確定度：     "+str[3]+"\nB類(系統)不確定度：     "+str[4]+"\nC類(組合)不確定度：     "+str[5]+"\n最終計算結果：      　    "+str[0]+" ± "+str[5]+"\n不確定度百分比：      　"+str[0]+"(1 ± "+str[6]+"%)");

    data.clear();
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    showD = arg1;
}

void MainWindow::on_action_triggered()
{
    f->resize(264,320);
    f->setMaximumSize(720, 1000);
    f->setWindowTitle("關於");
    f->setWindowIcon(QIcon(":/icon_256x256.ico"));
    f->show();
}
