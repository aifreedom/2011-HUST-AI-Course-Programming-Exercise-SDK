#include <iostream>
#include <cstdio>
#include "map.hpp"
using namespace std;

Map::Map(string filename)
{
     FILE* logFile = fopen(filename.c_str(), "r");
     if (!LoadMap(logFile))
     {
          // TODO: throws an exception for wrong format of log file
          ;
     }
     fclose(logFile);
}

bool Map::LoadMap(FILE* logFile)
{
     int n, m; // height and width of the map
     int x, y; // initial pos of robot
     
     if (fscanf(logFile, "%d%d", &n, &m) != 2)
     {
          return false;
     }
     if (fscanf(logFile, "%d%d", &x, &y) != 2)
     {
          return false;
     }
     for (int i=0; i<n; i++)
     {
          char ch;
          vector<int> line;
          for (int j=0; j<m; j++)
          {
               ch = getchar();
               switch (ch)
               {
               case '.':
                    line.push_back(1);
                    break;
               case '#':
                    line.push_back(0);
                    break;
               default:
                    return false;
               }
          }
          ch = getchar();
          if (ch != '\n')
          {
               return false;
          }
          map.push_back(line);
     }
     return true;
}

// Return the map
vector<vector<int> > Map::GetMap()
{
    return map;
}

// Return current step count
int Map::GetStep()
{
    return curStep;
}

// Return all log records in a vector
vector<LogEntry> Map::GetLog()
{
     return stepList;
}

bool Map::NextStep()
{
    return true;
}

bool Map::PreStep()
{
    return true;
}

bool Map::ToBegin()
{
    return true;
}

bool Map::ToEnd()
{
    return true;
}
