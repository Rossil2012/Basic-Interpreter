#ifndef TOKENIZER_HH
#define TOKENIZER_HH

#include <QString>
#include <string>
#include <sstream>
#include "program.hh"
#include "statement.hh"
#include "exp.hh"

class Tokenizer : public QObject
{
    Q_OBJECT
private:
    Program *Parent;
public:
    Tokenizer(Program *parent);
public slots:
    void parse(const QString &str);
    void safe_parse(const QString &str);
    void dynamic_parse(const QString &str);
};

#endif // TOKENIZER_HH
