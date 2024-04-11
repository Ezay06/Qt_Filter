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
 * brightness filter is very slow if the slider updates the image each time it changes, solution might be to add a seperate button for changing the brightness.
 * behind bars filter is un-implemented, pressing button in menu leads to nothing .
 * wrong naming the behind bar button
*/
