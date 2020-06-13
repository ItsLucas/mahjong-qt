#include "gamewindow.h"
#include "llkrank.h"
#include "successwindow.h"
#include "ui_gamewindow.h"
#include <QDesktopWidget>
#include <QGridLayout>
#include <QPainter>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QThread>

#include <QBitmap>
#define AUTO_SOLVE_INTERVAL 1000

gameWindow::gameWindow(modeselectwindow* parent)
    : QMainWindow(parent)
    , ui(new Ui::gameWindow)
{
    ui->setupUi(this);

    name = "";
    level = 1;
    wipedNum = 0;
    isPaused = 0;

    ui->coverLabel->setVisible(0); //暂停幕布不可见
    select = parent; //开始界面
    //设置按钮
    connect(ui->settingButton, SIGNAL(clicked()), select->s, SLOT(on_volumecontrol_clicked()));

    //得分
    score = 0;

    //初始化行列
    row = 10, col = 16;
    //已点击按钮位置保存
    pair = new Position(-1, -1);
    //初始化动态mapButton数组
    buttons = new mapButton**[row + 2];
    for (int i = 0; i < row + 2; i++)
        buttons[i] = new mapButton*[col + 2];

    this->setWindowIcon(QIcon(":/bg/bg/ico.png"));

    //mapGUI框架
    QGridLayout* layout = new QGridLayout();
    ui->widget->setLayout(layout);
    layout->setSpacing(0);

    //内核连接
    if (select->getMode() == 1)
        llkCore = new llk(row + 2, col + 2, 15, 0);
    else
        llkCore = new llk(row + 2, col + 2, 20, 0);
    llkCore->genmap();
    llk_map llkMap = llkCore->getmap();

    //    //print test
    //    for (int i = 0; i < row + 2; i++) {
    //        for (int j = 0; j < col + 2; j++) {
    //            //test
    //            printf("%d\t", llkMap.mat[i][j]);
    //        }
    //        //test
    //        printf("\n");
    //    }

    //生成map
    for (int i = 0; i < row + 2; i++)
        for (int j = 0; j < col + 2; j++) {

            if (i == 0 || j == 0 || i == row + 1 || j == col + 1) //padding
                buttons[i][j] = new mapButton(0, i, j); //编号0:不可见
            else {
                buttons[i][j] = new mapButton(llkMap.mat[i][j], i, j);
                //buttons[i][j] = new mapButton(j, i, j);
                //返回点击的按钮坐标到judge()中
                connect(buttons[i][j], SIGNAL(aButtonClicked(int, int)), this, SLOT(judge(int, int)));
            }

            layout->addWidget(buttons[i][j], i, j);
        }

    //时间进度条
    maxTime = 300;
    ui->progressBar->setMaximum(maxTime);
    ui->progressBar->setValue(maxTime);
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);

    //画板
    //QLayout *lineLayout = new QLayout();

    lm = new lineMap(ui->centralwidget, this);
    //设置画板大小和位置
    lm->setGeometry(QRect(10, 20, 691, 481));
    lm->raise();
    lm->setVisible(0);

    //多人模式
    if (select->getMode() == 2) {
        multiplayerMode();
    }

    this->setWindowFlag(Qt::FramelessWindowHint);

    QBitmap bmp(this->size());

    bmp.fill();

    QPainter p(&bmp);
    p.setRenderHints(QPainter::SmoothPixmapTransform);

    p.setPen(Qt::NoPen);

    p.setBrush(Qt::black);

    p.drawRoundedRect(bmp.rect(), 20, 20);

    setMask(bmp);

    //自动解题
    autoTimer = NULL; //初始化为NULL
    ui->autoSolveButton->setCheckable(1);
    tag = 0; //标识：一次点亮，一次消去
}

gameWindow::~gameWindow()
{
    delete ui;
}

//重新开始游戏，刷新矩阵
void gameWindow::generate()
{

    delete llkCore;
    if (level > 3) {
        llkCore = new llk(row + 2, col + 2, 20, 0);
    } else
        llkCore = new llk(row + 2, col + 2, 15, 0);
    llkCore->genmap();
    llk_map llkMap = llkCore->getmap();

    for (int i = 0; i < row + 2; i++)
        for (int j = 0; j < col + 2; j++) {

            if (i == 0 || j == 0 || i == row + 1 || j == col + 1) //padding
                buttons[i][j]->set(0, i, j); //编号0:不可见
            else {
                buttons[i][j]->setVisible(1);
                buttons[i][j]->set(llkMap.mat[i][j], i, j);
            }
        }
    ui->levelLabel_2->setText(QString::number(level));

    //进度条
    ui->progressBar->setMaximum(maxTime);
    ui->progressBar->setValue(maxTime);
    ui->coverLabel->setVisible(0);
    setButtonsEnabled(1);
    ui->autoSolveButton->setEnabled(1);
    timer->start(1000);
    wipedNum = 0;
}

