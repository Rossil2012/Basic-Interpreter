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
        throw QString("NO ELEMENT");
    }
    return pop_p->Num;
}

template <>
QString Stack::showPOP<QString>()
{
    if (isEmpty())
    {
        throw QString("NO ELEMENT");
    }
    return pop_p->Var;
}

template <>
char Stack::showPOP<char>()
{
    if (isEmpty())
    {
        throw QString("NO ELEMENT");
    }
    return pop_p->Opt;
}

template <>
int Stack::pop<int>()
{
    if (isEmpty())
    {
        throw QString("NO ELEMENT");
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
        throw QString("NO ELEMENT");
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
        throw QString("NO ELEMENT");
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
        throw QString("NO ELEMENT");
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
        throw QString("NO ELEMENT");
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
        throw QString("NO ELEMENT");
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
        throw QString("invalid EXPRESSION");
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
        throw QString("invalid EXPRESSION");
    }

    int value;
    bool varExist;
    emit sig_getVar(Var, value, varExist);
    if (!varExist)
    {
        throw QString("VARIABLE DO NOT EXIST");
    }
    return value;
}

bool IdentifierExp::check()
{
    return isValid;
}


expTree::expTree(Stack &s)
{
    root = new node;
    if (s.getPOPtype() == Stack::elem::NUM)
    {
        if (s.Length() != 1)
        {
            throw QString("INVALID EXPRESSION TREE");
        }
        root->Type = node::NUM;
        root->num = s.pop<int>();
    }
    else if (s.getPOPtype() == Stack::elem::VAR)
    {
        if (s.Length() != 1)
        {
            throw QString("INVALID EXPRESSION TREE");
        }
        root->Type = node::VAR;
        root->var = s.pop<QString>();
    }
    else if (s.getPOPtype() == Stack::elem::OPT)
    {
        root->Type = node::OPT;
        root->opt = s.pop<char>();
        makeTree(root, &s);
    }
}

expTree::~expTree()
{

}

int expTree::Eval(node *N)
{
    if (!N->Left && !N->Right)
    {
        if (N->Type == node::OPT)
        {
            throw QString("INVALID EXPRESSION TREE");
        }
        else if (N->Type == node::NUM)
        {
            return N->num;
        }
        else if (N->Type == node::VAR)
        {
            bool varExist = false;
            int value;
            emit sig_getVar(N->var, value, varExist);
            if (!varExist)
            {
                throw QString("VARIABLE DO NOT EXIST");
            }
            return value;
        }
    }
    else if (N->Left && N->Right)
    {
        if (N->Type == node::NUM || N->Type == node::VAR)
        {
            throw QString("INVALID EXPRESSION TREE");
        }
        switch (N->opt)
        {
            case '+' :
            {
                return Eval(N->Left) + Eval(N->Right);
            }
            case '-' :
            {
                return Eval(N->Left) - Eval(N->Right);
            }
            case '*' :
            {
                return Eval(N->Left) * Eval(N->Right);
            }
            case '/' :
            {
                return Eval(N->Left) / Eval(N->Right);
            }
            default:
            {
                throw QString("INVALID EXPRESSION TREE");
            }
        }
    }
    else
    {
        throw QString("INVALID EXPRESSION TREE");
    }

}

void expTree::makeTree(node *N, Stack *s)
{
    if (N->Type == node::NUM || N->Type == node::VAR)
    {
        return;
    }

    else if (N->Type == node::OPT)
    {
        if (s->Length() < 2)
        {
            throw QString("INVALID EXPRESSION TREE");
        }
        N->Left = new node;
        N->Right = new node;
        if (s->getPOPtype() == Stack::elem::NUM)
        {
            N->Right->Type = node::NUM;
            N->Right->num = s->pop<int>();
        }
        else if (s->getPOPtype() == Stack::elem::VAR)
        {
            N->Right->Type = node::VAR;
            N->Right->var = s->pop<QString>();
        }
        else if (s->getPOPtype() == Stack::elem::OPT)
        {
            N->Right->Type = node::OPT;
            N->Right->opt = s->pop<char>();
            makeTree(N->Right, s);
        }
        if (s->getPOPtype() == Stack::elem::NUM)
        {
            N->Left->Type = node::NUM;
            N->Left->num = s->pop<int>();
        }
        else if (s->getPOPtype() == Stack::elem::VAR)
        {
            N->Left->Type = node::VAR;
            N->Left->var = s->pop<QString>();
        }
        else if (s->getPOPtype() == Stack::elem::OPT)
        {
            N->Left->Type = node::OPT;
            N->Left->opt = s->pop<char>();
            makeTree(N->Left, s);
        }

    }
}

int expTree::eval()
{
    if (!root)
    {
        throw QString("EMPTY TREE");
    }
    return Eval(root);
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
                    else
                    {
                        opt.pop<char>();
                    }

                    break;
                }
                default:
                {
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

    try
    {
        Tree = new expTree(exp);
        connect(Tree, &expTree::sig_getVar, this, &CompoundExp::getVar);
    }
    catch (const char *)
    {
        isValid = false;
        return;
    }

    isValid = true;
}

CompoundExp::~CompoundExp()
{}

int CompoundExp::eval()
{
    if (!isValid)
    {
        throw QString("invalid EXPRESSION");
    }

    return Tree->eval();
}

bool CompoundExp::check()
{
    return isValid;
}

void CompoundExp::getVar(const QString &var, int &value, bool &varExist)
{
    emit sig_getVar(var, value, varExist);
}
