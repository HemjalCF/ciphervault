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

    const unsigned char key[17] = "0123456789abcdef"; // 16-byte key for AES-128
    const unsigned char plaintext[12] = "Hello, AES!";
    unsigned char ciphertext[AES_BLOCK_SIZE];
    unsigned char decryptedText[AES_BLOCK_SIZE];

    aes128.encryptAES(plaintext, key, ciphertext);
    aes128.decryptAES(ciphertext, key, decryptedText);

    std::cout << "Original Text: " << plaintext << std::endl;
    std::cout << "Encrypted Text: ";
    for (int i = 0; i < AES_BLOCK_SIZE; ++i)
        std::cout << std::hex << (int)ciphertext[i];
    std::cout << std::endl;

    std::cout << "Decrypted Text: " << decryptedText << std::endl;
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
        out << "userfullname:"+ user_full_name + "\n";
        out << "username:"+ user_username + "\n";
        out << "password:"+ user_password + "\n";
    }
    file.flush();
    file.close();
    ui->label_reg_info->setText("Registration Successful!");
    ui->lineEdit_full_name->clear();
    ui->lineEdit_username->clear();
    ui->lineEdit_password->clear();
}

