#ifndef EVALSTATE_HH
#define EVALSTATE_HH

#include <QMap>
#include <QObject>

class Evalstate : public QObject
{
    Q_OBJECT
private:
    QMap<QString, int> Vars;
    int varNum;
public:
    Evalstate();
    bool getVar(const QString &var, int &value);
    void letVar(const QString &var, int value);
    void clear();
    void printVars();
signals:
    void print(const QString &str);
};

#endif // EVALSTATE_HH
