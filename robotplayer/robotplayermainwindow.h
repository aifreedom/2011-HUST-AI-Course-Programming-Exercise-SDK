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

#ifndef ROBOTPLAYERMAINWINDOW_H
#define ROBOTPLAYERMAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QAbstractItemView>
#include <QHeaderView>
#include <QLabel>
#include <QIcon>
#include <QScrollArea>
#include <QString>
#include <QTimer>

#include "mapbrowser.h"
#include "map.hpp"

typedef struct record{
    char step;
    QString log;
}record;

class robotPlayerMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit robotPlayerMainWindow(QWidget *parent = 0);
    ~robotPlayerMainWindow();

private:

    bool isRunning; // If auto play, set this true, else false
    vector<LogEntry> log;
    Map *robotMap;

    QSplitter *mainSplitter;
    QWidget *upperWidget, *downWidget;
    QScrollArea *mapScroll;
    mapBrowser *mapWidget;
    QVBoxLayout *upperLayout;
    QHBoxLayout *upperLayout1, *upperLayout2, *downLayout;

    QPushButton *open, *reset, *stepBack, *playOrPause, *stepForward, *toEnd;

    QTableWidget *historyLogTable;

    QTimer *timer;
    void robotOperation(int op); // op = 0 next step, 1 pre step, 2 to begin, 3 to end

signals:

public slots:

private slots:
    void openButtonClicked();
    void resetClicked();
    void stepBackClicked();
    void playOrPauseClicked();
    void stepForwardClicked();
    void toEndClicked();
    void timeOutOp();
};

#endif // ROBOTPLAYERMAINWINDOW_H
