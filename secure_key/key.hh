#ifndef KEY_HH
#define KEY_HH

#include <QMainWindow>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <iostream>
#include <cstring>
#include "aes128.hh"

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
    void show_register_user_ui(void);
    void show_login_user_ui(void);
    void register_user(void);

    AES128 aes128;

};
#endif // KEY_HH
