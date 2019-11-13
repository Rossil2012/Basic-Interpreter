#ifndef CONSOLE_HH
#define CONSOLE_HH

#include <QObject>
#include <QTextEdit>
#include <QWidget>

class Console : public QTextEdit
{
    Q_OBJECT
public:
    explicit Console(QWidget *parent = nullptr);

signals:
    void newLineWritten(QString newline);

public slots:
    void write(QString msg);

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
};

#endif // CONSOLE_HH
