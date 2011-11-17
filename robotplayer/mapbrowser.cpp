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

#include "mapbrowser.h"

mapBrowser::mapBrowser(QWidget *parent) :
    QWidget(parent)
{
    this->parent = parent;
    map.clear();
    robot_x = robot_y = -1;
    setBackgroundRole(QPalette::Dark);

    robot = new QPixmap(":/images/robot.png");
    draw_sizex = 600;
    draw_sizey = 400;
}

QSize mapBrowser::sizeHint()const
{
        return QSize(draw_sizex, draw_sizey);
}

int mapBrowser::setMap(vector<vector<int> > mapInput)
{
    map = mapInput;
    repaint();
    return 0;
}

int mapBrowser::setRobotPlace(int x, int y)
{
    this->robot_x = x;
    this->robot_y = y;
    repaint();
    return 0;
}

void mapBrowser::paintEvent(QPaintEvent *)
{
    int start_x , start_y;//where to draw the map
    int draw_x, draw_y;//the show width and height of the map
    int rect_x, rect_y;//the size of the each small rectangle

    int rowNum, columnNum;// the num of the row and column of the map
    cout<< "repaint of mapBrowser" << endl;

    QPainter painter(this);
    painter.setBrush(Qt::yellow);
    if(map.size() == 0){
        return;//before init or newly reset , do nothing
    }
    else{
        rowNum = map.size();
        columnNum = map.at(0).size();
        cout << "columnNum:" << columnNum << endl;
        cout << "rowNum:" << rowNum << endl;
        painter.setBrush(Qt::gray);
        painter.drawRect(0, 0, draw_sizex, draw_sizey);
        if(columnNum*4 >= rowNum*6){
            start_x = 0;
            start_y = (draw_sizey - draw_sizex*rowNum/columnNum)/2;
            draw_x = draw_sizex;
            draw_y = draw_x * rowNum / columnNum;
        }
        else{
            start_x = (draw_sizex - draw_sizey*columnNum/rowNum)/2;
            start_y = 0;
            draw_y = draw_sizey;
            draw_x = draw_y * columnNum / rowNum;
        }
        rect_x = draw_x / columnNum;
        rect_y = draw_y / rowNum;

        for(int i = 0; i < rowNum; i++){
            for(int j = 0; j < columnNum; j++){
                if(map.at(i).at(j) == -2){
                    painter.setBrush(Qt::black);
                    painter.drawRect(start_x + rect_x*j, start_y + rect_y*i,
                                     rect_x - 2, rect_y - 2);
                }
                else{
                    painter.setBrush(Qt::yellow);
                    painter.drawRect(start_x + rect_x*j, start_y + rect_y*i,
                                     rect_x - 2, rect_y - 2);
                    if(map.at(i).at(j) != -1){
                        painter.setPen(Qt::black);
                        painter.drawText(start_x + rect_x*j + rect_x/2, start_y + rect_y*i + rect_y/2,
                                         QString::number(map.at(i).at(j)));
                        painter.setPen(Qt::white);
                    }
                }
            }
        }
        painter.drawPixmap(start_x + rect_x*robot_x, start_y + rect_y*robot_y, rect_x, rect_y, *robot);
        painter.setBrush(Qt::gray);
        painter.setPen(Qt::gray);
        if(columnNum*4 >= rowNum*6){
            painter.drawRect(0, draw_sizey - start_y, draw_sizex, start_y);
            painter.drawRect(0, 0, draw_sizex, start_y);
        }
        else{
            painter.drawRect(draw_sizex - start_x, 0, start_x, draw_sizey);
            painter.drawRect(0, 0, start_x, draw_sizey);
        }
    }
}
