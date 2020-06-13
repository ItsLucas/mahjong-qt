#include "gamewindow.h"
#include "llkrank.h"
#include "llkserver.h"
#include "modeselectwindow.h"
#include "rankingwindow.h"
#include "startwindow.h"
#include <QApplication>
#include <fstream>
#include <llkmsg.h>
#include <string>
using namespace std;
int main(int argc, char* argv[])
{
    if (argc == 2 && strcmp("server", argv[1]) == 0) {
        QCoreApplication a(argc, argv);
        llkserver server(25376);
        return a.exec();
    } else {
        QApplication a(argc, argv);
        startWindow w;
        w.show();
        return a.exec();
    }
}
