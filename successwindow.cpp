#include "successwindow.h"
#include "ui_successwindow.h"

successWindow::successWindow(gameWindow *parent) :
    QDialog(parent),
    ui(new Ui::successWindow)
{
    ui->setupUi(this);

    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    g = parent;

    ui->pushButton->setFlat(true);



}

successWindow::~successWindow()
{
    delete ui;
}

void successWindow::on_pushButton_clicked()
{

    g->level++;//关数+1
    g->maxTime -= 20;//秒数-20

    g->generate();
    close();

}
