#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QLineEdit>
#include <QTextEdit>

class InputLine : public QLineEdit
{
private:

public:
    InputLine(QWidget *parent);

};

class MainWindow : public QMainWindow
{
private:
    InputLine Input;
public:
    MainWindow(QWidget *parent);
};

#endif // MAINWINDOW_HH
