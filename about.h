#ifndef ABOUT_H
#define ABOUT_H

#include <QFrame>

QT_BEGIN_NAMESPACE
namespace Ui { class Frame; }
QT_END_NAMESPACE

class About : public QFrame
{
    Q_OBJECT

public:
    About(QWidget *parent = nullptr);
    ~About();

private slots:

private:
    Ui::Frame *ui;
};

#endif // ABOUT_H
