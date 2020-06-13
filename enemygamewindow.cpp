#include "enemygamewindow.h"
#include "ui_enemygamewindow.h"
#include"QPainter"
#include<QBitmap>

enemyGameWindow::enemyGameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::enemyGameWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/bg/bg/ico.png"));
    this->setWindowTitle("对手棋盘");
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);


    QBitmap bmp(this->size());

    bmp.fill();

    QPainter p(&bmp);
    p.setRenderHints(QPainter::SmoothPixmapTransform);

    p.setPen(Qt::NoPen);

    p.setBrush(Qt::black);

    p.drawRoundedRect(bmp.rect(),20,20);

    setMask(bmp);
}

void enemyGameWindow::LoadImage(int row, int col, int num)
{
    if(num!=0){
        //数字非0，加载相应图片
        QString res = ":/items/item/";
        res.append(QString::number(num)+ ".png");
        squares[row][col]->setStyleSheet("QLabel{border-image: url(" + res + ")}");
    }else
        squares[row][col]->setStyleSheet("");

}

void enemyGameWindow::InitMap(int m[20][20])
{
    square_row = 10, square_col = 16;
    square_size = 20;
    square_num=square_col*square_row;
    score=0;

    int startX=40,startY=50;
    //根据行列分配数组空间
    squares = new QLabel**[square_row];
    for(int i = 0; i < square_row; i++)
        squares[i] = new QLabel*[square_col];
    //生成每个方块
    for(int i=0;i<square_row;i++)
        for(int j=0;j<square_col;j++){
            squares[i][j]=new QLabel(this);
            squares[i][j]->resize(square_size,square_size);
            squares[i][j]->move(startX+j*square_size,startY+i*square_size);
            map[i+1][j+1]=m[i+1][j+1];      //地图赋值
            LoadImage(i,j,map[i+1][j+1]);
        }
}

int enemyGameWindow::Refresh(queue<pair<int, int>> wipequeue, int score)
{
    if(square_num==0)return 0;
    while(!wipequeue.empty())
    {
        pair<int,int> front=wipequeue.front();
        wipequeue.pop();
        //if(map[front.first][front.second]!=0){
            map[front.first][front.second]=0;
            square_num--;
            squares[front.first-1][front.second-1]->setStyleSheet("");
        //}
    }
    this->score=score;
    ui->label_score->setNum(this->score);
    return 1;
}

enemyGameWindow::~enemyGameWindow()
{
    delete ui;
}
