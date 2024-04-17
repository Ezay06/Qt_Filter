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
