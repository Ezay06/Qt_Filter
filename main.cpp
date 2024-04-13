#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}


//Baby Photoshop version 1.0



//ISSUES

/*
 *skewwing the image -80 degrees crashes the program
 *
 *return to menu button added in the save menu
 *
 *FIXED#lighten/darker wheel doesnot reset its position after exiting the filter even after loading a new image#
 *
 *
 *
*/
