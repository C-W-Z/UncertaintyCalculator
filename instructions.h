#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <QFrame>

namespace Ui {
class Instructions;
}

class Instructions : public QFrame
{
    Q_OBJECT

public:
    explicit Instructions(QWidget *parent = nullptr);
    ~Instructions();

private:
    Ui::Instructions *ui;
};

#endif // INSTRUCTIONS_H
