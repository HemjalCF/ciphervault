#include "key.hh"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Key w;
    w.show();
    return a.exec();
}
