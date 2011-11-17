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

#ifndef MAPBROWSER_H
#define MAPBROWSER_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <vector>
#include <iostream>
using namespace std;

class mapBrowser : public QWidget
{
    Q_OBJECT
private:
    QWidget *parent;
    QPixmap *robot;
    int draw_sizex, draw_sizey;
    int robot_x, robot_y;

    vector<vector<int> > map;

public:
    explicit mapBrowser(QWidget *parent = 0);
    QSize sizeHint() const;
    int setMap(vector<vector<int> > mapInput);
    int setRobotPlace(int x, int y);

protected:
    void paintEvent(QPaintEvent *);

signals:

public slots:

};

#endif // MAPBROWSER_H
