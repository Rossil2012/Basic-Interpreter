#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QLineEdit>
#include <QTextEdit>
#include <QLayout>
#include <QLabel>
#include "program.hh"
#include "tokenizer.hh"

class InputLine : public QLineEdit
{
    Q_OBJECT
public:
    InputLine(QWidget *parent);
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

#endif // MAINWINDOW_HH
