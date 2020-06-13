#ifndef VOLUMECONTROLWINDOW_H
#define VOLUMECONTROLWINDOW_H

#include <QMainWindow>
#include <startwindow.h>

namespace Ui {
class volumecontrolwindow;
}

class volumecontrolwindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit volumecontrolwindow(startWindow *parent);
    ~volumecontrolwindow();

public slots:
     void setVolume(int value);
     void back();

private:
    Ui::volumecontrolwindow *ui;
    startWindow* s;//
};

#endif // VOLUMECONTROLWINDOW_H
