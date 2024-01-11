#ifndef FRAME_H
#define FRAME_H

#include <QFrame>

QT_BEGIN_NAMESPACE
namespace Ui { class Frame; }
QT_END_NAMESPACE

class Frame : public QFrame
{
    Q_OBJECT

public:
    Frame(QWidget *parent = nullptr);
    ~Frame();

private slots:

private:
    Ui::Frame *ui;

};

#endif // FRAME_H
