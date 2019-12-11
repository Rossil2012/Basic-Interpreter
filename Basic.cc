#include <QApplication>

#include "mainwindow.hh"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    MainWindow2 window;
    window.show();


    /*
    QMainWindow window(nullptr);
    Console input;
    input.setFixedSize(600,600);

    Program p;
    Tokenizer tk(&p);
    try {



    QObject::connect(&input, &Console::newLineWritten, &tk, &Tokenizer::safe_parse);
    window.setCentralWidget(&input);
    window.show();





        tk.parse("1 LET a = 1+2+3");
        tk.parse("500 END");
        tk.parse("200 PRINT a");
        tk.parse("400 PRINT a * 2");
        tk.parse("5 IF 1 > 0 THEN 400");







        //tk.parse("100 LET a =  ");

        //hey,there why variable do not exist?????????????
        tk.parse("200 PRINT (a + 2");
        tk.parse("500 END");



        //p.execute();
    } catch (const char *str)
    {
        qDebug() << str << endl;
    }
    */


    return a.exec();
}
