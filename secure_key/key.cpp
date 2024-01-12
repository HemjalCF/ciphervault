#include "key.hh"
#include "ui_key.h"

Key::Key(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Key)
{
    ui->setupUi(this);
    connect(ui->pushButton_login,&QPushButton::clicked,this,&Key::check_login);  // check for login info
    connect(ui->pushButton_register, &QPushButton::clicked,this,&Key::show_register_user_ui); // show register user UI
    connect(ui->pushButton_reg_insert, &QPushButton::clicked,this,&Key::register_user);       // register a user
    connect(ui->pushButton_goto_reg, &QPushButton::clicked,this,&Key::show_register_user_ui);
    connect(ui->pushButton_back_to_login, &QPushButton::clicked,this,&Key::show_login_user_ui);
    connect(ui->commandLinkButton_to_login, &QCommandLinkButton::clicked,this,&Key::show_login_user_ui);


    //test_aes128_algorithm();

    // save webadress, id and pasword page

    connect(ui->pushButton_save, &QCommandLinkButton::clicked,this,&Key::save_id_passwords);
    connect(ui->pushButton_add_new_pass_page, &QPushButton::clicked,this,&Key::show_id_passwords_page);
    connect(ui->pushButton_goto_final_page, &QPushButton::clicked,this,&Key::show_final_page);

    // copy password

    connect(ui->pushButton_copy_pass1, &QPushButton::clicked,this,&Key::copy_id_passwords );
    connect(ui->pushButton_copy_pass2, &QPushButton::clicked,this,&Key::copy_id_passwords );
    connect(ui->pushButton_copy_pass3, &QPushButton::clicked,this,&Key::copy_id_passwords );
    connect(ui->pushButton_copy_pass4, &QPushButton::clicked,this,&Key::copy_id_passwords );
    connect(ui->pushButton_copy_pass5, &QPushButton::clicked,this,&Key::copy_id_passwords );
    connect(ui->pushButton_logout, &QPushButton::clicked,this,&Key::show_login_user_ui);

}


Key::~Key()
{
    delete ui;
}

void Key::print_database(std::map<webstr,std::map<id_str,pass_str>> db)
{
    qDebug() << "Printing application database!";
    if(db.empty()){
        qDebug() << "Datbase Empty";
        return;
    }
    else
    {
        qDebug() << "Total size: "<< db.size();
        for (auto itr = db.begin(); itr != db.end(); ++itr) {
            qDebug() << "User full name: "<< itr->first; // should contain key name
            auto i_p = itr->second; // should contain saved id and password
            for (auto uid_itr = i_p.begin(); uid_itr != i_p.end(); ++uid_itr) {
                    qDebug() << "Username: "<<uid_itr->first;
                    qDebug() << "Password: "<<uid_itr->second;
            }
        }
    }
}

