#include "key.hh"
#include "ui_key.h"

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
    connect(ui->commandLinkButton_to_login, &QCommandLinkButton::clicked,this,&Key::show_login_user_ui);

    //    Test code
    //    std::string test = "My name is Khan";
    //    std::string res="";
    //    aes128.encryptAES(test,res);
    //    std::cout << "E Text: "<<res<<std::endl;
    //    std::string de_res="";
    //    aes128.decryptAES(res, de_res);
    //    std::cout << "D Text: " << de_res << std::endl;
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
    QString stored_username="",stored_password="";
    QString username_entered = ui->lineEdit_user_name_entered->text();
    QString password_entered = ui->lineEdit_password_entered->text();
    //qDebug()<<username_entered<<" and "<<password_entered;


    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        qsizetype found_u = line.lastIndexOf(username_key_text);
        qsizetype found_p = line.lastIndexOf(password_key_text);
        if(found_u != -1) {
            stored_username= line.mid(username_key_text.size());
            found_username = true;
            qDebug() << stored_username;
        }
        if(found_p != -1) {
            stored_password= line.mid(username_key_text.size());
            found_password = true;
            qDebug() << stored_password;
        }

        // qDebug()<<line;
    }
    file.close();

    if(not found_username)
    {
        qDebug()<< "username not found!";
        ui->label_info->setText("No user found!");
    }
    else if (not found_password){
        qDebug()<< "Password not found!";
        ui->label_info->setText("No user found!");
    }
    else {
        if(stored_username == username_entered){

            if(stored_password == password_entered){

                ui->label_info->setText("Verified user!");
                ui->lineEdit_user_name_entered->clear();
                ui->lineEdit_password_entered->clear();
            }
            else ui->label_info->setText("Wrong Password!");
        }
        else  ui->label_info->setText("Username Not found!");
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
        std::string u_f_name_p_t = user_full_name.toStdString();
        std::string u_f_name_c_t;
        aes128.encryptAES(u_f_name_p_t, u_f_name_c_t);
        std::cout<< "Saving: u f name: " << u_f_name_c_t;
        out << "userfullname:"+ QString::fromUtf8(u_f_name_c_t.c_str()) + "\n";

        std::string u_name_p_t =  user_username.toStdString();
        std::string u_name_c_t;
        aes128.encryptAES(u_name_p_t, u_name_c_t);
        std::cout<< "Saving: u name: " << u_name_c_t;
        out << "username:"+ QString::fromUtf8(u_name_c_t.c_str()) + "\n";

        std::string u_pass_p_t = user_password.toStdString();
        std::string u_pass_c_t;
        aes128.encryptAES(u_pass_p_t, u_pass_c_t);
        std::cout<< "Saving: u pass: " << u_pass_c_t;
        out << "password:"+ QString::fromUtf8(u_pass_c_t.c_str()) + "\n";
        qDebug()<< "******* Reg completed *********";
    }
    file.flush();
    file.close();
    ui->label_reg_info->setText("Registration Successful!");
    ui->lineEdit_full_name->clear();
    ui->lineEdit_username->clear();
    ui->lineEdit_password->clear();
}
