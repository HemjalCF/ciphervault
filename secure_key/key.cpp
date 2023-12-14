#include "key.hh"
#include "ui_key.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <iostream>
#include <cstring>

Key::Key(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Key)
{
    ui->setupUi(this);
    connect(ui->pushButton_login,&QPushButton::clicked,this,&Key::check_login);

    QString filename = "Data.txt";
    QFile file(filename);
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream << "something\n";
    }
    file.close();

}


Key::~Key()
{
    delete ui;
}

void Key::check_login()
{
    qDebug()<<"clicked\n";
}

