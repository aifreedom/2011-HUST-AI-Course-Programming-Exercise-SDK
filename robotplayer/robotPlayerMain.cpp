#include <QApplication>
#include <QTextCodec>
#include "robotplayermainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale()); //支持中文显示

    robotPlayerMainWindow w;
    w.show();

    return a.exec();
}
