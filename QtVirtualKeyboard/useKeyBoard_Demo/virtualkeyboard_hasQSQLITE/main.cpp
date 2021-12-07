#include "virtualkeyboard.h"
#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QLineEdit>
void setStyleQss(QString qss_path)
{
    if(qss_path == nullptr)
         return;
     QFile qss(qss_path);
     if(!qss.open(QFile::ReadOnly))
     {
        qDebug()<<"Error: void setStyleQss()";
         return;
     }
     qApp->setStyleSheet(qss.readAll());
     qss.close();
}



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    setStyleQss(":/keyboard.qss");
    auto keyboard = VirtualKeyBoard::getKeyboard();//大小不可改变，使用 setFixeSize
    keyboard->init("/opt/UIfile/ChinesePY.db");
    keyboard->setGeometry(600,100, 400,400);
    keyboard->show();


    auto le = new QLineEdit(nullptr);
    le->setGeometry(600,600,200,40);
    le->show();


    le = new QLineEdit(nullptr);
    le->setGeometry(0,700,200,40);
    le->show();

    return a.exec();
}



