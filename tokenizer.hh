#ifndef TOKENIZER_HH
#define TOKENIZER_HH

#include <QString>
#include <string>
#include <sstream>
#include "program.hh"
#include "statement.hh"
#include "exp.hh"
#include "Console.hh"

class Tokenizer : public QObject
{
    Q_OBJECT
private:
    Program *PG;
public:
    Tokenizer(Program *pg);
public slots:
    void parse(const QString &str);
signals:
    void exit_interpreter();
    void print(const QString &str);

};

#endif // TOKENIZER_HH
