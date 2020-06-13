#ifndef RANKINGWINDOW_H
#define RANKINGWINDOW_H

#include <QMainWindow>
#include <startwindow.h>

namespace Ui {
class rankingwindow;
}

class rankingwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit rankingwindow(startWindow *parent);
    ~rankingwindow();

private slots:
    void on_back_clicked();

private:
    Ui::rankingwindow *ui;
    startWindow* s;
};

#endif // RANKINGWINDOW_H
