#include "key.hh"
#include "ui_key.h"
#include "register.hh"


Key::Key(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Key)
{
    ui->setupUi(this);
    connect(ui->pushButton_login,&QPushButton::clicked,this,&Key::check_login);
    connect(ui->pushButton_register, &QPushButton::clicked, this, &Key::register_user);


    QFile file(filename);
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream out(&file);
        out << "username:hemjal\n";
        out <<"password:khan\n";
    }
    file.flush();
    file.close();

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
        this->hide();
        Register r;
        r.setModal(true);
        r.exec();
    }
    else if (not found_password){
        qDebug()<< "Password not found!";
    }
    else {

        ui->label_info->setText("Verified user!");
    }

}

void Key::register_user()
{
    ui->stackedWidget->setCurrentWidget(ui->page_register_user);
    QFile file(filename);
    if (file.open(QIODevice::ReadWrite)) {
        file.flush();
        QTextStream out(&file);
       // out << "username:hemjal\n";
       // out <<"password:khan\n";
    }
    file.flush();
    file.close();


}

