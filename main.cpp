/* This is an image editing program created by 3 students as a part of the winged dragon competition supervised by Dr. Mohammed El-ramly at FCAI CU
 * The contributors to the application: Ziad Sabri Mansour Awad Omar - ID:20230155 - Section 8 - Did the black and white, crop, resize, flip, purple, and old tv filters.
 *                                      Abdelrahman Mohammed Abdelgawad - ID:20230216 - Section 8 - Did the invert, rotate, frame, blur, wano, oil painting filters.
 *                                      Ahmed Ibrahim Ahmed Mohammed - ID:20230004 - Section 8 - Did the greyscale, merge, darken and lighten, detect edges, infrared, skew, ball, behind bars, pixel, decorate, and drop of water filters.
*/







#include "mainwindow.h"
#include <QApplication>

#include <QFileInfo>


QString getMainCppPath()
{
    // __FILE__ is a predefined macro that expands to the name of the current source file
    return QFileInfo(__FILE__).absolutePath();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;


    QString mainCppPath = getMainCppPath()+"/SyNet.qss";

    //The theme
    QFile styleSheetFile(mainCppPath);
    styleSheetFile.open(QFile::ReadOnly);

    QString styleSheet = QLatin1String(styleSheetFile.readAll());
    a.setStyleSheet(styleSheet);


    w.show();
    return a.exec();
}


//Baby Photoshop version 1.0



//ISSUES