void Key::check_login()
{
    bool found_user_f_name = false,found_username = false, found_password = false;
    QString user_f_name_key_text = "userfullname:";
    QString username_key_text = "username:";
    QString password_key_text = "password:";
    QString stored_user_f_name="",stored_username="",stored_password="";
    QString username_entered = ui->lineEdit_user_name_entered->text();
    QString password_entered = ui->lineEdit_password_entered->text();
    qDebug()<<username_entered<<" and "<<password_entered;


    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        qsizetype found_u_f = line.lastIndexOf(user_f_name_key_text);
        qsizetype found_u = line.lastIndexOf(username_key_text);
        qsizetype found_p = line.lastIndexOf(password_key_text);
        if(found_u_f != -1) {
            stored_username= line.mid(user_f_name_key_text.size());
            found_user_f_name = true;
            std::string stored_user_f_name_c_t = stored_username.toStdString();
            std::cout << "E Text: " << stored_user_f_name_c_t << std::endl;
            std::string stored_user_f_name_p_t = "";
            aes128.decryptAES(stored_user_f_name_c_t, stored_user_f_name_p_t);
            std::cout << "D Text: " << stored_user_f_name_p_t << std::endl;
            stored_user_f_name = QString::fromUtf8(stored_user_f_name_p_t.c_str());
            qDebug() << stored_user_f_name;
        }
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
            uid[stored_user_f_name].emplace(stored_username,stored_password);
            print_database(uid);
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
                set_uname_password();
                ui->stackedWidget->setCurrentWidget(ui->page_final);
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
    QString full_name="",username="",password="";
    QString user_full_name = ui->lineEdit_full_name->text();
    QString user_username = ui->lineEdit_username->text();
    QString user_password = ui->lineEdit_password->text();
    qDebug()<< "Entered User Full Name:" << user_full_name;
    qDebug()<< "Entered Username:" << user_username;
    qDebug()<< "Entered Password:" << user_password;

    if(user_username.length() < ENC_MIN_SIZE || \
        user_password.length() < ENC_MIN_SIZE){
        ui->label_reg_info->setText("Username or Password too short!");
    }
    else if(user_username.length() > ENC_MAX_SIZE || \
               user_password.length() > ENC_MAX_SIZE)
    {
        ui->label_reg_info->setText("Username or Password too big!");
    }
    else{
        QFile file(filename);
        if (file.open(QIODevice::ReadWrite | QIODevice::Truncate)) {  // Lets remove evrything in the file
            QTextStream out(&file);
            uid.clear(); // lets clear database first
            std::string u_f_name_p_t = user_full_name.toStdString();
            std::string u_f_name_c_t;
            aes128.encryptAES(u_f_name_p_t, u_f_name_c_t);
            full_name = QString::fromUtf8(u_f_name_c_t.c_str());
            qDebug()<< "Saving: u f name: " << full_name;
            out << "userfullname:"+ full_name + "\n";

            std::string u_name_p_t =  user_username.toStdString();
            std::string u_name_c_t;
            aes128.encryptAES(u_name_p_t, u_name_c_t);
            username = QString::fromUtf8(u_name_c_t.c_str());
            qDebug()<< "Saving: u name: " << username;
            out << "username:"+ username + "\n";

            std::string u_pass_p_t = user_password.toStdString();
            std::string u_pass_c_t;
            aes128.encryptAES(u_pass_p_t, u_pass_c_t);
            password = QString::fromUtf8(u_pass_c_t.c_str());
            qDebug()<< "Saving: u pass: " << password;
            out << "password:"+ password + "\n";
            uid[full_name][username]= password;
            qDebug()<< "******* Reg. completed *********";
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

void Key::show_id_passwords_page()
{
    ui->stackedWidget->setCurrentWidget(ui->page_save_password);
}

void Key::save_id_passwords()
{
    if(wids.size()>5){
        ui->label_show_save_password_status->setText("No storage left!");
        return;
    }
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream in(&file);
    int i=0;
    while (!in.atEnd()) {
        QString line = in.readLine();
        qDebug()<<"found ID: "<<i<<" "<<line;
        i++;
    }
    qDebug()<< "******* Reading completed *********";

    file.close();

    QString web_id = ui->lineEdit_webadress->text();
    QString user_id = ui->lineEdit_web_id->text();
    QString user_pass = ui->lineEdit_web_password->text();

    if(web_id.length() < ENC_MIN_SIZE || \
        user_id.length() < ENC_MIN_SIZE || \
        user_pass.length() < ENC_MIN_SIZE){
        ui->label_reg_info->setText("Credentials too Short!");
    }
    else if(web_id.length() > ENC_MAX_SIZE || \
            user_id.length() > ENC_MAX_SIZE || \
            user_pass.length() > ENC_MAX_SIZE){
        ui->label_reg_info->setText("Credentials too Big!");
    }
    else{
        qDebug()<< "Got web id: "<< web_id <<" User ID: "<< user_id <<" Password: "<< user_pass;
        std::string web_id_c_t= "",user_id_c_t = "", user_pass_c_t = "";
        aes128.encryptAES(web_id.toStdString(), web_id_c_t);
        aes128.encryptAES(user_id.toStdString(), user_id_c_t);
        aes128.encryptAES(user_pass.toStdString(), user_pass_c_t);

        if (file.open(QIODevice::ReadWrite | QIODevice::Append)) {  // Lets add in the file
            QTextStream out(&file);            
            out << "web_adress:"+ QString::fromUtf8(web_id_c_t.c_str()) + "\n";
            out << "web_user_id:"+ QString::fromUtf8(user_id_c_t.c_str()) + "\n";
            out << "web_user_pass:"+ QString::fromUtf8(user_pass_c_t.c_str()) + "\n";
            wids[web_id][user_id] = user_pass;
            qDebug()<< "******* Saving completed *********";
        }
        file.flush();
        file.close();
        ui->label_show_save_password_status->setText("Password Saved!");
        ui->lineEdit_webadress->clear();
        ui->lineEdit_web_id->clear();
        ui->lineEdit_web_password->clear();
    }
}

void Key::show_final_page()
{
    ui->stackedWidget->setCurrentWidget(ui->page_final);
    set_uname_password();
}

void Key::set_uname_password()
{
    qDebug()<<"Testing: set user name and password ";
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString web1_key_text = "web_adress";
    QString usr1_key_text = "web_user_id";
    QString pass1_key_text = "web_user_pass";
    QString web_addr="", web_id ="", web_pass ="";

    int i=1;
    while (!in.atEnd()) {
        QString line = in.readLine();
        if(line.indexOf(web1_key_text) != -1){
            qDebug()<<"found: "<<i<<" "<<line;
            QString web_addr_enc = line.mid(web1_key_text.size()+1);
            std::string web_addr_dec = "";
            aes128.decryptAES(web_addr_enc.toStdString(), web_addr_dec);
            web_addr = QString::fromUtf8(web_addr_dec.c_str());
           // ui->label_web1->setText(web_addr);
        }
        else if(line.indexOf(usr1_key_text) != -1){
            qDebug()<<"found: "<<i<<" "<<line;
            QString web_usr_enc = line.mid(usr1_key_text.size()+1);
            std::string web_usr_dec = "";
            aes128.decryptAES(web_usr_enc.toStdString(), web_usr_dec);
            web_id = QString::fromUtf8(web_usr_dec.c_str());
            //ui->label_uid_1->setText(web_id);
        }
        else if(line.indexOf(pass1_key_text) != -1){
            qDebug()<<"found: "<<i<<" "<<line;
            QString web_pass_enc = line.mid(pass1_key_text.size()+1);
            std::string web_pass_dec = "";
            aes128.decryptAES(web_pass_enc.toStdString(), web_pass_dec);
            web_pass = QString::fromUtf8(web_pass_dec.c_str());
            //ui->label_pass1->setText(web_pass);
            wids[web_addr][web_id] = web_pass;
        }
        i++;
    }
    print_database(wids);
    int j=1;
    for (auto wid_itr = wids.begin(); wid_itr != wids.end(); ++wid_itr) {
        auto ids_m = wid_itr->second;
        for (auto uid_itr = ids_m.begin(); uid_itr != ids_m.end(); ++uid_itr) {
            auto id =uid_itr->first;
            auto pass = uid_itr->second;
            switch (j) {
            case 1:
                ui->label_web1->setText(wid_itr->first);
                ui->label_uid_1->setText(id);
                ui->label_pass1->setText(pass);
                break;
            case 2:
                ui->label_web2->setText(wid_itr->first);
                ui->label_uid_2->setText(id);
                ui->label_pass2->setText(pass);
                break;
            case 3:
                ui->label_web3->setText(wid_itr->first);
                ui->label_uid_3->setText(id);
                ui->label_pass3->setText(pass);
                break;
            case 4:
                ui->label_web4->setText(wid_itr->first);
                ui->label_uid_4->setText(id);
                ui->label_pass4->setText(pass);
                break;
            case 5:
                ui->label_web5->setText(wid_itr->first);
                ui->label_uid_5->setText(id);
                ui->label_pass5->setText(pass);
                break;
            default:
                break;
            }
        }
        j++;
    }
    qDebug()<< "******* Displaying completed *********";
    file.close();
}

void Key::copy_id_passwords(void)
{
    int pass_but = 0;
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString text_to_be_copied = "";
    // ui->label_copy_status->setText("Copied!");
    QObject* senderObj = sender();
    if (senderObj->isWidgetType())
    {
        QPushButton* button = qobject_cast<QPushButton*>(senderObj);
        qDebug()<<"Signal received from: "<<button->objectName();
        if(QString::compare(button->objectName(), "pushButton_copy_pass1") == 0) pass_but = 1;
        else if(QString::compare(button->objectName(), "pushButton_copy_pass2") == 0) pass_but = 2;
        else if(QString::compare(button->objectName(), "pushButton_copy_pass3") == 0) pass_but = 3;
        else if(QString::compare(button->objectName(), "pushButton_copy_pass4") == 0) pass_but = 4;
        else if(QString::compare(button->objectName(), "pushButton_copy_pass5") == 0) pass_but = 5;
    }

    switch (pass_but) {
    case 1:
        qDebug()<<"Signal received from: button 1";
        text_to_be_copied = ui->label_pass1->text();
        break;
    case 2:
        qDebug()<<"Signal received from: button 2";
        text_to_be_copied = ui->label_pass2->text();
        break;
    case 3:
        qDebug()<<"Signal received from: button 3";
        text_to_be_copied = ui->label_pass3->text();
        break;
    case 4:
        qDebug()<<"Signal received from: button 4";
        text_to_be_copied = ui->label_pass4->text();
        break;
    case 5:
        qDebug()<<"Signal received from: button 5";
        text_to_be_copied = ui->label_pass5->text();
        break;

    default:

        break;
    }
    clipboard->setText(text_to_be_copied);
}
