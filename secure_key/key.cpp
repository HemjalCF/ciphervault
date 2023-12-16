#include "key.hh"
#include "ui_key.h"
#include "register.hh"


Key::Key(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Key)
{
    ui->setupUi(this);
    connect(ui->pushButton_login,&QPushButton::clicked,this,&Key::check_login);
    connect(ui->pushButton_register, &QPushButton::clicked,this,&Key::show_register_user_ui);
    connect(ui->pushButton_reg_insert, &QPushButton::clicked,this,&Key::register_user);
    connect(ui->pushButton_goto_reg, &QPushButton::clicked,this,&Key::show_register_user_ui);
    connect(ui->pushButton_back_to_login, &QPushButton::clicked,this,&Key::show_login_user_ui);

//    QFile file(filename);
//    if (file.open(QIODevice::ReadWrite)) {
//        QTextStream out(&file);
//        out << "ername:hemjal\n";
//        out <<"password:khan\n";
//    }
//    file.flush();
//    file.close();

}


Key::~Key()
{
    delete ui;
}

void Key::check_login()
{
    bool found_username = false, found_password = false;
    QString username_key_text = "username:";
    QString password_key_text = "password:";

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        qsizetype found_u = line.lastIndexOf(username_key_text);
        qsizetype found_p = line.lastIndexOf(password_key_text);
        if(found_u != -1) {
            QString username= line.mid(username_key_text.size());
            found_username = true;
            qDebug() << username;
        }
        if(found_p != -1) {
            QString password= line.mid(username_key_text.size());
            found_password = true;
            qDebug() << password;
        }

       // qDebug()<<line;
    }
    file.close();

    if(not found_username)
    {
        qDebug()<< "username not found!";
        ui->label_info->setText("User not found!");
    }
    else if (not found_password){
        qDebug()<< "Password not found!";
        ui->label_info->setText("Incorrect Password!");
    }
    else {

        ui->label_info->setText("Verified user!");
    }

}

void Key::show_register_user_ui()
{
    ui->stackedWidget->setCurrentWidget(ui->page_register_user);
}

void Key::show_login_user_ui()
{
    ui->stackedWidget->setCurrentWidget(ui->page);
}

void Key::register_user()
{
    QString user_full_name = ui->lineEdit_full_name->text();
    QString user_username = ui->lineEdit_username->text();
    QString user_password = ui->lineEdit_password->text();
    qDebug()<< "Entered User Full Name:" << user_full_name;
    qDebug()<< "Entered Username:" << user_username;
    qDebug()<< "Entered Password:" << user_password;
    QFile file(filename);
    if (file.open(QIODevice::ReadWrite | QIODevice::Truncate)) {  // Lets remove evrything in the file
        QTextStream out(&file);
        out << "userfullname:"+ user_full_name + "\n";
        out << "username:"+ user_username + "\n";
        out << "password:"+ user_password + "\n";
    }
    file.flush();
    file.close();
}

