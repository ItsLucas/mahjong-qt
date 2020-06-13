#include "rankingwindow.h"
#include "QPainter"
#include "llkrank.h"
#include "startwindow.h"
#include "ui_rankingwindow.h"
#include <QBitmap>
#include <bits/stdc++.h>
rankingwindow::rankingwindow(startWindow* parent)
    : QMainWindow(parent)
    , ui(new Ui::rankingwindow)
{
    ui->setupUi(this);

    this->setWindowFlag(Qt::FramelessWindowHint);

    QBitmap bmp(this->size());

    bmp.fill();

    QPainter p(&bmp);
    p.setRenderHints(QPainter::SmoothPixmapTransform);

    p.setPen(Qt::NoPen);

    p.setBrush(Qt::black);

    p.drawRoundedRect(bmp.rect(), 20, 20);

    setMask(bmp);

    s = parent;

    this->setWindowIcon(QIcon(":/bg/bg/ico.png"));

    ui->back->setFlat(true);

    QList<QLabel*> list;
    list.append(ui->one);
    list.append(ui->one_2);
    list.append(ui->one_3);
    list.append(ui->one_4);
    list.append(ui->one_5);
    list.append(ui->one_6);
    list.append(ui->one_7);
    list.append(ui->one_8);
    list.append(ui->one_9);

    llkrank* rank = new llkrank();
    //rank->addrank("test4", 50);
    vector<lrank> info = rank->getrank();

    for (auto a : info) {
        cout << a.name << "  " << a.t << endl;
    }

    int i = info.size() - 1;
    for (auto a : list) {
        a->setFont(QFont("Arial Black", 13));
//        a->setFont(QFont("华文彩云", 13));
        a->setText("");
        if (i >= 0)
            a->setText(QString::fromStdString(info[i].name) + "\t" + QString::number(info[i].t));
        i--;
    }
}

rankingwindow::~rankingwindow()
{
    delete ui;
}

void rankingwindow::on_back_clicked()
{
    this->hide();

    s->show();
}
