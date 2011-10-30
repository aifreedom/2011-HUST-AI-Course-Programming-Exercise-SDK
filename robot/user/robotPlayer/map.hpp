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
     Point(int x, int y):x(x), y(y)
          {
          }
     Point()
          {
               x = -1;
               y = -1;
          }
};

class LogEntry
{
public:
     Point Pos;
     enum Direction {E, S, W, N} MoveDir;
     std::string UserData;
};

class Map
{
private:
     std::vector<std::vector<int> > map;
     std::vector<LogEntry> stepList;
     bool LoadMap(FILE* logFile);
     int curStep;
public:
     Map(std::string fileName);
     std::vector<std::vector<int> > GetMap();
     int GetStep();
     std::vector<LogEntry> GetLog();
public:
     bool NextStep();
     bool PreStep();
     bool ToBegin();
     bool ToEnd();
};

#endif // MAP_H
