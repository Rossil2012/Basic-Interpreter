#ifndef CONSOLE_HH
#define CONSOLE_HH

#include <QObject>
#include <QTextEdit>
#include <QWidget>
#include <QEventLoop>

#include <QDebug>

class Console : public QTextEdit
{
    Q_OBJECT
private:
    enum {CODING, RUNNING} State;
    QString inLine;

public:
    explicit Console(QWidget *parent = nullptr);

signals:
    void newLineWritten(const QString &newline);
    void newInput();

public slots:
    void write(QString msg);
    void input(QString &in);
    void changeState(int state);

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
};

#endif // CONSOLE_HH
