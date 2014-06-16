//
//  GameUtils.h
//  CocosManual
//
//  Created by Jiangy on 14-5-22.
//
//

#ifndef CocosManual_GameUtils_h
#define CocosManual_GameUtils_h

#include <iostream>

using namespace std;

inline const int random(int from, int to)
{
    static bool haveSeeds;
	if (!haveSeeds) {
		srand((unsigned) time(NULL));
		haveSeeds = true;
	}
	int randonNum = from + rand() % (to - from);
	return randonNum;
}

inline const char * ctime(long second, bool withHour = true)
{
    if (second < 0) {
        second = 0;
    }
    int hour = second / 3600;
    second %= 3600;
    int minute = second / 60;
    second %= 60;
    std::string s_time = to_string(hour);
    s_time.append(":");
    if (minute < 10) {
        s_time.append("0");
    }
    s_time.append(to_string(minute));
    s_time.append(":");
    if (second < 10) {
        s_time.append("0");
    }
    s_time.append(to_string(second));
    if (!withHour || hour == 0) {
        size_t hourPos = s_time.find(":");
        s_time = s_time.substr(hourPos + 1);
    }
    return s_time.c_str();
}

inline const char * localeTime(time_t t = 0) {
    char timeNow[20];
    time_t lt;
    struct tm * tp;
    if (t == 0) {
        lt = time(NULL);
    } else {
        lt = t;
    }
    tp = localtime(&lt);
    strftime(timeNow, 20, "%Y-%m-%d %T", tp);
    std::string time(timeNow);
    return time.c_str();
}

inline const string replace_all(string src, string oldStr, string newStr)
{
    string dst = "";
    int oldSize = oldStr.length();
    size_t index = src.find(oldStr);
    while (index != string::npos) {
        dst.append(src.substr(0, index));
        dst.append(newStr);
        src = src.substr(index + oldSize);
        index = src.find(oldStr);
    }
    dst.append(src);
    
    return dst;
}

inline const size_t find_last_pos(string src, string str)
{
    size_t tempPos = 0;
    size_t pos = src.find(str, tempPos);
    while (pos != string::npos) {
        tempPos = pos;
        pos = src.find(str, tempPos + 1);
    }
    return tempPos;
}

inline const int afterTime(time_t t = 0)
{
    return (time(NULL) + t) == time(NULL) ? t : (time(NULL) + t);
}

inline const int currTimeSeconds()
{
    return time(NULL);
}

inline const int timeDifference(time_t t = 0)
{
    return t-time(NULL);
}

#endif
