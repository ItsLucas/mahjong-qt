#ifndef SUCCESSWINDOW_H
#define SUCCESSWINDOW_H

#include <QDialog>
#include "gamewindow.h"

namespace Ui {
class successWindow;
}

class successWindow : public QDialog
{
    Q_OBJECT

public:
    explicit successWindow(gameWindow *parent);
    ~successWindow();

    gameWindow *g;


private slots:
    void on_pushButton_clicked();

private:
    Ui::successWindow *ui;

};

#endif // SUCCESSWINDOW_H
