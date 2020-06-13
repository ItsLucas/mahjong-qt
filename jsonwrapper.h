#ifndef JSONWRAPPER_H
#define JSONWRAPPER_H
#include <bits/stdc++.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/stream.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

using namespace std;
using namespace rapidjson;

Value string2Val(string s, rapidjson::Document::AllocatorType& allocator)
{
    char buffer[s.size() + 5];
    int len = sprintf(buffer, "%s", s.c_str());
    Value ret(kStringType);
    ret.SetString(buffer, len, allocator);
}

#endif // JSONWRAPPER_H
