/*insert_()函数在game_over()尾部调用，insert_()函数用于写入玩家信息（addrank），也就是名字name加分数score
 * 玩家退出游戏后点击排行榜，排行榜new出来之后就显示排行榜信息，排行榜类的构造器里面写getrank；
*/
#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include "mapbutton.h"
#include "llk.h"
#include "startwindow.h"
#include "modeselectwindow.h"
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QTimer>
#include <QGridLayout>
#include "enemygamewindow.h"
namespace Ui {
class gameWindow;
}

class lineMap;

class Position {
public:
    int x;
    int y;
    Position(int x1, int y1): x(x1), y(y1){}
    void set(int x1, int y1) {x = x1; y = y1;}
    void reset(){x = y = -1;}
    int isEmpty(){if(x == -1 || y == -1) return 1; else return 0;}
};


class gameWindow : public QMainWindow
{
    Q_OBJECT

public:

    //mapButton *buttons[10][16];
    mapButton ***buttons;
    Position* pair;
    QGridLayout *layout;
    //map的长宽
    int row;
    int col;
    //内核
    llk *llkCore;
    //倒计时
    QTimer *timer;


    //开始界面，调节设置
    modeselectwindow *select;

    //画线widget
    lineMap *lm;

    //自动解题
    QTimer *autoTimer;
    //QThread *extraThread;
    int tag;

    //****************
    //info
    string name; //玩家名字
    int level;  //关卡数
    //得分
    int score;
    //游戏时长
    int maxTime;
    int wipedNum;//消去按钮的数目
    //游戏模式
    int mode;

    explicit gameWindow(modeselectwindow *parent);
    ~gameWindow();

    void setInfo(string name, int score, int level);
    void flashInfo();

    string get_info();
//    void database_connector();//连接数据库
//    void create_table();//生成一个表
    void insert_();//插入数据


    void generate();
    void wipeButtonPairs(int x1, int y1, int x2, int y2);//按钮连线且消失

    //提示功能
    vector<std::pair<int,int>> getPair();//得到一对待消去的坐标
    void judge(int, int, int, int);



private slots:
    void update();//进度条更新
    void autoSolve();//自动解题
    void judge(int x, int y);//judge the state of two keys and execute the relevant operation

    void on_pauseButton_clicked();

    void on_exitButton_clicked();

    void on_resetButton_clicked();

    void on_tipsButton_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_settingButton_clicked();

    void on_autoSolveButton_clicked(bool checked);

private:
    Ui::gameWindow *ui;
    void success();
    void game_over();

    int isPaused;

    enemyGameWindow *egm;
    int enemyMap[20][20];
    int enemyScore;
    queue<std::pair<int,int>> enemyWipequeue;

    void multiplayerMode();
    void frameAdjust();
    void creatEnemyFrame();
    void multiEnd(char);
    void setButtonsEnabled(bool state);
};


//*********************************
//画线图层类

class lineMap: public QWidget
{
    Q_OBJECT

public:

    vector<pair<int,int>> points;//装线的坐标

    lineMap(QWidget*, gameWindow*);
    ~lineMap();

    gameWindow* g;

    QPainter* paint;
    void paintEvent(QPaintEvent *event);
    QTimer* timer;//显示1s

    void showLine(vector<pair<int,int>>);

public slots:
    void update();


};



#endif // GAMEWINDOW_H