//判断函数：相同种类按钮消去
void gameWindow::judge(int x, int y)
{

    if (pair->isEmpty()) { //第一次未点击
        pair->set(x, y);
        buttons[x][y]->setClicked(1);
    } else {
        if (pair->x == x && pair->y == y) { //点击相同按钮
            buttons[x][y]->setClicked(0);
            pair->reset();
        } else {

            if (llkCore->check(x, y, pair->x, pair->y)) {

                printf("wipe pair:\n(%d,%d):%d\n(%d,%d):%d\n", x, y, buttons[x][y]->No, pair->x, pair->y, buttons[pair->x][pair->y]->No);

                llkCore->wipe(x, y, pair->x, pair->y);
                wipeButtonPairs(x, y, pair->x, pair->y);
                //                buttons[x][y]->setTransparent();
                //                buttons[pair->x][pair->y]->setTransparent();
                pair->reset();

                //分数更新
                score += 10;
                ui->scoreLabel_2->setText(QString::number(score));

                //判断胜利
                wipedNum += 2;
                if (wipedNum >= col * row) {
                    //胜利
                    if (select->getMode() == 2) {
                        multiEnd('w');
                    } else
                        success();
                }

            } else {

                printf("unwiped pair:\n(%d,%d):%d\n(%d,%d):%d\n", x, y, buttons[x][y]->No, pair->x, pair->y, buttons[pair->x][pair->y]->No);

                buttons[pair->x][pair->y]->setClicked(0);
                buttons[x][y]->setClicked(0);
                pair->reset();
            }

            //print test
            llk_map llkMap = llkCore->getmap();
            for (int i = 0; i < row + 2; i++) {
                for (int j = 0; j < col + 2; j++) {
                    //test
                    printf("%d\t", llkMap.mat[i][j]);
                }
                //test
                printf("\n");
            }
        }
    }
}

// 暂停/继续游戏
void gameWindow::on_pauseButton_clicked()
{
    ui->resetButton->setEnabled(isPaused);
    ui->tipsButton->setEnabled(isPaused);
    ui->autoSolveButton->setEnabled(isPaused);

    ui->label_3->setEnabled(isPaused);
    ui->label_4->setEnabled(isPaused);
    ui->tipsTImesLabel->setEnabled(isPaused);

    if (isPaused) {
        isPaused = 0;
        ui->coverLabel->setVisible(0);
        ui->label->setText("暂停");
        timer->start(1000);
    } else {
        isPaused = 1;
        ui->coverLabel->setVisible(1);
        ui->label->setText("继续");
        timer->stop();
    }
}

void gameWindow::on_exitButton_clicked()
{

     game_over();

    select->s->player->stop();
    select->s->player->play();
    select->s->show();
    select->close();

    if (select->getMode() == 2)
        egm->close();
    this->close();
}

void gameWindow::update()
{
    if (select->getMode() == 2)
        if (!egm->Refresh(llkCore->wipequeue, score))
            multiEnd('f');

    int sec = ui->progressBar->value();

    int max = ui->progressBar->maximum();

    if (sec > 0) {
        sec--;
        ui->progressBar->setValue(sec);
        //        ui->progressBar

        //进度条变色
        if ((sec > max / 6) && (sec <= max / 2)) {
            ui->progressBar->setStyleSheet("QProgressBar {   border: 2px solid grey;  border-radius: 5px;   background-color: #FFFFFF;}"
                                           "QProgressBar::chunk {   background-color: #FFE606;   width: 20px;}"
                                           "QProgressBar {   border: 2px solid grey;  border-radius: 5px;   text-align: center;}");
        }

        if (sec < max / 6) {
            ui->progressBar->setStyleSheet("QProgressBar {   border: 2px solid grey;  border-radius: 5px;   background-color: #FFFFFF;}"
                                           "QProgressBar::chunk {   background-color: #FA4733;   width: 20px;}"
                                           "QProgressBar {   border: 2px solid grey;  border-radius: 5px;   text-align: center;}");
        }
    } else {
        //game over
        if (select->getMode() == 2) {
            if (score < egm->getScore()) {
                multiEnd('w');
            } else if (score < egm->getScore()) {
                multiEnd('f');
            } else
                multiEnd('d');
        } else
            game_over();
    }
}

