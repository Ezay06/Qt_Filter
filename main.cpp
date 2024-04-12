#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}


/*issues till now:
 * merge filter is incomplete, still need to develop a window to accept a second image.
 * Skewing filter is half implemented, still need to develop a window to accept user input.
 * behind bars filter is un-implemented, pressing button in menu leads to nothing .
 * wrong naming the behind bar button
*/
