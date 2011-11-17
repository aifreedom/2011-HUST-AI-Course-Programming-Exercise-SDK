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

#include "robotplayermainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QTableWidgetItem>
#include <QBrush>
#include <QPoint>
#include <QModelIndex>
#include<QMessageBox>
#include <iostream>

using namespace std;

robotPlayerMainWindow::robotPlayerMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    isRunning = false;
    robotMap = NULL;
    log.clear();

    this->setWindowTitle(tr("RobotPlayer - Hust 2011 AI Course Programming Exercise SDK"));

    mainSplitter = new QSplitter(Qt::Vertical);

    upperWidget = new QWidget();
    upperLayout = new QVBoxLayout();

    upperLayout1 = new QHBoxLayout();
    open = new QPushButton();
    open->setIcon(QIcon(":/images/open.png"));
    upperLayout1->addWidget(open);

    reset = new QPushButton();
    reset->setIcon(QIcon(":/images/reset.png"));
    upperLayout1->addWidget(reset);

    stepBack = new QPushButton();
    stepBack->setIcon(QIcon(":/images/stepBack.png"));
    upperLayout1->addWidget(stepBack);

    playOrPause = new QPushButton();
    playOrPause->setIcon(QIcon(":/images/play.png"));
    upperLayout1->addWidget(playOrPause);

    stepForward = new QPushButton();
    stepForward->setIcon(QIcon(":/images/stepForward.png"));
    upperLayout1->addWidget(stepForward);

    toEnd = new QPushButton();
    toEnd->setIcon(QIcon(":/images/toEnd.png"));
    upperLayout1->addWidget(toEnd);

    upperLayout2 = new QHBoxLayout();
    mapWidget = new mapBrowser(this);
    mapScroll = new QScrollArea();
    mapScroll->setWidget(mapWidget);
    mapScroll->viewport()->setBackgroundRole(QPalette::Dark);
    mapScroll->viewport()->setAutoFillBackground(true);
    mapScroll->setAlignment(Qt::AlignCenter);
    upperLayout2->addWidget(mapScroll);

    upperLayout->addLayout(upperLayout1);
    upperLayout->addLayout(upperLayout2);
    upperWidget->setLayout(upperLayout);

    downWidget = new QWidget();
    downLayout = new QHBoxLayout();
    historyLogTable = new QTableWidget();
    QStringList tabHeaders;
    tabHeaders << tr("��") << tr("robotλ��") << tr("��һ��")<<tr("�û��Զ���Log");
    historyLogTable->setColumnCount(3);//4);
    historyLogTable->setHorizontalHeaderLabels(tabHeaders);
    historyLogTable->setEditTriggers(QAbstractItemView::NoEditTriggers);//�������Ϊ��ֹ�༭
    historyLogTable->setSelectionBehavior(QAbstractItemView::SelectRows);//�������Ϊ����ѡ��
    historyLogTable->setSelectionMode(QAbstractItemView::SingleSelection);//�������Ϊ��ѡ������ѡ�ж��У�
    historyLogTable->verticalHeader()->setHidden(true);//����ʾ�к�
    historyLogTable->setColumnWidth(0, 100);//�����п�
    historyLogTable->setColumnWidth(1, 100);//�����п�
    historyLogTable->setColumnWidth(2, 100);//�����п�
    downLayout->addWidget(historyLogTable);
    downWidget->setLayout(downLayout);

    mainSplitter->addWidget(upperWidget);
    mainSplitter->addWidget(downWidget);

    this->setCentralWidget(mainSplitter);

    playOrPause->setEnabled(false);
    reset->setEnabled(false);
    stepBack->setEnabled(false);
    stepForward->setEnabled(false);
    toEnd->setEnabled(false);

    connect(open, SIGNAL(clicked()), this, SLOT(openButtonClicked()));
    connect(reset, SIGNAL(clicked()), this, SLOT(resetClicked()));
    connect(stepBack, SIGNAL(clicked()), this, SLOT(stepBackClicked()));
    connect(playOrPause, SIGNAL(clicked()), this, SLOT(playOrPauseClicked()));
    connect(stepForward, SIGNAL(clicked()), this, SLOT(stepForwardClicked()));
    connect(toEnd, SIGNAL(clicked()), this, SLOT(toEndClicked()));

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(timeOutOp()));
    timer->stop();
}

robotPlayerMainWindow::~robotPlayerMainWindow()
{
    timer->stop();
    delete timer;

    delete open;
    delete reset;
    delete stepBack;
    delete playOrPause;
    delete stepForward;
    delete toEnd;

    delete mapWidget;

    delete historyLogTable;

    delete upperLayout1;
    delete upperLayout2;
    delete upperLayout;
    delete downLayout;

    delete upperWidget;
    delete downWidget;

    delete mainSplitter;
}

