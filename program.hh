#ifndef PROGRAM_HH
#define PROGRAM_HH

#include <QMap>
#include "statement.hh"
#include "evalstate.hh"

class Program : public QObject
{
private:
    QMap<int, Statement *> Statements;
    Evalstate Vars;
    int Cursor;
    QMap<int, Statement *>::iterator ite_Cursor;
    bool terminateFlag;
    void execute();
public:
    Program();
    ~Program();
    void run();
    void clear();
    void insert(int line, Statement *stmt);
public slots:
    void letVar(const QString &var, int value);
    void getVar(const QString &var, int &value, bool &varExist);
    void changeCursor(int line, bool &lineExist);
    void moveOn();
    void terminate();
};


#endif // PROGRAM_HH
