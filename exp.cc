#include "exp.hh"

bool chkVar(const QString &var)
{
    std::stringstream ss(var.toStdString());
    std::string stmp;
    ss >> stmp;
    if (ss.eof())
    {
        char cht = stmp.at(0);
        if (!((cht >= 'a' && cht <= 'z') || (cht >='A' && cht <='Z') || cht == '_'))
        {
            return false;
        }
        for (unsigned long i = 1; i < stmp.length(); ++i)
        {
            cht = stmp.at(i);
            if (!((cht >= 'a' && cht <= 'z') || (cht >='A' && cht <='Z') || (cht >= '0' && cht <= '9') || cht == '_'))
            {
                return false;
            }
        }
    }
    else
    {
        return false;
    }
    return true;
}

Stack::Stack()
    :pop_p(nullptr), end(nullptr), length(0)
{}

Stack::Stack(const Stack &ano)
{
    length = ano.length;
    elem *tmp = ano.end;
    if (!ano.end)
    {
        pop_p = nullptr;
        end = nullptr;
        return;
    }

    end = pop_p = new elem(*tmp);
    tmp = tmp->Prev;

    while(tmp)
    {
        elem *ttmp = pop_p;
        pop_p = new Stack::elem(*tmp);
        pop_p->Next = ttmp;
        ttmp->Prev = pop_p;
        tmp = tmp->Prev;
    }
}

Stack &Stack::operator=(const Stack &ano)
{
    if (this == &ano)
    {
        return *this;
    }

    while (length-- != 0)
    {
        elem *tmp = pop_p;
        pop_p = pop_p->Next;
        delete tmp;
    }

    length = ano.length;
    elem *tmp = ano.end;
    if (!ano.end)
    {
        pop_p = nullptr;
        end = nullptr;
        return *this;
    }

    end = pop_p = new elem(*tmp);
    tmp = tmp->Prev;

    while(tmp)
    {
        elem *ttmp = pop_p;
        pop_p = new Stack::elem(*tmp);
        pop_p->Next = ttmp;
        ttmp->Prev = pop_p;
        tmp = tmp->Prev;
    }

    return *this;
}

Stack::~Stack()
{
    while (length-- != 0)
    {
        elem *tmp = pop_p;
        pop_p = pop_p->Next;
        delete tmp;
    }
}

int Stack::Length() const
{
    return length;
}

void Stack::stat(int &num, int &var, int &opt)
{
    int Num = 0, Var = 0, Opt = 0;
    elem *tmp = end;
    while (tmp)
    {
        if (tmp->Type == elem::NUM)
        {
            ++Num;
        }

        if (tmp->Type == elem::VAR)
        {
            ++Var;
        }

        if (tmp->Type == elem::OPT)
        {
            ++Opt;
        }

        tmp = tmp->Prev;
    }
    num = Num;
    var = Var;
    opt = Opt;
}

bool Stack::isEmpty() const
{
    return length == 0;
}

Stack::elem::type Stack::getPOPtype() const
{
    return pop_p->Type;
}

Stack::elem::type Stack::getrPOPtype() const
{
    return end->Type;
}

template <>
int Stack::showPOP<int>()
{
    if (isEmpty())
    {
        throw "NO ELEMENT";
    }
    return pop_p->Num;
}

template <>
QString Stack::showPOP<QString>()
{
    if (isEmpty())
    {
        throw "NO ELEMENT";
    }
    return pop_p->Var;
}

template <>
char Stack::showPOP<char>()
{
    if (isEmpty())
    {
        throw "NO ELEMENT";
    }
    return pop_p->Opt;
}

template <>
int Stack::pop<int>()
{
    if (isEmpty())
    {
        throw "NO ELEMENT";
    }
    elem *tmp = pop_p;
    int TMP = tmp->Num;
    pop_p = pop_p->Next;
    delete tmp;
    --length;
    return TMP;
}

