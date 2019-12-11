#ifndef STATEMENT_HH
#define STATEMENT_HH

#include <QString>
#include "exp.hh"

class Statement : public QObject
{
    Q_OBJECT
public:
    virtual ~Statement() = 0;
    virtual void execute() = 0;
signals:
    void sig_letVar(const QString &var, int value);
    void sig_getVar(const QString &var, int &value, bool &varExist);
    void sig_changeCursor(int line, bool &lineExist);
    void sig_moveOn();
    void sig_terminate();

    void sig_input(QString &in);
    void sig_print(const QString &out);
};

class Remstmt : public Statement
{
private:
    QString remark;
public:
    Remstmt(const QString &rem);
    virtual ~Remstmt() override;
    virtual void execute() override;
};

class Letstmt : public Statement
{
    Q_OBJECT
private:
    QString leftValue;
    Expression *rightValue;
public:
    Letstmt(const QString &lv, Expression *rv);
    virtual ~Letstmt() override;
    virtual void execute() override;
public slots:
    void getVar(const QString &var, int &value, bool &varExist);
};

class Printstmt : public Statement
{
    Q_OBJECT
private:
    Expression *Exp;
public:
    Printstmt(Expression *exp);
    virtual ~Printstmt() override;
    virtual void execute() override;
public slots:
    void getVar(const QString &var, int &value, bool &varExist);
};

class Inputstmt : public Statement
{
private:
    QString Var;
public:
    Inputstmt(const QString &var);
    virtual ~Inputstmt() override;
    virtual void execute() override;
};

class Gotostmt : public Statement
{
private:
    int Line;
public:
    Gotostmt(int line);
    virtual ~Gotostmt() override;
    virtual void execute() override;
};

class Ifstmt : public Statement
{
    Q_OBJECT
private:
    Expression *Exp1;
    char Opt;
    Expression *Exp2;
    int Line;
public:
    Ifstmt(Expression *exp1, char opt, Expression *exp2, int line);
    virtual ~Ifstmt() override;
    virtual void execute() override;
public slots:
    void getVar(const QString &var, int &value, bool &varExist);
};

class Endstmt : public Statement
{
public:
    Endstmt();
    virtual ~Endstmt() override;
    virtual void execute() override;
};

#endif // STATEMENT_HH
