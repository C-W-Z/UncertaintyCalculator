#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <about.h>
#include <instructions.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_EnterButton_clicked();

    void on_spinBox_valueChanged(int arg1);

    void on_about_triggered();

    void on_instructions_triggered();

    void on_Scientific_notation_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
    About *f1;
    Instructions *f2;
};
#endif // MAINWINDOW_H