template <>
QString Stack::pop<QString>()
{
    if (isEmpty())
    {
        throw "NO ELEMENT";
    }
    elem *tmp = pop_p;
    QString TMP = tmp->Var;
    pop_p = pop_p->Next;
    delete tmp;
    --length;
    return TMP;
}

template <>
char Stack::pop<char>()
{
    if (isEmpty())
    {
        throw "NO ELEMENT";
    }
    elem *tmp = pop_p;
    char TMP = tmp->Opt;
    pop_p = pop_p->Next;
    delete tmp;
    --length;
    return TMP;
}

template <>
int Stack::rpop<int>()
{
    if (isEmpty())
    {
        throw "NO ELEMENT";
    }
    elem *tmp = end;
    int TMP = tmp->Num;
    end = end->Prev;
    delete tmp;
    --length;
    return TMP;
}

template <>
QString Stack::rpop<QString>()
{
    if (isEmpty())
    {
        throw "NO ELEMENT";
    }
    elem *tmp = end;
    QString TMP = tmp->Var;
    end = end->Prev;
    delete tmp;
    --length;
    return TMP;
}

template <>
char Stack::rpop<char>()
{
    if (isEmpty())
    {
        throw "NO ELEMENT";
    }
    elem *tmp = end;
    char TMP = tmp->Opt;
    end = end->Prev;
    delete tmp;
    --length;
    return TMP;
}

template <class in>
void Stack::push(const in &elem)
{
    if (pop_p)
    {
        Stack::elem *tmp = pop_p;
        pop_p = new Stack::elem(elem, pop_p);
        tmp->Prev = pop_p;
        ++length;
    }
    else
    {
        end = pop_p = new Stack::elem(elem, pop_p);
        ++length;
    }
}

void Stack::print()
{
    elem *tmp = end;
    while (tmp)
    {
        if (tmp->Type == elem::type::NUM)
            qDebug() << tmp->Num << ' ';
        else if (tmp->Type == elem::type::VAR)
            qDebug() << tmp->Var << ' ';
        else
            qDebug() << tmp->Opt << ' ';
        tmp = tmp->Prev;
    }
    qDebug() << endl;
}


Expression::~Expression()
{}

ConstantExp::ConstantExp(const QString &con)
    : Num(-1), isValid(false)
{
    std::stringstream ss(con.toStdString());
    std::string stmp;
    ss >> stmp;
    if (ss.eof())
    {
        std::stringstream sst(stmp);
        int num;
        sst >> num;
        if (sst.eof())
        {
            Num = num;
            isValid = true;
        }
    }
}

ConstantExp::~ConstantExp()
{}

int ConstantExp::eval()
{
    if (!isValid)
    {
        throw "invalid EXPRESSION";
    }
    return Num;
}

bool ConstantExp::check()
{
    return isValid;
}

IdentifierExp::IdentifierExp(const QString &var)
    : Var(""), isValid(false)
{
    isValid = chkVar(var);
    if (isValid)
    {
        Var = var;
    }
}

IdentifierExp::~IdentifierExp()
{}

//to be edited
int IdentifierExp::eval()
{
    if (!isValid)
    {
        throw "invalid EXPRESSION";
    }

    int value;
    bool varExist;
    emit sig_getVar(Var, value, varExist);
    if (!varExist)
    {
        throw "VARIABLE DO NOT EXIST";
    }
    return value;
}

bool IdentifierExp::check()
{
    return isValid;
}

