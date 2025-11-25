#include "mainwindow.h"

#include <QApplication>

std::string plik_tekstowy ;
//*******************************************************************************
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if(argc > 1)
        plik_tekstowy = argv[1];
    MainWindow w;
    w.show();
    return a.exec();
}
