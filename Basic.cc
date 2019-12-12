#include <QApplication>

#include "mainwindow.hh"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    MainWindow window(&a);
    window.show();

    return a.exec();
}
