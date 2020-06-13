#ifndef MODESELECTWINDOW_H
#define MODESELECTWINDOW_H

#include <QMainWindow>
#include <startwindow.h>

namespace Ui {
class modeselectwindow;
}

class modeselectwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit modeselectwindow(/*QWidget *parent = nullptr, */startWindow *parent);
    ~modeselectwindow();

    startWindow* s;

    int getMode(){return mode;};
private slots:
    void on_SimpleMode_clicked();


    void on_MultiMode_clicked();

private:
    Ui::modeselectwindow *ui;

    int mode;


};

#endif // MODESELECTWINDOW_H
