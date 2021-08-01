#include "mainwindow.h"

#include <QApplication>


// 處理 SDL main redefined 問題
//#undef main

#if defined(QT_NEEDS_QMAIN)
#define main qMain
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
