#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>

class Point
{
public:
     int x;
     int y;
public:
     Point(int x, int y):x(x), y(y) {}
     Point() {Point(0, 0);}
};

class LogEntry
{
public:
     Point Pos;
     enum Direction {E, S, W, N} MoveDir;
     //std::string UserData;
};

class Map
{
public:
     Map(std::string fileName);
     std::vector<std::vector<int> > GetMap();
     std::pair<int, int> GetMapSize();
     int GetStep();
     std::vector<LogEntry> GetLog();
     bool ValidPos(Point pos);
     bool NextStep();
     bool PreStep();
     bool ToBegin();
     bool ToEnd();
private:
     std::vector<std::vector<int> > map;
     std::vector<LogEntry> stepList;
     bool LoadMap(FILE* logFile);
     bool LoadSteps(FILE* logFile);
     Point MovePos(Point pos, LogEntry::Direction dir);
     void GetLinefeed(FILE* logFile);
     int curStep;
     Point startPos;
};

#endif // MAP_H