//顺利通关
void gameWindow::success()
{
    //设置按钮不可点击
    setButtonsEnabled(0);
    ui->autoSolveButton->setEnabled(0);

    timer->stop();
    ui->coverLabel->setText("");
    ui->coverLabel->setVisible(1); //幕布

    //分数折算
    int leftSec = ui->progressBar->value();
    score += leftSec * level;
    ui->scoreLabel_2->setText(QString::number(score));

    successWindow* sw = new successWindow(this);
    sw->show();
}

//游戏结束
void gameWindow::game_over()
{
    ui->coverLabel->setVisible(1); //降下幕布
    setButtonsEnabled(0);
    ui->autoSolveButton->setEnabled(0);

    ui->coverLabel->setText("Total score:" + QString::number(score));
    timer->stop();

    if (autoTimer == NULL)
        insert_();

    //调用相关窗口，使用get_info()可得到字符串
}

//重置按钮
void gameWindow::on_resetButton_clicked()
{
    if (score - 100 < 0)
        return;

    llkCore->reset();
    llk_map llkMap = llkCore->getmap();

    for (int i = 1; i <= row; i++)
        for (int j = 1; j <= col; j++) {

            if (llkMap.mat[i][j] == 0)
                buttons[i][j]->setTransparent();
            else {
                buttons[i][j]->set(llkMap.mat[i][j], i, j);
            }
        }

    score -= 100;
    ui->scoreLabel_2->setNum(score);
}

//提示按钮
void gameWindow::on_tipsButton_clicked()
{
    tag = 1;
    int times = ui->tipsTImesLabel->text().toInt();
    if (times <= 0)
        return;
    else
        ui->tipsTImesLabel->setText(QString::number(--times));

    vector<std::pair<int, int>> cPair = getPair();
    judge(cPair[0].first, cPair[0].second, cPair[1].first, cPair[1].second);
}

vector<std::pair<int, int>> gameWindow::getPair()
{
    vector<std::pair<int, int>> tmp;

    llk_map m = llkCore->getmap();
    int is_find = 0;
    int a1, b1, a2, b2;

    for (int x1 = 1; x1 <= row; x1++) {
        if (is_find == 1)
            break;

        for (int y1 = 1; y1 <= col; y1++) {
            if (is_find == 1)
                break;
            if (m.mat[x1][y1] == 0)
                continue;

            for (int x2 = 1; x2 <= row; x2++) {
                if (is_find == 1)
                    break;

                for (int y2 = 1; y2 <= col; y2++) {
                    if (is_find == 1)
                        break;
                    if (m.mat[x2][y2] == 0)
                        continue;

                    if ((x1 != x2 || y1 != y2) && llkCore->check(x1, y1, x2, y2)) {
                        is_find = 1;
                        a1 = x1;
                        a2 = x2;
                        b1 = y1;
                        b2 = y2;
                    }
                }
            }
        }
    }

    tmp.push_back(std::pair<int, int>(a1, b1));
    tmp.push_back(std::pair<int, int>(a2, b2));
    pair->reset();

    return tmp;
}

void gameWindow::judge(int x1, int y1, int x2, int y2)
{
    buttons[x1][y1]->setClicked(1);
    buttons[x2][y2]->setClicked(1);

    llkCore->wipe(x1, y1, x2, y2);

    printf("find pairs:(%d,%d), (%d,%d)\n", x1, y1, x2, y2);

    wipeButtonPairs(x1, y1, x2, y2);
    //    buttons[x1][y1]->setTransparent();
    //    buttons[x2][y2]->setTransparent();

    pair->reset();

    //分数更新
    score += 10;
    ui->scoreLabel_2->setText(QString::number(score));

    //判断胜利
    wipedNum += 2;
    if (wipedNum >= col * row) {

        //自动解题
        if (autoTimer) {

            autoTimer->stop();
            delete autoTimer;
            autoTimer = NULL;
            tag = 0;

            game_over();

        } else {
            //胜利
            success();
        }
    }

    //print test
    llk_map llkMap = llkCore->getmap();
    for (int i = 0; i < row + 2; i++) {
        for (int j = 0; j < col + 2; j++) {
            //test
            printf("%d\t", llkMap.mat[i][j]);
        }
        //test
        printf("\n");
    }
}

