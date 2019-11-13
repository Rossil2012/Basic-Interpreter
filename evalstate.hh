#ifndef EVALSTATE_HH
#define EVALSTATE_HH

#include <QMap>

class Evalstate
{
private:
    QMap<QString, int> Vars;
    int varNum;
public:
    Evalstate();
    bool getVar(const QString &var, int &value);
    void letVar(const QString &var, int value);
    void clear();
};

#endif // EVALSTATE_HH
