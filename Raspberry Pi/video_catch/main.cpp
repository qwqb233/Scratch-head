/*
 * @Author: qwqb233 qwqb.zhang@gmail.com
 * @Date: 2026-01-10 15:00:30
 * @LastEditors: qwqb233 qwqb.zhang@gmail.com
 * @FilePath: \video_catch\main.cpp
 * @Description: 
 */
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