void gameWindow::setInfo(string name, int score, int level)
{

    this->name = name;
    this->score = score;
    this->level = level;
    flashInfo();
}

void gameWindow::insert_()
{
    llkrank* rank = new llkrank();

    rank->addrank(name, score);
    rank->saverank();
    //cout << name << "  " << score << endl;
}

void gameWindow::flashInfo()
{
    ui->scoreLabel_2->setText(QString::number(score));
    ui->levelLabel_2->setText(QString::number(level));
}

string gameWindow::get_info()
{

    return name + "_" + to_string(score) + "_" + to_string(level);
}

void gameWindow::wipeButtonPairs(int x1, int y1, int x2, int y2)
{
    vector<std::pair<int, int>> points = llkCore->getpath().data;
    lm->showLine(points);
    //    //print test
    //    //part1
    //    printf("data:(all)\n");
    //    for (int i = 0; i < points.size(); i++) {
    //        printf("(%d,%d)", points[i].first, points[i].second);
    //    }
    //    printf("\n");
    ////    part2
    //    printf("data:(part)\n");
    //    for (int i = 0; i < llkCore->getpath().data.size(); i++) {
    //        printf("(%d,%d)", llkCore->getpath().data[i].first, llkCore->getpath().data[i].second);
    //    }
    //    printf("\n");
}

//多人模式
void gameWindow::multiplayerMode()
{
    frameAdjust();
    creatEnemyFrame();
}

//界面调整
void gameWindow::frameAdjust()
{
    ui->levelLabel->hide();
    ui->levelLabel_2->hide();
    ui->autoSolveButton->hide();
    int button_width;
    button_width = ui->label->width();

    ui->label_3->move(ui->label_4->x() - button_width - 5, ui->label_4->y());
    ui->resetButton->move(ui->label_4->x() - button_width - 5, ui->label_4->y());
    ui->label->move(ui->label_3->x() - button_width - 5, ui->label_3->y());
    ui->pauseButton->move(ui->label_3->x() - button_width - 5, ui->label_3->y());
}

//创建对手棋盘界面
void gameWindow::creatEnemyFrame()
{
    //生成对手界面
    egm = new enemyGameWindow;
    egm->InitMap(llkCore->getmap().mat);
    egm->show();

    //获取屏幕大小
    int screen_width, screen_height;
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect deskRect = desktopWidget->screenGeometry();
    screen_width = deskRect.width();
    screen_height = deskRect.height();

    //调整界面位置
    int newX, newY;
    newX = screen_width / 2 + this->width() / 2 - egm->width() - 30;
    newY = screen_height / 2 - this->height() / 2;
    egm->move(newX, newY);

    //刷新对手界面
    //QTimer *egmTimer=new QTimer(this);
    //connect(egmTimer,SIGNAL(timeout()),egm,SLOT(refreshEGM()));
    //egmTimer->start(1000);
}

void gameWindow::multiEnd(char ch)
{
    //设置按钮不可点击
    setButtonsEnabled(false);
    timer->stop();

    QString info;
    switch (ch) {
    case 'w':
        info = "你赢了xD";
        break;
    case 'f':
        info = "你输了:(";
        break;
    case 'd':
        info = "平局:>";
        break;
    }
    ui->coverLabel->setText(info);
    ui->coverLabel->setVisible(1); //幕布

    egm->hide();
}

//自动解题
void gameWindow::autoSolve()
{
    vector<std::pair<int, int>> cPair;
    cPair = getPair();
    judge(cPair[0].first, cPair[0].second, cPair[1].first, cPair[1].second);
}

void gameWindow::on_autoSolveButton_clicked(bool checked)
{

    if (checked) {
        if (autoTimer == NULL) {
            autoTimer = new QTimer();

            connect(autoTimer, SIGNAL(timeout()), this, SLOT(autoSolve()));
            autoTimer->start(AUTO_SOLVE_INTERVAL);

        } else {

            autoTimer->start();
        }

        setButtonsEnabled(0);
    } else {
        autoTimer->stop();
        setButtonsEnabled(1);
    }
}

void gameWindow::on_pushButton_clicked()
{
    if (select->getMode() == 2)
        multiEnd('w');
    else
        success();
}

