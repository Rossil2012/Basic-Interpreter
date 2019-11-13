#include "tokenizer.hh"

enum expType{CONST, IDF, COMP};

Expression *makeEXP(const std::string &str, expType type)
{
    if (type == CONST)
    {
        return new ConstantExp(QString::fromStdString(str));
    }
    else if (type == IDF)
    {
        return new IdentifierExp(QString::fromStdString(str));
    }
    else
    {
        return new CompoundExp(QString::fromStdString(str));
    }
}

Tokenizer::Tokenizer(Program *parent)
    : Parent(parent)
{}

void Tokenizer::safe_parse(const QString &str)
{
    try
    {
        parse(str);
    }
    catch (const char *error_str)
    {
        qDebug() << error_str;
    }
}

void Tokenizer::parse(const QString &str)
{
    std::stringstream ss(str.toStdString());

    int line;
    ss >> line;

    if (line == 0)
    {
        if (ss.eof())
        {
            Parent->run();
            return;
        }
        else
        {
            throw "WRONG RUN TOKEN";
        }
    }
    else if (line == -1)
    {
        if (ss.eof())
        {
            Parent->clear();
            return;
        }
        else
        {
            throw "WRONG CLEAR TOKEN";
        }
    }

    if (ss.eof())
    {
        throw "LACK OF STATEMENT";
    }
    else if (!ss.good() || line < 0)
    {
        throw "INVALID LINE";
    }

    std::string token;
    ss >> token;


    Statement *tmp = nullptr;

    if (token == "REM")
    {
        std::string remark;
        if (!ss.eof())
        {
            //
            getline(ss, remark, '\0');
        }
        if (!ss.eof())
        {
            throw "WRONG REM FORMAT";
        }
        tmp = new Remstmt(QString::fromStdString(remark));
    }

    else if (token == "LET")
    {
        if (ss.eof())
        {
            throw "LACK OF VARIABLE";
        }
        std::string var;
        ss >> var;
        std::string equal;
        ss >> equal;
        if (equal != "=")
        {
            throw "NO EQUAL SIGN";
        }
        if (ss.eof())
        {
            throw "LACK OF EXPRESSION";
        }
        std::string exp;
        //
        getline(ss, exp, '\0');

        Expression *mexp = makeEXP(exp, COMP);
        if (mexp->check())
        {
            tmp = new Letstmt(QString::fromStdString(var), mexp);
        }
        else
        {
            throw  "INVALID EXPRESSION";
        }
    }

    else if (token == "PRINT")
    {
        std::string exp;
        //
        getline(ss, exp, '\0');
        if (!ss.eof())
        {
            throw "WRONG PRINT FORMAT";
        }

        Expression *mexp = makeEXP(exp, COMP);
        if (mexp->check())
        {
            tmp = new Printstmt(mexp);
        }
        else
        {
            throw  "INVALID EXPRESSION";
        }
    }

    else if (token == "INPUT")
    {
        std::string var;
        ss >> var;
        if (!ss.eof())
        {
            throw "WRONG INPUT FORMAT";
        }

        Expression *mvar = makeEXP(var, IDF);
        if (!mvar->check())
        {
            throw "INVALID VARIABLE NAME";
        }

        tmp = new Inputstmt(QString::fromStdString(var));
    }

    else if (token == "GOTO")
    {
        int line;
        ss >> line;
        if (!ss.eof())
        {
            throw "WRONG LINE NUMBER";
        }

        tmp = new Gotostmt(line);
    }

    else if (token == "IF")
    {
        std::string exp1, exp2, stmp;
        char opt;
        int line;

        if (ss.eof())
        {
            throw "LACK OF EXPRESSION 1";
        }

        while (!ss.eof())
        {
            ss >> stmp;
            if (stmp == ">" || stmp == "<" || stmp == "=")
            {
                break;
            }
            exp1.append(stmp);
        }
        if (exp1.empty())
        {
            throw "LACK OF EXPRESSION 1";
        }
        else if (stmp == ">" || stmp == "<" || stmp == "=")
        {
            if (ss.eof())
            {
                throw "LACK OF EXPRESSION 2";
            }
            opt = stmp.at(0);
        }
        else if (ss.eof())
        {
            throw "LACK OF OPT";
        }
        else
        {
            throw "INVALID OPERATOR";
        }

        while (!ss.eof())
        {
            ss >> stmp;
            if (stmp == "THEN")
            {
                break;
            }
            exp2.append(stmp);
        }
        if (exp2.empty())
        {
            throw "LACK OF EXPRESSION 2";
        }
        else if (stmp == "THEN")
        {
            if (ss.eof())
            {
                throw "LACK OF LINE NUM";
            }
        }
        else //ss.eof()
        {
            throw "LACK OF \"THEN\"";
        }

        ss >> line;
        if (!ss.eof())
        {
            throw "INVALID LINE NUMBER";
        }

        Expression *mexp1 = makeEXP(exp1, COMP), *mexp2 = makeEXP(exp2, COMP);
        if (!mexp1->check())
        {
            throw "INVALID EXPRESSION 1";
        }
        if (!mexp2->check())
        {
            throw "INVALID EXPRESSION 2";
        }

        tmp = new Ifstmt(mexp1, opt, mexp2, line);
    }

    else if (token == "END")
    {
        tmp = new Endstmt;
    }

    else
    {
        throw "WRONG TOKEN";
    }

    if (!ss.eof())
    {
        throw "EXCESSIVE STATEMENT";
    }

    if (tmp)
    {
        QObject::connect(tmp, &Statement::sig_getVar, Parent, &Program::getVar, Qt::DirectConnection);
        QObject::connect(tmp, &Statement::sig_letVar, Parent, &Program::letVar, Qt::DirectConnection);
        QObject::connect(tmp, &Statement::sig_changeCursor, Parent, &Program::changeCursor, Qt::DirectConnection);
        QObject::connect(tmp, &Statement::sig_moveOn, Parent, &Program::moveOn, Qt::DirectConnection);
        QObject::connect(tmp, &Statement::sig_terminate, Parent, &Program::terminate, Qt::DirectConnection);
        Parent->insert(line, tmp);
    }
}





























