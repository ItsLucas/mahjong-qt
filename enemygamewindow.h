#ifndef ENEMYGAMEWINDOW_H
#define ENEMYGAMEWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include "llk.h"

namespace Ui {
class enemyGameWindow;
}

class enemyGameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit enemyGameWindow(QWidget *parent = nullptr);
    ~enemyGameWindow();

    int Refresh(queue<pair<int, int>> wipequeue,int score);

    void InitMap(int map[20][20]);

    int getScore(){return score;};
private:
    Ui::enemyGameWindow *ui;

    QLabel ***squares;

    int map[20][20];

    int square_row;

    int square_col;

    int square_size;

    int square_num;

    int score;

    void LoadImage(int,int,int);
};

#endif // ENEMYGAMEWINDOW_H
