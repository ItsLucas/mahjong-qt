#ifndef LLK_H
#define LLK_H
#include <QTcpSocket>
#include <bits/stdc++.h>
#include <llkmsg.h>
using namespace std;
#define LLK_DIFF_SIMPLE 1016
#define LLK_DIFF_MEDIUM 1017
#define LLK_DIFF_HARD 1018

struct llk_config {
    int h;
    int w;
    int types;
    int difficulty;
};
struct data {
    int x, y, dir, turn;
    bool operator<(const data& c) const
    {
        return c.turn < turn;
    }
};

struct path {
    vector<pair<int, int>> data;
};

class llk : public QObject {
    Q_OBJECT
public:
    llk();
    llk(int h, int w, int types, int diff, bool multi = false);
    void genmap();
    bool check(int x1, int y1, int x2, int y2);
    llk_map getmap();
    bool wipe(int x1, int y1, int x2, int y2);
    path getpath();
    void reset();
    llk_map getoppomap();
    queue<pair<int, int>> wipequeue;
    bool multiReady = false;
    void notifyStart();

private:
    llk_config config;
    int mat[20][20];
    bool checkh(int x1, int y1, int x2, int y2);
    bool checkv(int x1, int y1, int x2, int y2);
    bool checkzero(int x1, int y1, int x2, int y2);
    bool checkone(int x1, int y1, int x2, int y2);
    bool checktwo(int x1, int y1, int x2, int y2);
    bool judge();
    void push();
    void dfs(int row, int col, int row2, int col2, int dir, int corners);
    bool bfs(int x1, int yy1, int x2, int y2);
    void getp(int x, int y);
    path p;
    string uuid;
    string oppouuid;
    QTcpSocket* client;
    bool multi = false;
    stack<pair<int, int>> s;

    llk_map oppomap;
private slots:
    void readSlot();
};

#endif // LLK_H
