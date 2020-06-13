#include <QMessageBox>
#include <algorithm>
#include <iostream>
#include <llk.h>
#include <llkmsg.h>
#include <queue>
using namespace std;
llk::llk() {}
int myrandom(int i) { return std::rand() % i; }
llk::llk(int h, int w, int types, int diff, bool multi)
{
    config.h = h;
    config.w = w;
    this->multi = multi;
    config.types = types;
    config.difficulty = diff;
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            mat[i][j] = 0;
        }
    }
    uuid = QUuid::createUuid().toString().toStdString();
    if (multi) {
        client = new QTcpSocket(this);
        connect(client, SIGNAL(readyRead()), this, SLOT(readSlot()));
        client->connectToHost("gitlab.itslucas.me", 25376);
    }
    while (!wipequeue.empty())
        wipequeue.pop();
}

llk_map llk::getoppomap()
{
    return oppomap;
}
void llk::notifyStart()
{
    if (multi) {
        llkmsg msg(string("any"), LLK_MSG_TYPE_CTL);
        msg.setCommand(uuid);
        client->write(msg.getmsg().c_str());
    }
}
void llk::readSlot()
{
    QByteArray arr;
    arr = client->readAll();
    llkmsg msg(uuid, LLK_MSG_TYPE_WAIT_PARSE);
    msg.parsemsg(arr.toStdString());
    if (msg.notme)
        return;
    switch (msg.type) {
    case LLK_MSG_TYPE_MAP_INIT: {
        oppomap = msg.getmap();
        break;
    }
    case LLK_MSG_TYPE_CTL: {
        oppouuid = msg.getCommand();
        break;
    }
    case LLK_MSG_TYPE_WIPE: {
        wipequeue.push(msg.getloc());
        break;
    }
    default:
        break;
    }
}
void llk::genmap()
{
    int pc = config.h * config.w / config.types;
    if (pc % 2 != 0) {
        QMessageBox::warning(
            NULL,
            "Kernel Error",
            "Can't generate a playable map. Please make sure h*w/type is odd.");
        return;
    }
    int tmp[config.h - 2][config.w - 2] = { 0 };
    int pn = 1;
    int cur = 0;
    for (int j = 0; j < config.h - 2; j++) {
        for (int i = 0; i < config.w - 2; i++) {
            tmp[j][i] = pn;
            cur++;
            if (cur == pc) {
                pn++;
                cur = 0;
            }
        }
    }
    srand((unsigned long)time(NULL));
    std::random_shuffle((int*)tmp, (int*)tmp + (config.h - 2) * (config.w - 2));
    for (int i = 1; i < config.w - 1; i++) {
        for (int j = 1; j < config.h - 1; j++) {
            mat[j][i] = tmp[j - 1][i - 1];
        }
        printf("\n");
    }
    for (int i = 0; i < 20; i++) {
        mat[i][0] = 0;
        mat[i][config.w] = 0;
    }
    for (int i = 0; i < 20; i++) {
        mat[config.h][i] = 0;
        mat[0][i] = 0;
    }
    if (multi) {
        llkmsg msg(uuid, LLK_MSG_TYPE_MAP_INIT);
        msg.setmap(getmap());
        string s = msg.getmsg();
        client->write(s.c_str());
        client->flush();
    }
}

void llk::reset()
{
    int tmp[config.h - 2][config.w - 2] = { 0 };
    memset(tmp, 0, sizeof(tmp));
    for (int i = 1; i < config.w - 1; i++) {
        for (int j = 1; j < config.h - 1; j++) {
            tmp[j - 1][i - 1] = mat[j][i];
        }
    }
    srand((unsigned long)time(NULL));
    std::random_shuffle((int*)tmp, (int*)tmp + (config.h - 2) * (config.w - 2));
    for (int i = 1; i < config.w - 1; i++) {
        for (int j = 1; j < config.h - 1; j++) {
            mat[j][i] = tmp[j - 1][i - 1];
        }
    }
    for (int i = 0; i < 20; i++) {
        mat[i][0] = 0;
        mat[i][config.w] = 0;
    }
    for (int i = 0; i < 20; i++) {
        mat[config.h][i] = 0;
        mat[0][i] = 0;
    }
}

path llk::getpath()
{
    path p;
    while (!s.empty()) {
        p.data.push_back(s.top());
        s.pop();
    }
    return p;
}
static constexpr int NO_DIR = 0;
static constexpr int EAST = 1;
static constexpr int SOUTH = 2;
static constexpr int WEST = 3;
static constexpr int NORTH = 4;
bool hasPath = false;
void llk::dfs(int row, int col, int row2, int col2, int dir, int corners)
{
    s.push(make_pair(row, col));
    if (row == row2 && col == col2) {
        hasPath = true;
        return;
    } else {
        if (col + 1 <= config.w + 1 && (mat[row][col + 1] == 0 || mat[row2][col2] == mat[row][col + 1]) && dir != WEST && hasPath == false) {
            if (dir == EAST || dir == NO_DIR) {
                dfs(row, col + 1, row2, col2, EAST, corners);
            } else if ((dir == NORTH || dir == SOUTH) && corners <= 1) {
                dfs(row, col + 1, row2, col2, EAST, corners + 1);
            }
        } //east
        if (col - 1 >= 0 && (mat[row][col - 1] == 0 || mat[row2][col2] == mat[row][col - 1]) && dir != EAST && hasPath == false) {
            if (dir == WEST || dir == NO_DIR) {
                dfs(row, col - 1, row2, col2, WEST, corners);
            } else if ((dir == NORTH || dir == SOUTH) && corners <= 1) {
                dfs(row, col - 1, row2, col2, WEST, corners + 1);
            }
        } //west
        if (row + 1 <= config.h + 1 && (mat[row + 1][col] == 0 || mat[row2][col2] == mat[row + 1][col]) && dir != SOUTH && hasPath == false) {
            if (dir == NORTH || dir == NO_DIR) {
                dfs(row + 1, col, row2, col2, NORTH, corners);
            } else if ((dir == WEST || dir == EAST) && corners <= 1) {
                dfs(row + 1, col, row2, col2, NORTH, corners + 1);
            }
        } //north
        if (row - 1 >= 0 && (mat[row - 1][col] == 0 || mat[row2][col2] == mat[row - 1][col]) && dir != NORTH && hasPath == false) {
            if (dir == SOUTH || dir == NO_DIR) {
                dfs(row - 1, col, row2, col2, SOUTH, corners);
            } else if ((dir == WEST || dir == EAST) && corners <= 1) {
                dfs(row - 1, col, row2, col2, SOUTH, corners + 1);
            }
        } //south
        if (!hasPath)
            s.pop();
    }
}
bool llk::check(int x1, int y1, int x2, int y2)
{
    hasPath = false;
    while (!s.empty())
        s.pop();
    dfs(x1, y1, x2, y2, NO_DIR, 0);
    return hasPath && mat[x1][y1] == mat[x2][y2];
}

bool llk::wipe(int x1, int y1, int x2, int y2)
{
    mat[x1][y1] = 0;
    mat[x2][y2] = 0;
    return true;
}

llk_map llk::getmap()
{
    llk_map ret;
    for (int i = 1; i < config.w - 1; i++) {
        for (int j = 1; j < config.h - 1; j++) {
            ret.mat[j][i] = mat[j][i];
        }
    }
    return ret;
}
