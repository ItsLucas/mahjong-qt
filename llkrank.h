#ifndef LLKRANK_H
#define LLKRANK_H
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QTcpSocket>
#include <bits/stdc++.h>
#include <rapidjson/rapidjson.h>
using namespace std;
using namespace rapidjson;

struct lrank {
    string name;
    int t;
    bool operator<(const lrank& l) const
    {
        return t < l.t;
    }
};

class llkrank : public QObject {
    Q_OBJECT
public:
    llkrank(bool init);
    void parserank(string jsoncontent);
    llkrank();
    void saverank();
    vector<lrank> getrank();
    void addrank(string name, int t);
    string getRankRaw();
    bool online = false;

private:
    vector<lrank> ranks;
    QTcpSocket* client;
    QNetworkAccessManager* manager;
    QNetworkRequest request;
private slots:
    void readSlot();
    void onResult(QNetworkReply* reply);
};

#endif // LLKRANK_H
