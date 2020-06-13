#include "modeselectwindow.h"
#include "ui_modeselectwindow.h"
#include"gamewindow.h"
#include<QMessageBox>
#include"QPainter"
#include<QBitmap>

modeselectwindow::modeselectwindow(startWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::modeselectwindow)
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

    this->setWindowIcon(QIcon(":/bg/bg/ico.png"));
    s = parent;

    /*设置背景*/
    setAutoFillBackground(true);
    QPalette pal = this->palette();

    pal.setBrush(backgroundRole(), QPixmap(":/bg/bg/bg.jpg"));
    setPalette(pal);

    ui->SimpleMode->setFlat(true);
    ui->MultiMode->setFlat(true);
//    ui->HardMode->setFlat(true);


}

modeselectwindow::~modeselectwindow()
{
    delete ui;
}

void modeselectwindow::on_SimpleMode_clicked()
{
    if(ui->lineEdit->text() == ""){

        QMessageBox::about(this, "oops", "您的昵称不能为空哦");
        return;

    }

    mode=1;
    s->player->stop();
    s->player->play();
    s->hide();
    gameWindow* w = new gameWindow(this);
    w->show();

    w->name = ui->lineEdit->text().toStdString();
    this->hide();

}


void modeselectwindow::on_MultiMode_clicked()
{

//    if(ui->lineEdit->text() == ""){

//        QMessageBox::about(this, "oops", "您的昵称不能为空哦");
//        return;

//    }

//    mode=2;
//    gameWindow* w = new gameWindow(this);
//    w->show();

//    w->name = ui->lineEdit->text().toStdString();
//    this->hide();
     s->show();
     close();

}
