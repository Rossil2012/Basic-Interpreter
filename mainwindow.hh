#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QLineEdit>
#include <QTextEdit>
#include <QLayout>
#include <QLabel>
#include "program.hh"
#include "tokenizer.hh"
#include "Console.hh"

class InputLine : public QLineEdit
{
    Q_OBJECT
public:
    InputLine(QWidget *parent = nullptr);
private slots:
    void sendCode();
signals:
    void newCodeWritten(const QString &code);
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    InputLine Input;
    QTextEdit ErrorView;
    QTextEdit ProgramView;
    Program PG;
    Tokenizer TK;
public:
    MainWindow(QWidget *parent = nullptr);
public slots:
    void parseCode(const QString &code);
};

class MainWindow2 : public QMainWindow
{
private:
    Console View;
public:
    MainWindow2(QWidget *parent = nullptr);
};

#endif // MAINWINDOW_HH
