#include "volumecontrolwindow.h"
#include "ui_volumecontrolwindow.h"
#include"startwindow.h"
#include"QPainter"
#include<QBitmap>

volumecontrolwindow::volumecontrolwindow(startWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::volumecontrolwindow)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::FramelessWindowHint);
    QBitmap bmp(this->size());

    bmp.fill();

    QPainter p(&bmp);
    p.setRenderHints(QPainter::SmoothPixmapTransform);

    p.setPen(Qt::NoPen);

    p.setBrush(Qt::black);

    p.drawRoundedRect(bmp.rect(),20,20);

    setMask(bmp);


    QPalette pl = ui->plainTextEdit->palette();

   pl.setBrush(QPalette::Base,QBrush(QColor(255,0,0,0)));

   ui->plainTextEdit->setPalette(pl);
   ui->pushButton->setFlat(true);



    /**/
    this->setWindowIcon(QIcon(":/bg/bg/ico.png"));

    /*设置滑动条*/
    //设置滑动条控件的最小值
    ui->horizontalSlider->setMinimum(0);
    //设置滑动条控件的最大值
    ui->horizontalSlider->setMaximum(100);
    //设置音量
    ui->horizontalSlider->setValue(parent->player->volume());

    s = parent;
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(setVolume(int)));

    //返回
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(back()));
    setWindowFlags(windowFlags()&~ Qt::WindowMinMaxButtonsHint);
    setWindowFlags(windowFlags()&~ Qt::WindowCloseButtonHint);
}

volumecontrolwindow::~volumecontrolwindow()
{
    delete ui;
}

void volumecontrolwindow::setVolume(int value){
    s->player->setVolume(value);


}

void volumecontrolwindow::back(){
    this->hide();
}

