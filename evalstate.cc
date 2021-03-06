#include "evalstate.hh"

Evalstate::Evalstate()
    : varNum(0)
{}

bool Evalstate::getVar(const QString &var, int &value)
{
    if (Vars.contains(var))
    {
        value = Vars[var];
        return true;
    }
    else
    {
        return false;
    }
}

void Evalstate::letVar(const QString &var, int value)
{
    if (!Vars.contains(var))
    {
        varNum++;
    }

    Vars.insert(var, value);
}

void Evalstate::clear()
{
    Vars.clear();
    varNum = 0;
}

void Evalstate::printVars()
{
    for (auto i = Vars.begin(); i != Vars.end(); ++i)
    {
        emit print(i.key() + ": ");
        emit print(QString::number(i.value()) + "\n");
    }
}
