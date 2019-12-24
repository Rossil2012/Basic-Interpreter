#ifndef PROGRAM_HH
#define PROGRAM_HH

#include <QMap>
#include <QVector>
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
    bool de_on;
    QVector<int> bp;
    void execute();

public:
    Program();
    ~Program();
    void run();
    void clear();
    void addStmt(int line, Statement *stmt);
    void addCode(int line, const QString &code);
    void printCode();
    bool isDebugging();

public slots:
    void letVar(const QString &var, int value);
    void getVar(const QString &var, int &value, bool &varExist);
    void changeCursor(int line, bool &lineExist);
    void moveOn();
    void end();
    void terminate();

    void de_start();
    void de_stop();
    bool de_setBp(int line);
    bool de_delBp(int line);
    void de_showBp();
    void de_stepin();
    void de_continue();
    void de_display();


signals:
    void changeState(int state);
    void input(QString &in);
    void print(const QString &out);
};


#endif // PROGRAM_HH
