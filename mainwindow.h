#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <frame.h>

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

    void on_action_triggered();

private:
    Ui::MainWindow *ui;
    Frame *f;

};
#endif // MAINWINDOW_H
