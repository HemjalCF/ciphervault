#ifndef KEY_HH
#define KEY_HH

#include <QMainWindow>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QClipboard>
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
    #define ENC_MAX_SIZE 15
    #define ENC_MIN_SIZE 6

    QString filename = "Data.txt";
    void check_login(void);
    void show_register_user_ui(void);
    void show_login_user_ui(void);
    void register_user(void);
    void test_aes128_algorithm(void);

    void show_id_passwords_page(void);
    void save_id_passwords(void);

    void show_final_page(void);
    void set_uname_password(void);
    void copy_id_passwords(void);
    void copy_pass1(void);

    AES128 aes128;

};
#endif // KEY_HH
