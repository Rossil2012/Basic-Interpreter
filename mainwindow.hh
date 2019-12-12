#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QLineEdit>
#include <QTextEdit>
#include <QLayout>
#include <QLabel>
#include <QApplication>
#include "program.hh"
#include "tokenizer.hh"
#include "Console.hh"


class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Console View;
    Program PG;
    Tokenizer TK;
public:
    MainWindow(QApplication *app = nullptr, QWidget *parent = nullptr);
private slots:
    void parseCode(const QString &str);
};

#endif // MAINWINDOW_HH
