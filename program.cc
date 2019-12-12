#include "program.hh"

Program::Program()
    :Cursor(0),  normal_end(true), terminateFlag(false)
{}

Program::~Program()
{
    for (auto i = Statements.begin(); i != Statements.end(); ++i)
    {
        delete i.value();
    }
}

void Program::run()
{
    //Initialization
    Vars.clear();
    Cursor = 0;
    ite_Cursor = Statements.begin();
    terminateFlag = false;

    //Change the state of console to RUNNING
    emit changeState(1);

    execute();

    //Change the state of console to CODING
    if (normal_end)
    {
        emit print("PROGRAM \n");//////
    }

}

void Program::clear()
{
    Statements.clear();
    Codes.clear();
    Vars.clear();
    Cursor = 0;
    ite_Cursor = Statements.begin();
    terminateFlag = false;
}

void Program::addStmt(int line, Statement *stmt)
{
    Statements.insert(line, stmt);
}

void Program::addCode(int line, const QString &code)
{
    Codes.insert(line, code);
}

void Program::printCode()
{
    for (auto i = Codes.begin(); i != Codes.end(); ++i)
    {
        emit print(*i + '\n');
    }
}

void Program::execute()
{
    ite_Cursor = Statements.begin();
    Cursor = ite_Cursor.key();
    while (!terminateFlag)
    {
        if (Statements.isEmpty())
        {
            terminate();
            throw "RUNTIME ERROR: PROGRAM IS EMPTY";
        }
        ite_Cursor.value()->execute();
    }

}

void Program::letVar(const QString &var, int value)
{
    Vars.letVar(var, value);
}

void Program::getVar(const QString &var, int &value, bool &varExist)
{
    varExist = Vars.getVar(var, value);
}

void Program::changeCursor(int line, bool &lineExist)
{
    if (Statements.contains(line))
    {
        Cursor = line;
        ite_Cursor = Statements.find(line);
        lineExist = true;
    }
    else
    {
        lineExist = false;
    }
}

void Program::moveOn()
{
    ++ite_Cursor;
    if (ite_Cursor == Statements.end())
    {
        terminate();
        throw "RUNTIME ERROR: NO NEXT STATEMENT";
    }
    Cursor = ite_Cursor.key();
}

void Program::end()
{
    normal_end = true;
    terminateFlag = true;
    emit changeState(0);
}

void Program::terminate()
{
    normal_end = false;
    terminateFlag = true;
    emit changeState(0);
    emit print("PROGRAM TERMINATE\n");
}
