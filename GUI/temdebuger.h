#ifndef TEMDEBUGER_H
#define TEMDEBUGER_H

#include <QMainWindow>

namespace Ui {
class TemDebuger;
}

class TemDebuger : public QMainWindow
{
    Q_OBJECT

public:
    explicit TemDebuger(QWidget *parent = 0);
    ~TemDebuger();
    void display(const std::string& str);


private:
    Ui::TemDebuger *ui;
};

#endif // TEMDEBUGER_H
