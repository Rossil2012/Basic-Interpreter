#include "Console.hh"
#include <QKeyEvent>
#include <QTextLine>
#include <QTextCursor>

Console::Console(QWidget *parent)
    : QTextEdit(parent), State(CODING)
{
}


void Console::write(const QString &msg)
{
    QTextCursor cursor = this->textCursor();
    cursor.movePosition(QTextCursor::End);
    cursor.insertText(msg);
}

void Console::keyPressEvent(QKeyEvent *event)
{
    if (this->textCursor().hasSelection())
    {
        return;
    }
    if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace)
    {
        return;
    }
    if (event->key() == Qt::Key_Return) 
    {
        QTextCursor cursor = this->textCursor();
        cursor.movePosition(QTextCursor::End);
        cursor.select(QTextCursor::LineUnderCursor);
        QString lastLine = cursor.selectedText();
        cursor.movePosition(QTextCursor::End);
        cursor.insertText("\n");
        if (State == CODING)
        {
            emit newLineWritten(lastLine);
        }
        else if (State == RUNNING)
        {
            inLine = lastLine;
            inLine.remove(0, 1);
            emit newInput();
        }
        return;
    }
    QTextEdit::keyPressEvent(event);
}

void Console::input(QString &in)
{
    QEventLoop loop;
    connect(this, &Console::newInput, &loop, &QEventLoop::quit, Qt::DirectConnection);
    loop.exec();
    in = inLine;
}

void Console::changeState(int state)
{
    if (state == 0)
    {
        State = CODING;
    }
    else if (state == 1)
    {
        State = RUNNING;
    }
}

void Console::Exit_interpreter()
{
    emit exit_interpreter();
}