CompoundExp::CompoundExp(const QString &cexp)
    : isValid(false)
{
    QString Exp = cexp;
    for (int i = 0; i != Exp.length(); ++i)
    {
        QChar ic = Exp.at(i);
        if (ic == '+' || ic == '-' || ic == '*' || ic == '/' || ic == '(' || ic == ')')
        {
            Exp.insert(i, ' ');
            i += 2;
            Exp.insert(i, ' ');
        }
    }
    Exp.push_front("( ");
    Exp.push_back(" )");


    //Infix Expression to Suffix Expression
    Stack opt;
    Stack exp;
    std::stringstream ss(Exp.toStdString());
    while (!ss.eof())
    {
        std::string tmp;
        int ite;
        char o;
        ss >> tmp;
        char ctmp = tmp.at(0);
        if (ctmp == '+' || ctmp == '-' || ctmp == '*' || ctmp == '/' || ctmp == '(' || ctmp == ')')
        {
            if (tmp.length() != 1)
            {
                //throw "WRONG OPT";
                return;
            }
            o = ctmp;
            switch (o)
            {
                case '(':
                {
                    opt.push<char>(o);
                    break;
                }
                case '+':
                {
                    while (!opt.isEmpty())
                    {
                        if (opt.showPOP<char>() == '(')
                        {
                            break;
                        }
                        exp.push<char>(opt.pop<char>());
                    }
                    if (opt.isEmpty())
                    {
                        isValid = false;
                        return;
                    }
                    opt.push<char>('+');
                    break;
                }
                case '-':
                {
                    while (!opt.isEmpty())
                    {
                        if (opt.showPOP<char>() == '(')
                        {
                            break;
                        }
                        exp.push<char>(opt.pop<char>());
                    }
                    if (opt.isEmpty())
                    {
                        isValid = false;
                        return;
                    }
                    opt.push<char>('-');
                    break;
                }
                case '*':
                {
                    while (!opt.isEmpty())
                    {
                        if (opt.showPOP<char>() == '+' || opt.showPOP<char>() == '-' || opt.showPOP<char>() == '(')
                        {
                            break;
                        }
                        exp.push<char>(opt.pop<char>());
                    }
                    if (opt.isEmpty())
                    {
                        isValid = false;
                        return;
                    }
                    opt.push<char>('*');
                    break;
                }
                case '/':
                {
                    while (!opt.isEmpty())
                    {
                        if (opt.showPOP<char>() == '+' || opt.showPOP<char>() == '-' || opt.showPOP<char>() == '(')
                        {
                            break;
                        }
                        exp.push<char>(opt.pop<char>());
                    }
                    if (opt.isEmpty())
                    {
                        isValid = false;
                        return;
                    }
                    opt.push<char>('/');
                    break;
                }
                case ')':
                {
                    while (!opt.isEmpty())
                    {
                        if (opt.showPOP<char>() == '(')
                        {
                            break;
                        }
                        exp.push<char>(opt.pop<char>());
                    }
                    if (opt.isEmpty())
                    {
                        isValid = false;
                        return;
                    }
                    else //opt.showPOP<char>() == '('
                    {
                        opt.pop<char>();
                    }

                    break;
                }
                default:
                {
                    //throw "invalid OPERATOR";
                    isValid = false;
                    return;
                }
            }
        }
        else if (ctmp >= '0' && ctmp <= '9')
        {
            std::stringstream sst(tmp);
            sst >> ite;
            if (sst.eof())
            {
                exp.push<int>(ite);
            }
            else
            {
                return;
            }
        }
        else if (chkVar(QString::fromStdString(tmp)))
        {
            exp.push<QString>(QString::fromStdString(tmp));
        }
        else
        {
            return;
        }
    }

    EXP = exp;

    /*
    //to be revised

    int numCount = 0, varCount = 0, optCount = 0;
    exp.stat(numCount, varCount, optCount);
    if (numCount + varCount == optCount + 1)
    {
        isValid = true;
    }
    */

    Stack target;
    int count = 0;
    char topt;
    while (!exp.isEmpty())
    {
        if (count >= 2)
        {
            if (exp.getrPOPtype() == Stack::elem::type::OPT)
            {
                topt = exp.rpop<char>();
                if (!(topt == '+' || topt == '-' || topt == '*' || topt == '/'))
                {
                    //should never be reached
                    //throw "Unvalid Operator!";
                    throw "never reach in compound exp";
                }
                for (int i = 0; i < 2; ++i)
                {
                    if (target.getrPOPtype() == Stack::elem::type::NUM)
                    {
                        target.pop<int>();
                    }
                    else if (target.getrPOPtype() == Stack::elem::type::VAR)
                    {
                        target.pop<QString>();
                    }
                    --count;
                }
                target.push<int>(0);
                ++count;
            }
            else if (exp.getrPOPtype() == Stack::elem::type::NUM)
            {
                exp.rpop<int>();
                target.push<int>(0);
                ++count;
            }
            else if (exp.getrPOPtype() == Stack::elem::type::VAR)
            {
                exp.rpop<QString>();
                target.push<int>(0);
                ++count;
            }
        }
        else
        {
            if (exp.getrPOPtype() == Stack::elem::type::OPT)
            {
                isValid = false;
                return;
            }
            else if (exp.getrPOPtype() == Stack::elem::type::NUM)
            {
                target.push<int>(exp.rpop<int>());
                ++count;
            }
            else if (exp.getrPOPtype() == Stack::elem::type::VAR)
            {
                exp.rpop<QString>();
                target.push<int>(0);
                ++count;
            }
        }
    }

    if (target.Length() != 1)
    {
        isValid = false;
        return;
    }
    else
    {
        isValid = true;
    }

}

