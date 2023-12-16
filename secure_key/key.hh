#ifndef KEY_HH
#define KEY_HH

#include <QMainWindow>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <iostream>
#include <cstring>

QT_BEGIN_NAMESPACE
namespace Ui { class Key; }
QT_END_NAMESPACE

class Key : public QMainWindow
{
    Q_OBJECT

public:
    Key(QWidget *parent = nullptr);
    ~Key();

private:
    Ui::Key *ui;
    QString filename = "Data.txt";
    void check_login(void);
    void register_user(void);

};
#endif // KEY_HH
