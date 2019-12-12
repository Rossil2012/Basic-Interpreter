#ifndef EXP_HH
#define EXP_HH

#include <QDebug>
#include <QString>
#include <string>
#include <sstream>

bool chkVar(const QString &var);

class Stack
{
public:
    struct elem
    {
        enum type {NUM, VAR, OPT} Type;
        elem(int num, elem *next)
            : Type(NUM), Num(num), Var(""), Opt(-1), Next(next), Prev(nullptr)
        {}
        elem(QString var, elem *next)
            : Type(VAR), Num(-1), Var(var), Opt(-1), Next(next), Prev(nullptr)
        {}
        elem(char opt, elem *next)
            : Type(OPT), Num(-1), Var(""), Opt(opt), Next(next), Prev(nullptr)
        {}
        elem(const elem &ano)
            : Type(ano.Type), Num(ano.Num), Var(ano.Var), Opt(ano.Opt), Next(nullptr), Prev(nullptr)
        {}
        int Num;
        QString Var;
        char Opt;
        elem *Next;
        elem *Prev;
    };
private:
    elem *pop_p;
    elem *end;
    int length;
public:
    Stack();
    Stack(const Stack &ano);
    Stack &operator=(const Stack & ano);
    ~Stack();
    int Length() const;
    void stat(int &num, int &var, int &opt);
    bool isEmpty() const;
    elem::type getPOPtype() const;
    elem::type getrPOPtype() const;
    template <class out> out showPOP();
    template <class out> out pop();
    template <class out> out rpop();
    template <class in> void push(const in &elem);


    /*
    //Instantiation Declaration
    template<> int showPOP<int>();
    template<> QString showPOP<QString>();
    template<> char showPOP<char>();
    template<> int pop<int>();
    template<> QString pop<QString>();
    template<> char pop<char>();
    template<> int rpop<int>();
    template<> QString rpop<QString>();
    template<> char rpop<char>();
    */


    void print();
};

class Expression : public QObject
{
    Q_OBJECT
public:
    virtual ~Expression() = 0;
    virtual int eval() = 0;
    virtual bool check() = 0;
signals:
    void sig_getVar(const QString &var, int &value, bool &varExist);
};

class ConstantExp : public Expression
{
private:
    int Num;
    bool isValid;
public:
    ConstantExp(const QString &num);
    virtual ~ConstantExp() override;
    virtual int eval() override;
    virtual bool check() override;
};

class IdentifierExp : public Expression
{
private:
    QString Var;
    bool isValid;
public:
    IdentifierExp(const QString &var);
    virtual ~IdentifierExp() override;
    virtual int eval() override;
    virtual bool check() override;

};

class expTree : public QObject
{
    Q_OBJECT
    struct node
    {
        enum {OPT, NUM, VAR} Type;
        node *Left = nullptr;
        node *Right = nullptr;
        int num = -1;
        char opt = -1;
        QString var;
    };
private:
    node *root;
    int Eval(node *N);
    void makeTree(node *N, Stack *s);
public:
    expTree(Stack &s);
    ~expTree();
    int eval();
signals:
    void sig_getVar(const QString &var, int &value, bool &varExist);
};

class CompoundExp : public Expression
{
    Q_OBJECT
private:
    Stack EXP;
    expTree *Tree;
    bool isValid;
public:
    CompoundExp(const QString &cexp);
    virtual ~CompoundExp() override;
    virtual int eval() override;
    virtual bool check() override;
public slots:
    void getVar(const QString &var, int &value, bool &varExist);
};


#endif // EXP_HH
