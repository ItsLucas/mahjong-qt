#include "startwindow.h"
#include "ui_startwindow.h"
#include"modeselectwindow.h"
#include"rankingwindow.h"
#include"volumecontrolwindow.h"
#include "QGraphicsDropShadowEffect"
#include"QPainter"
#include<QBitmap>

startWindow::startWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::startWindow)
{
    ui->setupUi(this);

    setWindowFlag(Qt::FramelessWindowHint);

    QBitmap bmp(this->size());

    bmp.fill();

    QPainter p(&bmp);
    p.setRenderHints(QPainter::SmoothPixmapTransform);

    p.setPen(Qt::NoPen);

    p.setBrush(Qt::black);

    p.drawRoundedRect(bmp.rect(),20,20);

    setMask(bmp);

    /**/
    this->setWindowIcon(QIcon(":/bg/bg/ico.png"));
    /*设置背景*/
    setAutoFillBackground(true);
    QPalette pal = this->palette();

    pal.setBrush(backgroundRole(), QPixmap(":/bg/bg/backg.jpg"));
    setPalette(pal);
    /*设置按钮透明*/
    ui->gameMode->setFlat(true);
    ui->startGame->setFlat(true);
    ui->setting->setFlat(true);
    ui->volumecontrol->setFlat(true);


    /*添加背景音乐*/
    QMediaPlaylist* playlist = new QMediaPlaylist;
    playlist->addMedia(QUrl("qrc:/BGM/BGM/bgm.wav"));

    playlist->setCurrentIndex(1);
    playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);//循环播放


    player = new QMediaPlayer;
    player->setPlaylist(playlist);
    player->setVolume(60);
    player->play();



}

startWindow::~startWindow()
{
    delete ui;
}


void startWindow::on_gameMode_clicked()
{


    modeselectwindow* w = new modeselectwindow(this);


    w->show();

    this->hide();
}

void startWindow::on_startGame_clicked()//排行榜按钮
{


    rankingwindow* w = new rankingwindow(this);
    w->show();

    this->hide();

}

void startWindow::on_setting_clicked()//退出游戏按钮
{

    exit(0);

}

void startWindow::on_volumecontrol_clicked()
{

    volumecontrolwindow * w = new volumecontrolwindow(this);
    w->show();


}

