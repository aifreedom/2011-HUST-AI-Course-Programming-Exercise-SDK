#include <iostream>
#include <cstdio>
#include "map.hpp"
using namespace std;

//const int bufSize = 10000;

Map::Map(string filename)
{
     FILE* logFile = fopen(filename.c_str(), "r");
     curStep = 0;
     // fprintf(stderr, "before load map\n");
     if (!LoadMap(logFile))
     {
          fprintf(stderr, "wrong map\n");
          // TODO: throws an exception for wrong format of log file
          throw exception();
     }
     // fprintf(stderr, "after load map\n");
     // fprintf(stderr, "before load step\n");
     if (!LoadSteps(logFile))
     {
          fprintf(stderr, "wrong log\n");
          // TODO: throws an exception for wrong format of log file
          throw exception();
     }
     // fprintf(stderr, "after load step\n");
     fclose(logFile);
}

// Load map from logFile
// Return value: true -- success, false -- failure
bool Map::LoadMap(FILE* logFile)
{
     int n, m; // height and width of the map
     
     if (fscanf(logFile, "%d%d", &n, &m) != 2) 
     {
          // map size wrong format
          return false;
     }
     if (fscanf(logFile, "%d%d", &startPos.x, &startPos.y) != 2)
     {
          // start pos wrong format
          return false;
     }

     // the upper-left corner of the map is (1, 1)
     startPos.x--;
     startPos.y--;
     
     GetLinefeed(logFile);

     if (n < 0 || m < 0)
     {
          return false;
     }
     
     for (int i=0; i<n; i++)
     {
          char ch;
          vector<int> line;
          for (int j=0; j<m; j++)
          {
               ch = fgetc(logFile);
               switch (ch)
               {
               case '.':
                    // empty tile
                    line.push_back(-1);
                    break;
               case '#':
                    // obstacle tile
                    line.push_back(-2);
                    break;
               default:
                    // wrong character in map
                    return false;
               }
          }

          GetLinefeed(logFile);
          map.push_back(line);
     }

     // if start pos not in map
     if (!ValidPos(startPos))
     {
          return false;
     }

     // if start pos is not a valid tile
     if (map[startPos.x][startPos.y] == -2)
     {
          return false;
     }

     map[startPos.x][startPos.y] = 0;

     return true;
}

bool Map::LoadSteps(FILE* logFile)
{
     int stepCount; // total steps
     Point pos = startPos;
     //char userData[bufSize];
     char dir;
     
     if (fscanf(logFile, "%d", &stepCount) != 1)
     {
          // TODO: throws an exception for wrong format of log file
          throw exception();
     }
     GetLinefeed(logFile);
     
     for (int i=0; i<stepCount; i++)
     {
          LogEntry step;
          step.Pos = pos;
          dir = fgetc(logFile);
          switch (dir)
          {
          case 'E':
               step.MoveDir = LogEntry::E;
               pos = MovePos(pos, LogEntry::E);
               break;
          case 'S':
               step.MoveDir = LogEntry::S;
               pos = MovePos(pos, LogEntry::S);
               break;
          case 'W':
               step.MoveDir = LogEntry::W;
               pos = MovePos(pos, LogEntry::W);
               break;
          case 'N':
               step.MoveDir = LogEntry::N;
               pos = MovePos(pos, LogEntry::N);
               break;
          default:
               // wrong format
               return false;
          }
          GetLinefeed(logFile);
          //fgets(userData, bufSize, logFile);
          //step.UserData = userData;
          stepList.push_back(step);
     }
     return true;
}

// Return: map in a vector<vector<int> >
vector<vector<int> > Map::GetMap()
{
     return map;
}

pair<int, int> Map::GetMapSize()
{
     return pair<int, int>(map.size(), map[0].size());
}

// Return: current step count
int Map::GetStep()
{
     return curStep;
}

// Return: all log records in a vector
vector<LogEntry> Map::GetLog()
{
     return stepList;
}

bool Map::NextStep()
{
     if ((unsigned)curStep < stepList.size()-1)
     {
          curStep++;
          Point pos = stepList[curStep].Pos;
          map[pos.x][pos.y] = curStep;
          return true;
     }
     else
     {
          return false;
     }
}

bool Map::PreStep()
{
     if (curStep > 0)
     {
          Point pos = stepList[curStep].Pos;
          map[pos.x][pos.y] = -1;
          curStep--;
          return true;
     }
     else
     {
          return false;
     }
}

bool Map::ToBegin()
{
     while (PreStep())
     {
     }
     return true;
}

bool Map::ToEnd()
{
     while (NextStep())
     {
     }
     return true;
}


// Move the pos along the dir
// Return: the point after movement
Point Map::MovePos(Point pos, LogEntry::Direction dir)
{
     Point newPos = pos;
     switch (dir)
     {
     case LogEntry::E:
          newPos.y++;
          break;
     case LogEntry::S:
          newPos.x++;
          break;
     case LogEntry::W:
          newPos.y--;
          break;
     case LogEntry::N:
          newPos.x--;
          break;
     default:
          break;
     }
     if (ValidPos(newPos))
     {
          return newPos;
     }
     else
     {
          return pos;
     }
}

// Validate if the pos has a valid coordinate and is not an obstacle
// tile
// Return: ture for valid, false for invalid
bool Map::ValidPos(Point pos)
{
     pair<int, int> size = GetMapSize();
     if (pos.x < 0 || pos.x >= size.first ||
         pos.y < 0 || pos.y >= size.second)
     {
          return false;
     }
     else
     {
          if (map[pos.x][pos.y] >= -1)
          {
               return true;
          }
          else
          {
               return false;
          }
     }
}

void Map::GetLinefeed(FILE* logFile)
{
     // the last character of a line should be a '\n'
     char ch = fgetc(logFile);
     if (ch != '\n')
     {
          // if not '\n', the format is wrong
          // TODO: throws an exception
          throw exception();
     }
}
