#ifndef CONSOLE_HH
#define CONSOLE_HH

#include <QObject>
#include <QTextEdit>
#include <QWidget>
#include <QEventLoop>

class Console : public QTextEdit
{
    Q_OBJECT
private:
    enum {CODING, RUNNING, DEBUGGING} State;
    QString inLine;
    bool isInputting;

public:
    explicit Console(QWidget *parent = nullptr);

signals:
    void newLineWritten(const QString &newline);
    void newInput();
    void exit_interpreter();

public slots:
    void write(const QString &msg);
    void input(QString &in);
    void changeState(int state);
    void Exit_interpreter();

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
};

#endif // CONSOLE_HH