void gameWindow::on_pushButton_2_clicked()
{
    if (select->getMode() == 2)
        multiEnd('f');
    else
        game_over();
}

void gameWindow::on_settingButton_clicked()
{
    isPaused = 0;
    on_pauseButton_clicked();
    //ui->autoSolveButton->setChecked(1);
    on_autoSolveButton_clicked(0);
}

void gameWindow::setButtonsEnabled(bool state)
{

    //设置按钮不可点击
    ui->pauseButton->setEnabled(state);
    ui->resetButton->setEnabled(state);
    ui->tipsButton->setEnabled(state);
    ui->settingButton->setEnabled(state);
    //ui->autoSolveButton->setEnabled(state);

    ui->label->setEnabled(state);
    ui->label_3->setEnabled(state);
    ui->label_4->setEnabled(state);
    ui->label_7->setEnabled(state);
    ui->tipsTImesLabel->setEnabled(state);
}

//**********************************************
//画线画板类
lineMap::lineMap(QWidget* widget, gameWindow* gw)
    : QWidget(widget)
{
    g = gw;
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
}
lineMap::~lineMap() {}

void lineMap::paintEvent(QPaintEvent* event)
{
    paint = new QPainter(this);

    paint->setPen(QPen(Qt::red, 7));

    QSize size = g->buttons[1][1]->size();

    if (points.empty()) {
        paint->end();
        return;
    }

    for (int i = 0; i < points.size() - 1; i++) {

        printf("path:");
        auto tmp = points[i], tmp2 = points[i + 1];
        printf("(%d, %d)(%d, %d)", tmp.first, tmp.second, tmp2.first, tmp2.second);
        printf("\n");

        QPoint p1, p2;

        // 最右一行路径修正
        if (points[i].second > 17) {
            p1 = g->buttons[points[i].first][17]->pos();
        } else
            p1 = g->buttons[points[i].first][points[i].second]->pos();

        if (points[i + 1].second > 17) {
            p2 = g->buttons[points[i + 1].first][17]->pos();
        } else
            p2 = g->buttons[points[i + 1].first][points[i + 1].second]->pos();

        //        p1 = g->buttons[points[i].first][points[i].second]->pos();
        //        p2 = g->buttons[points[i + 1].first][points[i + 1].second]->pos();

        printf("p1(%d, %d)\np2:(%d, %d)\n", p1.x(), p1.y(), p2.x(), p2.y());

        //画线位置修正
        p1.setX(p1.x() + size.width() / 2);
        p1.setY(p1.y() + size.height() / 2);

        p2.setX(p2.x() + size.width() / 2);
        p2.setY(p2.y() + size.height() / 2);

        //        //边框线修正
        if (points[i].second >= 17 || points[i].second <= 0)
            p1.setY(p1.y() - 6);

        if (points[i + 1].second >= 17 || points[i + 1].second <= 0)
            p2.setY(p2.y() - 6);

        paint->drawLine(p1, p2);
    }

    printf("***********************************");

    //paint->drawLine(0,0, 200,200);
    paint->end();
}

void lineMap::showLine(vector<pair<int, int>> p)
{
    //清空内存
    vector<pair<int, int>>().swap(points);

    points = p;

    //    for(int i = 0; i < p.size(); i++) {
    //        printf("showline:\n(%d,%d)",p[i].first, p[i].second);
    //    }
    //    printf("\n");

    setVisible(1);
    timer->start(AUTO_SOLVE_INTERVAL - 600);
}

void lineMap::update()
{

    //测试
    //    printf("update:\n");
    //test1
    //    if(points.empty()) printf("point is empty.\n");
    //    for(int i = 0; i < points.size(); i++) {
    //        printf("(%d,%d)",points[i].first, points[i].second);
    //    }
    //test2
    //    if(g->llkCore->getpath().data.empty()) printf("point is empty.\n");
    //    for(int i = 0; i < g->llkCore->getpath().data.size(); i++) {
    //        printf("(%d,%d)",g->llkCore->getpath().data[i].first, g->llkCore->getpath().data[i].second);
    //    }
    //    printf("\n");

    //    points = g->llkCore->getpath().data;

    //与画线一起消失
    g->buttons[points[0].first][points[0].second]->setTransparent();
    g->buttons[points[points.size() - 1].first][points[points.size() - 1].second]->setTransparent();

    setVisible(0);
    timer->stop();
}
