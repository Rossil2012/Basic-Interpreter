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

Tokenizer::Tokenizer(Program *pg)
    : PG(pg)
{}

void Tokenizer::parse(const QString &str)
{
    if (PG->isDebugging())
    {
        if (str.simplified() == "")
        {
            return;
        }
        std::stringstream ss(str.toStdString());
        std::string token;
        ss >> token;
        if (token == "B" || token == "BREAK")
        {
            if (ss.eof())
            {
                throw QString("NO BREAK LINE");
            }
            int line;
            ss >> line;
            if (!ss.good() && !ss.eof())
            {
                throw QString("INVALID LINE");
            }
            if (PG->de_setBp(line) == false)
            {
                throw QString("LINE DO NOT EXIST");
            }
            return;
        }
        else if (token == "D" || token == "DELETE")
        {
            if (ss.eof())
            {
                throw QString("NO BREAK LINE");
            }
            int line;
            ss >> line;
            if (!ss.good() && !ss.eof())
            {
                throw QString("INVALID LINE");
            }
            if (PG->de_delBp(line) == false)
            {
                throw QString("LINE DO NOT EXIST");
            }
            return;
        }
        else if (token == "SB" || token == "SHOWBREAKPOINTS")
        {
            PG->de_showBp();
            return;
        }
        else if (token == "S" || token == "STEPI")
        {
            PG->de_stepin();
            return;
        }
        else if (token == "C" || token == "CONTINUE")
        {
            PG->de_continue();
            return;
        }
        else if (token == "DIS" || token == "DISPLAY")
        {
            PG->de_display();
            return;
        }
        else if (token == "STOP")
        {
            PG->de_stop();
            return;
        }
        else
        {
            throw QString("INVALID DEBUG INSTRUCTION");
        }
    }


    std::stringstream ss(str.toStdString());
    if (str.simplified() == "")
    {
        throw QString("SYNTAX ERROR: EMPTY CODE");
    }

    int line = -100;
    ss >> line;

    if (!ss.good() && !ss.eof())
    {
        ss.clear();
        std::string ctr_token;
        ss >> ctr_token;
        if (ctr_token == "RUN")
        {
            if (ss.eof())
            {
                PG->run();
                return;
            }
        }
        else if (ctr_token == "LIST")
        {
            if (ss.eof())
            {
                PG->printCode();
                return;
            }
        }
        else if (ctr_token == "CLEAR")
        {
            if (ss.eof())
            {
                PG->clear();
                return;
            }
        }
        else if (ctr_token == "HELP")
        {
            if (ss.eof())
            {
                QString help;
                help.append("This is a very simple basic interpreter.\n");
                help.append("So I don't think you need a help documentation.\n");
                help.append("LOLOLOL\n");
                emit print(help);
                return;
            }
        }
        else if (ctr_token == "QUIT")
        {
            if (ss.eof())
            {
                emit exit_interpreter();
            }
        }
        else if (ctr_token == "DEBUG")
        {
            if (ss.eof())
            {
                PG->de_start();
            }
            return;
        }
        else
        {
            throw QString("SYNTAX ERROR: BAD CODE");
        }
    }

    if (ss.eof())
    {
        throw QString("SYNTAX ERROR: LACK OF STATEMENT");
    }
    else if (line < 0)
    {
        throw QString("SYNTAX ERROR: NEGATIVE LINE");
    }

    std::string token;
    ss >> token;


    Statement *tmp = nullptr;

    if (token == "REM")
    {
        if (ss.eof())
        {
            throw QString("SYNTAX ERROR: EMPTY REMARK");
        }

        std::string remark;
        if (!ss.eof())
        {
            //
            getline(ss, remark, '\0');
        }
        if (!ss.eof())
        {
            throw QString("SYNTAX ERROR: WRONG REM FORMAT");
        }
        tmp = new Remstmt(QString::fromStdString(remark));
    }

    else if (token == "LET")
    {
        if (ss.eof())
        {
            throw QString("SYNTAX ERROR: LACK OF VARIABLE");
        }
        std::string var;
        ss >> var;
        std::string equal;
        ss >> equal;
        if (equal != "=")
        {
            throw QString("SYNTAX ERROR: NO EQUAL SIGN");
        }
        if (ss.eof())
        {
            throw QString("SYNTAX ERROR: LACK OF EXPRESSION");
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
            throw  QString("SYNTAX ERROR: INVALID EXPRESSION");
        }
    }

    else if (token == "PRINT")
    {
        if (ss.eof())
        {
            throw QString("SYNTAX ERROR: NO EXPRESSION");
        }

        std::string exp;
        //
        getline(ss, exp, '\0');
        if (!ss.eof())
        {
            throw QString("SYNTAX ERROR: WRONG PRINT FORMAT");
        }

        Expression *mexp = makeEXP(exp, COMP);
        if (mexp->check())
        {
            tmp = new Printstmt(mexp);
        }
        else
        {
            throw  QString("SYNTAX ERROR: INVALID EXPRESSION");
        }
    }

    else if (token == "INPUT")
    {
        if (ss.eof())
        {
            throw QString("SYNTAX ERROR: NO VARIABLE");
        }

        std::string var;
        ss >> var;
        if (!ss.eof())
        {
            throw QString("SYNTAX ERROR: WRONG INPUT FORMAT");
        }

        Expression *mvar = makeEXP(var, IDF);
        if (!mvar->check())
        {
            throw QString("SYNTAX ERROR: INVALID VARIABLE NAME");
        }

        tmp = new Inputstmt(QString::fromStdString(var));
        connect(tmp, &Statement::sig_input, PG, &Program::input, Qt::DirectConnection);
    }

    else if (token == "GOTO")
    {
        if (ss.eof())
        {
            throw QString("SYNTAX ERROR: NO LINE");
        }

        int line;
        ss >> line;
        if (!ss.eof())
        {
            throw QString("SYNTAX ERROR: WRONG LINE NUMBER");
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
            throw QString("SYNTAX ERROR: LACK OF EXPRESSION 1");
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
            throw QString("SYNTAX ERROR: LACK OF EXPRESSION 1");
        }
        else if (stmp == ">" || stmp == "<" || stmp == "=")
        {
            if (ss.eof())
            {
                throw QString("SYNTAX ERROR: LACK OF EXPRESSION 2");
            }
            opt = stmp.at(0);
        }
        else if (ss.eof())
        {
            throw QString("SYNTAX ERROR: LACK OF OPT");
        }
        else
        {
            throw QString("SYNTAX ERROR: INVALID OPERATOR");
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
            throw QString("SYNTAX ERROR: LACK OF EXPRESSION 2");
        }
        else if (stmp == "THEN")
        {
            if (ss.eof())
            {
                throw QString("SYNTAX ERROR: LACK OF LINE NUM");
            }
        }
        else //ss.eof()
        {
            throw QString("SYNTAX ERROR: LACK OF \"THEN\"");
        }

        ss >> line;
        if (!ss.eof())
        {
            throw QString("SYNTAX ERROR: INVALID LINE NUMBER");
        }

        Expression *mexp1 = makeEXP(exp1, COMP), *mexp2 = makeEXP(exp2, COMP);
        if (!mexp1->check())
        {
            throw QString("SYNTAX ERROR: INVALID EXPRESSION 1");
        }
        if (!mexp2->check())
        {
            throw QString("SYNTAX ERROR: INVALID EXPRESSION 2");
        }

        tmp = new Ifstmt(mexp1, opt, mexp2, line);
    }

    else if (token == "END")
    {
        tmp = new Endstmt;
    }

    else
    {
        throw QString("SYNTAX ERROR: WRONG TOKEN");
    }

    if (!ss.eof())
    {
        throw QString("SYNTAX ERROR: EXCESSIVE STATEMENT");
    }

    if (tmp)
    {
        QObject::connect(tmp, &Statement::sig_getVar, PG, &Program::getVar, Qt::DirectConnection);
        QObject::connect(tmp, &Statement::sig_letVar, PG, &Program::letVar, Qt::DirectConnection);
        QObject::connect(tmp, &Statement::sig_changeCursor, PG, &Program::changeCursor, Qt::DirectConnection);
        QObject::connect(tmp, &Statement::sig_moveOn, PG, &Program::moveOn, Qt::DirectConnection);
        QObject::connect(tmp, &Statement::sig_end, PG, &Program::end, Qt::DirectConnection);
        QObject::connect(tmp, &Statement::sig_terminate, PG, &Program::terminate, Qt::DirectConnection);
        QObject::connect(tmp, &Statement::sig_print, PG, &Program::print, Qt::DirectConnection);
        PG->addStmt(line, tmp);
        PG->addCode(line, str);
    }
}





























