#include "llkrank.h"
#include <QEventLoop>
#include <jsonwrapper.h>
#include <llkmsg.h>
using namespace std;
using namespace rapidjson;

llkrank::llkrank(bool init)
{
    if (init) {
        ranks.push_back({ string("testplayer"), 100 });
        saverank();
        ranks.clear();
    }
    ifstream in;
    in.open("ranks.json");
    std::string jsonstring((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    in.close();
    Document d;
    if (!d.Parse(jsonstring.c_str()).HasParseError()) {
        if (d.HasMember("ranks") && d["ranks"].IsArray()) {
            const Value& arr = d["ranks"];
            for (int i = 0; i < arr.Size(); i++) {
                const Value& x = arr[i];
                string s;
                int v;
                if (x.HasMember("name") && x["name"].IsString()) {
                    s = x["name"].GetString();
                }
                if (x.HasMember("speed") && x["speed"].IsInt()) {
                    v = x["speed"].GetInt();
                }
                ranks.push_back({ s, v });
            }
        }
    }
    ofstream fout;
    fout.open("test.txt");
    for (auto x : ranks) {
        fout << x.name << " " << x.t << endl;
    }
    fout.close();
}

void llkrank::parserank(string jsoncontent)
{
    stringstream ss;
    ss << jsoncontent;
    int n;
    ss >> n;
    for (int i = 1; i <= n; i++) {
        string name;
        int ti;
        ss >> name >> ti;
        ranks.push_back({ name, ti });
    }
}

llkrank::llkrank()
{
    manager = new QNetworkAccessManager();
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onResult(QNetworkReply*)));
    request.setUrl(QUrl("http://teapot.itslucas.me/query.php"));
    QEventLoop loop;
    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
    manager->get(request);
    loop.exec();
}
void llkrank::onResult(QNetworkReply* reply)
{
    if (reply->error()) {
        cout << reply->errorString().toStdString() << endl;
    } else {
        QString answer = reply->readAll();
        parserank(answer.toStdString());
    }
}
void llkrank::readSlot()
{
    string buf = client->readAll().toStdString();
    llkmsg msg("any", 0);
    msg.parsemsg(buf);
    std::string jsonstring = msg.getRankJson();
    Document d;
    if (!d.Parse(jsonstring.c_str()).HasParseError()) {
        if (d.HasMember("ranks") && d["ranks"].IsArray()) {
            const Value& arr = d["ranks"];
            for (int i = 0; i < arr.Size(); i++) {
                const Value& x = arr[i];
                string s;
                int v;
                if (x.HasMember("name") && x["name"].IsString()) {
                    s = x["name"].GetString();
                }
                if (x.HasMember("speed") && x["speed"].IsInt()) {
                    v = x["speed"].GetInt();
                }
                ranks.push_back({ s, v });
            }
        }
    }
}
void llkrank::saverank()
{
    //DEPREPCATED
}

vector<lrank> llkrank::getrank()
{
    sort(ranks.begin(), ranks.end());
    return ranks;
}

void llkrank::addrank(string name, int t)
{
    /*
    ranks.push_back({ name, t });
    llkmsg msg("any", LLK_MSG_TYPE_RANK_UPDATE);
    msg.addRank(name, t);
    client->write(msg.getmsg().c_str());
    client->flush();
    sort(ranks.begin(), ranks.end());
    */
    ranks.push_back({ name, t });
    sort(ranks.begin(), ranks.end());
    stringstream ss;
    ss << "http://teapot.itslucas.me/update.php?name=" << name << "&time=" << t;
    request.setUrl(QUrl(ss.str().c_str()));
    manager->get(request);
}

string llkrank::getRankRaw()
{
    StringBuffer buffer;
    PrettyWriter<StringBuffer> writer(buffer);
    writer.StartObject();
    writer.String("ranks");
    writer.StartArray();
    for (auto x : ranks) {
        writer.StartObject();
        writer.String("name");
        writer.String(x.name.c_str());
        writer.String("speed");
        writer.Int(x.t);
        writer.EndObject();
    }
    writer.EndArray();
    writer.EndObject();
    return string(buffer.GetString());
}
