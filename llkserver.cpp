#include "llkserver.h"
#include <llkmsg.h>
#include <llkrank.h>
llkserver::llkserver(int port)
{
    server = new QTcpServer();
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnectionSlot()));
    server->listen(QHostAddress::AnyIPv4, port);
    cout << "Listening on port " << port << endl;
    rank = new llkrank(true);
}

void llkserver::newConnectionSlot()
{
    curClient = server->nextPendingConnection();
    clients.append(curClient);
    cout << "Connected: " << curClient->peerAddress().toString().toStdString() << endl;
    connect(curClient, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(curClient, SIGNAL(disconnected()), this, SLOT(disconnect()));
}

void llkserver::disconnect()
{
    for (int i = 0; i < clients.length(); i++) {
        if (clients[i]->state() == QAbstractSocket::UnconnectedState) {
            clients[i]->destroyed();
            clients.removeAt(i);
            cout << "Disconnected" << endl;
        }
    }
}

void llkserver::readData()
{
    for (auto x : clients) {
        QByteArray buffer = x->readAll();
        if (buffer.isEmpty())
            continue;
        cout << x->peerAddress().toString().toStdString() << endl;
        cout << buffer.toStdString() << endl;
        llkmsg msg("any", 0);
        msg.parsemsg(buffer.toStdString());
        if (msg.type == LLK_MSG_TYPE_RANK_GET) {
            cout << "Sending rank" << endl;
            x->write(rank->getRankRaw().c_str());
            x->flush();
        } else if (msg.type == LLK_MSG_TYPE_RANK_UPDATE) {
            cout << "Issue rank update" << endl;
            rank->addrank(msg.getRank().first, msg.getRank().second);
        } else {
            for (auto y : clients) {
                y->write(buffer);
            }
        }
    }
}
