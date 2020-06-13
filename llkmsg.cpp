#include "llkmsg.h"
#include <bits/stdc++.h>
#include <rapidjson/document.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/stream.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
using namespace std;
using namespace rapidjson;
llkmsg::llkmsg(string uuid, int msgtype)
{
    this->uuid = uuid;
    type = msgtype;
    memset(mat, 0, sizeof(mat));
}

void llkmsg::setloc(int x, int y)
{
    this->x = x;
    this->y = y;
}

pair<int, int> llkmsg::getloc()
{
    return make_pair(x, y);
}
void llkmsg::setmap(llk_map m)
{
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            mat[i][j] = m.mat[i][j];
        }
    }
}

llk_map llkmsg::getmap()
{
    llk_map m;
    memset(m.mat, 0, sizeof(m.mat));
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            m.mat[i][j] = mat[i][j];
        }
    }
}

string llkmsg::getmsg()
{
    StringBuffer buf;
    Writer<StringBuffer> writer(buf);
    writer.StartObject();
    writer.Key("msgtype");
    writer.Int(type);
    writer.Key("uuid");
    writer.String(uuid.c_str());
    switch (type) {
    case LLK_MSG_TYPE_WIPE: {
        writer.Key("x");
        writer.Int(x);
        writer.Key("y");
        writer.Int(y);
        break;
    }
    case LLK_MSG_TYPE_MAP_INIT: {
        writer.Key("map");
        writer.StartArray();
        for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 20; j++) {
                writer.Int(mat[i][j]);
            }
        }
        writer.EndArray();
        break;
    }
    case LLK_MSG_TYPE_CTL: {
        writer.Key("cmd");
        writer.String(cmd.c_str());
        break;
    }
    case LLK_MSG_TYPE_RANK_UPDATE: {
        writer.Key("name");
        writer.String(name.c_str());
        writer.Key("time");
        writer.Int(speed);
        break;
    }
    case LLK_MSG_TYPE_RANK_GET: {
        writer.Key("jsoncontent");
        writer.String(jsoncontent.c_str());
        break;
    }
    default:
        break;
    }
    writer.EndObject();
    return string(buf.GetString());
}

void llkmsg::parsemsg(string jsonstring)
{
    Document d;
    if (!d.Parse(jsonstring.c_str()).HasParseError()) {
        if (d.HasMember("uuid") && d["uuid"].IsObject()) {
            if (string("any") == string(d["uuid"].GetString())) {
                notme = false;
            } else if (uuid != string(d["uuid"].GetString())) {
                notme = true;
                return;
            }
        }
        if (d.HasMember("msgtype") && d["msgtype"].IsInt()) {
            type = d["msgtype"].GetInt();
            switch (type) {
            case LLK_MSG_TYPE_WIPE: {
                if (d.HasMember("x") && d["x"].IsObject()) {
                    x = d["x"].GetInt();
                    y = d["y"].GetInt();
                }
                break;
            }
            case LLK_MSG_TYPE_MAP_INIT: {
                if (d.HasMember("map") && d["map"].IsObject()) {
                    Value& arr = d["map"];
                    int pos = 0;
                    for (int i = 0; i < 20; i++) {
                        for (int j = 0; j < 20; j++) {
                            if (pos >= arr.Size())
                                break;
                            mat[i][j] = arr[pos].GetInt();
                        }
                    }
                }
                break;
            }
            case LLK_MSG_TYPE_CTL: {
                if (d.HasMember("cmd") && d["cmd"].IsString()) {
                    cmd = string(d["cmd"].GetString());
                }
            }
            case LLK_MSG_TYPE_RANK_UPDATE: {
                if (d.HasMember("name") && d["name"].IsString()) {
                    name = string(d["name"].GetString());
                }
                if (d.HasMember("time") && d["time"].IsInt()) {
                    speed = d["time"].GetInt();
                }
            }
            case LLK_MSG_TYPE_RANK_GET: {
                if (d.HasMember("jsoncontent") && d["jsoncontent"].IsString()) {
                    jsoncontent = string(d["jsoncontent"].GetString());
                }
            }
            default:
                break;
            }
        }
    }
}
void llkmsg::setCommand(string cmd)
{
    this->cmd = cmd;
}

string llkmsg::getCommand()
{
    return cmd;
}

void llkmsg::setRankJson(string content)
{
    jsoncontent = content;
}

string llkmsg::getRankJson()
{
    return jsoncontent;
}

void llkmsg::addRank(string name, int speed)
{
    this->name = name;
    this->speed = speed;
}

pair<string, int> llkmsg::getRank()
{
    return make_pair(name, speed);
}
