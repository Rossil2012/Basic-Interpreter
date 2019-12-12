#include "statement.hh"

Statement::~Statement()
{}

Remstmt::Remstmt(const QString &rem)
    : remark(rem)
{}

Remstmt::~Remstmt()
{}

void Remstmt::execute()
{
    emit sig_moveOn();
}

Letstmt::Letstmt(const QString &lv, Expression *rv)
    : leftValue(lv), rightValue(rv)
{
    if (!chkVar(lv))
    {
        throw QString("INVALID VARIABLE NAME");
    }
    connect(rv, &Expression::sig_getVar, this, &Letstmt::getVar, Qt::DirectConnection);
}

Letstmt::~Letstmt()
{
    delete rightValue;
}

void Letstmt::execute()
{
    int value;
    try
    {
         value = rightValue->eval();
    }
    catch (QString err_str)
    {
        emit sig_terminate();
        throw  "RUNTIME ERROR: " + err_str;
    }
    emit sig_letVar(leftValue, value);
    emit sig_moveOn();
}

void Letstmt::getVar(const QString &var, int &value, bool &varExist)
{
    emit sig_getVar(var, value, varExist);
}

Printstmt::Printstmt(Expression *exp)
    : Exp(exp)
{
    connect(Exp, &Expression::sig_getVar, this, &Printstmt::getVar, Qt::DirectConnection);
}

Printstmt::~Printstmt()
{
    delete Exp;
}

//
void Printstmt::execute()
{
    int toPrint;

    try
    {
        toPrint = Exp->eval();
    }
    catch (QString err_str)
    {
        emit sig_terminate();
        throw  QString("RUNTIME ERROR: " + err_str);
    }
    emit sig_print(QString::number(toPrint) + '\n');
    emit sig_moveOn();
}

void Printstmt::getVar(const QString &var, int &value, bool &varExist)
{
    emit sig_getVar(var, value, varExist);
}

Inputstmt::Inputstmt(const QString &var)
    : Var(var)
{}

Inputstmt::~Inputstmt()
{}

//
void Inputstmt::execute()
{
    int value;
    QString num;

    emit sig_print("?");
    //input >> value
    emit sig_input(num);
    bool isInt = false;
    value = num.toInt(&isInt);
    if (!isInt)
    {
        emit sig_terminate();
        throw QString("RUNTIME ERROR: INVALID NUMBER");
    }

    emit sig_letVar(Var, value);
    emit sig_moveOn();
}

Gotostmt::Gotostmt(int line)
    : Line(line)
{}

Gotostmt::~Gotostmt()
{}

void Gotostmt::execute()
{
    bool lineExist;
    emit sig_changeCursor(Line, lineExist);
    if (!lineExist)
    {
        emit sig_terminate();
        throw QString("RUNTIME ERROR: THE LINE DO NOT EXIST");
    }
}

Ifstmt::Ifstmt(Expression *exp1, char opt, Expression *exp2, int line)
    : Exp1(exp1), Opt(opt), Exp2(exp2), Line(line)
{
    connect(Exp1, &Expression::sig_getVar, this, &Ifstmt::getVar, Qt::DirectConnection);
    connect(Exp2, &Expression::sig_getVar, this, &Ifstmt::getVar, Qt::DirectConnection);
}

Ifstmt::~Ifstmt()
{
    delete Exp1;
    delete Exp2;
}

void Ifstmt::execute()
{
    int lvalue = Exp1->eval(), rvalue = Exp2->eval();
    bool isSatisfied = false;
    switch (Opt)
    {
        case '>':
        {
            if (lvalue > rvalue)
            {
                isSatisfied = true;
            }
            break;
        }

        case '<':
        {
            if (lvalue < rvalue)
            {
                isSatisfied = true;
            }
            break;
        }

        case '=':
        {
            if (lvalue == rvalue)
            {
                isSatisfied = true;
            }
            break;
        }

        default:
        {
            throw QString("RUNTIME ERROR: INVALID OPERATOR");
        }
    }
    if (isSatisfied)
    {
        bool lineExist;
        emit sig_changeCursor(Line, lineExist);
        if (!lineExist)
        {
            emit sig_terminate();
            throw QString("RUNTIME ERROR: THE LINE DO NOT EXIST");
        }
    }
    else
    {
        emit sig_moveOn();
    }

}

void Ifstmt::getVar(const QString &var, int &value, bool &varExist)
{
    emit sig_getVar(var, value, varExist);
}

Endstmt::Endstmt()
{}

Endstmt::~Endstmt()
{}

void Endstmt::execute()
{
    emit sig_end();
}
