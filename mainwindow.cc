#include "mainwindow.hh"

MainWindow::MainWindow(QApplication *app, QWidget *parent)
    : QMainWindow(parent), View(this), TK(&PG)
{
    connect(&TK, &Tokenizer::print, &View, &Console::write);
    connect(&TK, &Tokenizer::exit_interpreter, &View, &Console::Exit_interpreter);
    connect(&View, &Console::exit_interpreter, app, &QApplication::quit);
    connect(&View, &Console::newLineWritten, this, &MainWindow::parseCode);
    connect(&PG, &Program::changeState, &View, &Console::changeState);
    connect(&PG, &Program::input, &View, &Console::input);
    connect(&PG, &Program::print, &View, &Console::write);
    setCentralWidget(&View);
    layout()->setMargin(0);

    setFixedSize(600, 600);
}

void MainWindow::parseCode(const QString &str)
{
    bool isLegal = true;
    try
    {
        TK.parse(str);
    }
    catch (QString err_str)
    {
        View.write(err_str);
        View.write("\n");
        isLegal = false;
    }
}