CompoundExp::~CompoundExp()
{}

int CompoundExp::eval()
{
    if (!isValid)
    {
        throw "invalid EXPRESSION";
    }

    Stack target;
    int count = 0;
    int tmp[2] ={0};
    char topt;
    while (!EXP.isEmpty())
    {
        if (count >= 2)
        {
            if (EXP.getrPOPtype() == Stack::elem::type::OPT)
            {
                topt = EXP.rpop<char>();
                for (int i = 0; i < 2; ++i)
                {
                        tmp[i] = target.pop<int>();
                        --count;
                }
                switch(topt)
                {
                    case '+':
                    {
                        target.push<int>(tmp[0] + tmp[1]);
                        ++count;
                        break;
                    }
                    case '-':
                    {
                        target.push<int>(tmp[0] - tmp[1]);
                        ++count;
                        break;
                    }
                    case '*':
                    {
                        target.push<int>(tmp[0] * tmp[1]);
                        ++count;
                        break;
                    }
                    case '/':
                    {
                        if (tmp[1] == 0)
                        {
                            throw "Divided by Zero!";
                        }
                        target.push<int>(tmp[1] / tmp[0]);
                        ++count;
                        break;
                    }
                    default:
                    {
                        //should never be reached
                        //throw "Unvalid Operator!";
                        throw "never reach in compound EXP";
                    }
                }
            }
            else if (EXP.getrPOPtype() == Stack::elem::type::NUM)
            {
                target.push<int>(EXP.rpop<int>());
                ++count;
            }
            else if (EXP.getrPOPtype() == Stack::elem::type::VAR)
            {
                QString var = EXP.rpop<QString>();
                int value;
                bool varExist;
                emit sig_getVar(var, value, varExist);
                if (varExist)
                {
                    target.push<int>(value);
                    ++count;
                }
                else
                {
                    //to be reconsidered
                    //isValid = false;
                    throw "VARIABLE DO NOT EXIST";
                }

            }
        }
        else
        {
            if (EXP.getrPOPtype() == Stack::elem::type::OPT)
            {
                //should never be reached
                isValid = false;

            }
            else if (EXP.getrPOPtype() == Stack::elem::type::NUM)
            {
                target.push<int>(EXP.rpop<int>());
                ++count;
            }
            else if (EXP.getrPOPtype() == Stack::elem::type::VAR)
            {
                QString var = EXP.rpop<QString>();
                int value;
                bool varExist;
                emit sig_getVar(var, value, varExist);
                if (varExist)
                {
                    target.push<int>(value);
                    ++count;
                }
                else
                {
                    //to be reconsidered
                    //isValid = false;
                    throw "VARIABLE DO NOT EXIST";
                }
            }
        }
    }
    if (target.Length() == 1)
    {
        return target.pop<int>();
    }
    else
    {
        //should never be reached
        throw "never reach in compound exp";
    }
}

bool CompoundExp::check()
{
    return isValid;
}
