#ifndef PROGRAM_HH
#define PROGRAM_HH

#include <QMap>
#include "statement.hh"
#include "evalstate.hh"

class Program : public QObject
{
    Q_OBJECT
private:
    QMap<int, Statement *> Statements;
    QMap<int, QString> Codes;
    Evalstate Vars;
    int Cursor;
    QMap<int, Statement *>::iterator ite_Cursor;
    bool normal_end;
    bool terminateFlag;
    void execute();

public:
    Program();
    ~Program();
    void run();
    void clear();
    void addStmt(int line, Statement *stmt);
    void addCode(int line, const QString &code);
    void printCode();

public slots:
    void letVar(const QString &var, int value);
    void getVar(const QString &var, int &value, bool &varExist);
    void changeCursor(int line, bool &lineExist);
    void moveOn();
    void end();
    void terminate();

signals:
    void changeState(int state);
    void input(QString &in);
    void print(const QString &out);
};


#endif // PROGRAM_HH
