#ifndef LLKMSG_H
#define LLKMSG_H
#include <QTcpSocket>
#include <QUuid>
#include <bits/stdc++.h>

using namespace std;
#define LLK_MSG_TYPE_WIPE 13567
#define LLK_MSG_TYPE_MAP_INIT 13568
#define LLK_MSG_TYPE_CTL 13569
#define LLK_MSG_TYPE_WAIT_PARSE 13570
#define LLK_MSG_TYPE_RANK_UPDATE 13571
#define LLK_MSG_TYPE_RANK_GET 13572
struct llk_map {
    int mat[20][20];
};
class llkmsg {
public:
    llkmsg(string uuid, int msgtype);
    void setloc(int x, int y);
    void setmap(llk_map m);
    llk_map getmap();
    pair<int, int> getloc();
    void parsemsg(string jsonstring);
    string getmsg();
    void setCommand(string cmd);
    bool notme = false;
    int type;
    string getCommand();
    string getRankJson();
    void setRankJson(string content);
    void addRank(string name, int speed);
    pair<string, int> getRank();

private:
    int x;
    int y;
    int mat[20][20];
    string uuid;
    string cmd;
    string jsoncontent;
    string name;
    int speed;
};

#endif // LLKMSG_H
