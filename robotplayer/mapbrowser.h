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
