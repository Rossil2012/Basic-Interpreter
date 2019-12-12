#include "mainwindow.hh"

InputLine::InputLine(QWidget *parent)
    : QLineEdit(parent)
{
    connect(this, &QLineEdit::editingFinished, this, &InputLine::sendCode);
}

void InputLine::sendCode()
{
    selectAll();
    emit newCodeWritten(selectedText());
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), Input(this), ErrorView(this), ProgramView(this), TK(&PG)
{
    ErrorView.setReadOnly(true);
    ProgramView.setReadOnly(true);
    connect(&Input, &InputLine::newCodeWritten, this, &MainWindow::parseCode);

    setFixedSize(700, 700);

    Input.setFixedSize(550, 50);
    ErrorView.setFixedSize(550, 50);
    ProgramView.setFixedSize(600, 400);

    QWidget *cen_widget = new QWidget(this);
    cen_widget->setFixedSize(700, 700);
    QVBoxLayout *layout = new QVBoxLayout(cen_widget);

    QHBoxLayout *in_layout = new QHBoxLayout;
    QLabel *in_label = new QLabel("Input:", cen_widget);
    in_label->setFixedSize(50, 50);
    in_layout->addWidget(in_label);
    in_layout->addWidget(&Input);
    in_layout->setSpacing(0);

    QHBoxLayout *err_layout = new QHBoxLayout;
    QLabel *err_label = new QLabel("Error:", cen_widget);
    err_label->setFixedSize(50, 50);
    err_layout->addWidget(err_label);
    err_layout->addWidget(&ErrorView);
    err_layout->setSpacing(0);

    QHBoxLayout *pg_layout = new QHBoxLayout;
    pg_layout->addWidget(&ProgramView);

    layout->addLayout(in_layout);
    layout->addLayout(err_layout);
    layout->addLayout(pg_layout);
    layout->setMargin(50);
    cen_widget->setLayout(layout);
    setCentralWidget(cen_widget);


}

void MainWindow::parseCode(const QString &code)
{
    bool isLegal = true;
    try
    {
        TK.parse(code);
    }
    catch (const char *err_str)
    {
        ErrorView.setText(err_str);
        isLegal = false;
    }

    if (isLegal)
    {
        ErrorView.clear();
        ProgramView.append(code);
    }
}

MainWindow2::MainWindow2(QWidget *parent)
    : QMainWindow(parent), View(this), TK(&PG)
{
    connect(&View, &Console::newLineWritten, this, &MainWindow2::parseCode);
    connect(&PG, &Program::changeState, &View, &Console::changeState);
    connect(&PG, &Program::input, &View, &Console::input);
    connect(&PG, &Program::print, &View, &Console::write);
    setCentralWidget(&View);
    layout()->setMargin(0);

    setFixedSize(600, 600);
}

void MainWindow2::parseCode(const QString &str)
{
    bool isLegal = true;
    try
    {
        TK.parse(str);
    }
    catch (const char *err_str)
    {
        View.write(err_str);
        View.write("\n");
        isLegal = false;
    }
}




