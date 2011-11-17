// Huazhong University of Science and Technology
// 2011 AI Course Programming Exercise SDK - RobotPlayer
// Author: Jian Gong<gongjian1990@gmail.com> Song Xie<mail@xiesong.me>
// Date: November 10, 2011
//
// This file is part of HUST AI Course Programming Exercise SKD.
//
// HUST AI Course Programming Exercise SKD is free software: you can
// redistribute it and/or modify it under the terms of the GNU General
// Public License as published by the Free Software Foundation, either
// version 3 of the License, or (at your option) any later version.
// HUST AI Course Programming Exercise SKD is distributed in the hope
// that it will be useful, but WITHOUT ANY WARRANTY; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE. See the GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with HUST AI Course Programming Exercise SKD. If not, see
// <http://www.gnu.org/licenses/>.

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
