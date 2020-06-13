#ifndef MAPBUTTON_H
#define MAPBUTTON_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QPushButton>

class mapButton: public QPushButton
{
    Q_OBJECT

public slots:
    void buttonChange();
    void setClicked(bool);
signals:
    void aButtonClicked(int, int);


public:
    int No; //按钮编号

    //在map的坐标
    int x;
    int y;

    mapButton(int, int, int);//图片编号、坐标
    void set(int No, int x, int y);
    void setTransparent();
};

#endif // MAPBUTTON_H
