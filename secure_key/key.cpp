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

    //test_aes128_algorithm();

    // save webadress, id and pasword page

    connect(ui->pushButton_save, &QCommandLinkButton::clicked,this,&Key::save_id_passwords);


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
    qDebug()<<username_entered<<" and "<<password_entered;


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
            std::string stored_username_c_t = stored_username.toStdString();
            std::cout << "E Text: " << stored_username_c_t << std::endl;
            std::string stored_username_p_t = "";
            aes128.decryptAES(stored_username_c_t, stored_username_p_t);
            std::cout << "D Text: " << stored_username_p_t << std::endl;
            stored_username = QString::fromUtf8(stored_username_p_t.c_str());
            qDebug() << stored_username;
        }
        if(found_p != -1) {
            stored_password= line.mid(username_key_text.size());
            found_password = true;
            std::string stored_password_c_t = stored_password.toStdString();
            std::cout << "E Pass: " << stored_password_c_t << std::endl;
            std::string stored_password_p_t = "";
            aes128.decryptAES(stored_password_c_t, stored_password_p_t);
            stored_password = QString::fromUtf8(stored_password_p_t.c_str());
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
                ui->stackedWidget->setCurrentWidget(ui->page_save_password);
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

    if(user_username.length() < USERNAME_MIN_SIZE || \
        user_password.length() < PASSWORD_MIN_SIZE){
        ui->label_reg_info->setText("Username or Password too short!");
    }
    else if(user_username.length() > USERNAME_MAX_SIZE || \
               user_password.length() > PASSWORD_MAX_SIZE)
    {
        ui->label_reg_info->setText("Username or Password too big!");
    }
    else{
        QFile file(filename);
        if (file.open(QIODevice::ReadWrite | QIODevice::Truncate)) {  // Lets remove evrything in the file
            QTextStream out(&file);
            std::string u_f_name_p_t = user_full_name.toStdString();
            std::string u_f_name_c_t;
            aes128.encryptAES(u_f_name_p_t, u_f_name_c_t);
            qDebug()<< "Saving: u f name: " << QString::fromUtf8(u_f_name_c_t.c_str());
            out << "userfullname:"+ QString::fromUtf8(u_f_name_c_t.c_str()) + "\n";

            std::string u_name_p_t =  user_username.toStdString();
            std::string u_name_c_t;
            aes128.encryptAES(u_name_p_t, u_name_c_t);
            qDebug()<< "Saving: u name: " << QString::fromUtf8(u_name_c_t.c_str());
            out << "username:"+ QString::fromUtf8(u_name_c_t.c_str()) + "\n";

            std::string u_pass_p_t = user_password.toStdString();
            std::string u_pass_c_t;
            aes128.encryptAES(u_pass_p_t, u_pass_c_t);
            qDebug()<< "Saving: u pass: " << QString::fromUtf8(u_pass_c_t.c_str());
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
}

// Test code for checking encryption algorithm

void Key::test_aes128_algorithm()
{
    std::string test = "Test x";
    std::string res="";
    aes128.encryptAES(test,res);
    std::cout << "E Text: "<<res<<std::endl;
    std::string de_res="";
    aes128.decryptAES(res, de_res);
    std::cout << "D Text: " << de_res << std::endl;
}

void Key::save_id_passwords()
{
    QString web_id = ui->lineEdit_webadress->text();
    QString user_id = ui->lineEdit_web_id->text();
    QString user_pass = ui->lineEdit_web_password->text();
    qDebug()<< "Got web id: "<< web_id <<" User ID: "<< user_id <<" Password: "<< user_pass;
}
