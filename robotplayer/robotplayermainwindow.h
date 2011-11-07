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
