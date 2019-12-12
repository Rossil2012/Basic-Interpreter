#include "program.hh"

Program::Program()
    :Cursor(0),  normal_end(true), terminateFlag(false), de_on(false)
{
    connect(&Vars, &Evalstate::print, this, &Program::print);
}

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
        emit print("PROGRAM IS EXECUTED SUCCESSFULLY\n");
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

bool Program::isDebugging()
{
    return de_on;
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
            throw QString("RUNTIME ERROR: PROGRAM IS EMPTY");
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
        throw QString("RUNTIME ERROR: NO NEXT STATEMENT");
    }
    Cursor = ite_Cursor.key();
}

void Program::end()
{
    if (!de_on)
    {
        normal_end = true;
        terminateFlag = true;
        emit changeState(0);
    }
    else
    {
        de_stop();
    }
}

void Program::terminate()
{
    if (!de_on)
    {
        normal_end = false;
        terminateFlag = true;
        emit changeState(0);
        emit print("PROGRAM TERMINATES EXCEPTIONALLY\n");
    }
    else
    {
        de_stop();
    }
}

void Program::de_start()
{
    de_on = true;
    emit print("Debugger mode is on.\n");
    Vars.clear();
    Cursor = 0;
    ite_Cursor = Statements.begin();
    terminateFlag = false;
}

void Program::de_stop()
{
    de_on = false;
    Vars.clear();
    bp.clear();
    Cursor = 0;
    ite_Cursor = Statements.begin();
    terminateFlag = false;
    normal_end = true;
    emit print("DEBUG OVER.\n");
}

bool Program::de_setBp(int line)
{
    if (!Statements.contains(line))
    {
        return false;
    }
    if (!bp.contains(line))
    {
        bp.push_back(line);
    }
    return true;
}

bool Program::de_delBp(int line)
{
    if (!bp.contains(line))
    {
        return false;
    }
    bp.remove(bp.indexOf(line));
    return true;
}
void Program::de_showBp()
{
    for (auto i = bp.begin(); i != bp.end(); ++i)
    {
        emit print(QString::number(*i) + ' ');
    }
    emit print("\n");
}
void Program::de_stepin()
{
    ++ite_Cursor;
    if (ite_Cursor == Statements.end())
    {
        terminate();
    }
    Cursor = ite_Cursor.key();
}
void Program::de_continue()
{
    de_stepin();
    while (!terminateFlag)
    {
        if (bp.contains(Cursor))
        {
            emit print("Current line: " + Codes[Cursor] + "\n");
            break;
        }
        if (Statements.isEmpty())
        {
            emit print("RUNTIME ERROR: PROGRAM IS EMPTY");
            de_stop();
        }
        ite_Cursor.value()->execute();
    }
}
void Program::de_display()
{
    Vars.printVars();
}
