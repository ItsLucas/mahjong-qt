#ifndef LLKSERVER_H
#define LLKSERVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include <llk.h>
#include <llkmsg.h>
#include <llkrank.h>
class llkserver : public QObject {
    Q_OBJECT
public:
    llkserver(int port);

private:
    QTcpServer* server;
    QList<QTcpSocket*> clients;
    QTcpSocket* curClient;
    int oppomap[20][20];
    llkrank* rank;
private slots:
    void newConnectionSlot();
    void disconnect();
    void readData();
};

#endif // LLKSERVER_H