void robotPlayerMainWindow::openButtonClicked()
{
    cout<<"open button clicked" << endl;
    QString filename = QFileDialog::getOpenFileName(
                    this,
                    tr("���ļ�"),
                    "./",
                    tr("File(*.log)")
                    );
    if(filename == NULL){
        return ;
    }

    log.clear();
    // free the original map
    if(robotMap != NULL){
        delete robotMap;
        for(int i = 0; i < historyLogTable->rowCount(); i++){
            QTableWidgetItem *temp;
            for(int j = 0; j < 3; j++){
                temp = historyLogTable->itemAt(i, j);
                if(temp != NULL)
                    delete temp;
            }
        }
    }

    // create the new map
    try{
        robotMap = new Map(filename.toStdString());
    }
    catch(exception e){
        QMessageBox box(QMessageBox::Warning, tr("Error"), tr("��ȡreplay�ļ�����"));
        box.exec();

        isRunning = false;
        playOrPause->setEnabled(false);
        reset->setEnabled(false);
        stepBack->setEnabled(false);
        stepForward->setEnabled(false);
        toEnd->setEnabled(false);
        return;
    }
    log = robotMap->GetLog();
    historyLogTable->setRowCount(log.size());
    for(unsigned int i = 0; i < log.size(); i ++){
        QTableWidgetItem *temp;
        temp = new QTableWidgetItem(QString::number(i));
        historyLogTable->setItem(i, 0, temp);

        temp = new QTableWidgetItem(QString("row:" + QString::number(log.at(i).Pos.x + 1) + "," +
                                            "col:" + QString::number(log.at(i).Pos.y + 1)));
        historyLogTable->setItem(i, 1, temp);

        temp = new QTableWidgetItem(QString(log.at(i).MoveDir == LogEntry::E ? "E":
                                                log.at(i).MoveDir == LogEntry::S ? "S":
                                                    log.at(i).MoveDir == LogEntry::W ? "W":"N"));
        historyLogTable->setItem(i, 2, temp);
    }

    //������ʾ��0��
    for(int i = 0; i < 3; i ++){
        historyLogTable->item(0, i)->setBackground(QBrush(Qt::green));
    }

    //set the map browser
    mapWidget->setMap(robotMap->GetMap());
    cout << "robot original place:" << log.at(0).Pos.x << " " << log.at(0).Pos.y << endl;
    mapWidget->setRobotPlace(log.at(0).Pos.y, log.at(0).Pos.x);//����Pos.x��ʾ�У�Pos.y��ʾ��(��0Ϊ��ʼ)

    playOrPause->setEnabled(true);
    reset->setEnabled(true);
    stepBack->setEnabled(true);
    stepForward->setEnabled(true);
    toEnd->setEnabled(true);

    repaint();
}

void robotPlayerMainWindow::resetClicked()
{
    cout << "reset button clicked" << endl;
    isRunning = false;
    playOrPause->setIcon(QIcon(":/images/play.png"));
    robotOperation(2);
}

void robotPlayerMainWindow::stepBackClicked()
{
    cout << "step back button clicked" << endl;
    robotOperation(1);
}

void robotPlayerMainWindow::playOrPauseClicked()
{
    if(isRunning){
        reset->setEnabled(true);
        stepBack->setEnabled(true);
        stepForward->setEnabled(true);
        toEnd->setEnabled(true);
        isRunning = false;
        timer->stop();
        playOrPause->setIcon(QIcon(":/images/play.png"));
    }
    else
    {
        reset->setEnabled(false);
        stepBack->setEnabled(false);
        stepForward->setEnabled(false);
        toEnd->setEnabled(false);
        isRunning = true;
        timer->start(1000);
        playOrPause->setIcon(QIcon(":/images/pause.png"));
    }
}

void robotPlayerMainWindow::stepForwardClicked()
{
    cout << "step forward button clicked" << endl;
    robotOperation(0);
}

void robotPlayerMainWindow::toEndClicked()
{
    cout << "toEnd button clicked" << endl;
    robotOperation(3);
}

void robotPlayerMainWindow::timeOutOp()
{
    if(robotMap->GetStep() < historyLogTable->rowCount() - 1){
        robotOperation(0);
    }
    else{
        if(isRunning){
            reset->setEnabled(true);
            stepBack->setEnabled(true);
            stepForward->setEnabled(true);
            toEnd->setEnabled(true);
            isRunning = false;
            timer->stop();
            playOrPause->setIcon(QIcon(":/images/play.png"));
        }
    }
}

void robotPlayerMainWindow::robotOperation(int op)
{// op = 0 next step, 1 pre step, 2 to begin, 3 to end
    if(robotMap == NULL){
        return;
    }
    int curStep;
    curStep = robotMap->GetStep();
    for(int i = 0; i < 3; i ++){
        historyLogTable->item(curStep, i)->setBackground(QBrush(Qt::white));
    }
    switch(op){
    case 0:
        robotMap->NextStep();
        break;
    case 1:
        robotMap->PreStep();
        break;
    case 2:
        robotMap->ToBegin();
        break;
    case 3:
        robotMap->ToEnd();
        break;
    default:
        ;//do nothing
    }
    curStep = robotMap->GetStep();
    historyLogTable->selectRow(curStep);
    for(int i = 0; i < 3; i ++){
        historyLogTable->item(curStep, i)->setBackground(QBrush(Qt::green));
    }
    historyLogTable->setFocus();
    mapWidget->setMap(robotMap->GetMap());
    mapWidget->setRobotPlace(log.at(curStep).Pos.y, log.at(curStep).Pos.x);
}
