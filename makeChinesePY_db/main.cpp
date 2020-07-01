#include "mainwindow.h"
#include <QApplication>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFile>
#include <QString>
#include <QRegExp>
/*
QQPinYin.txt 有部分重复，因此创建db时会报错
";"作为结束符号 ni,hao;
            ni;
            关于词组，对照，可以到网络上去找，然后自己提取，最后再制成对应的数据库
*/


void creatWordGroup(QString PinYin_txt, QString sqlpath)
{
    QFile file(PinYin_txt);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    //添加数据库驱动
        QSqlDatabase db;
        if(QSqlDatabase::contains("my_sqlite"))
             db = QSqlDatabase::database("my_sqlite");
         else
             db = QSqlDatabase::addDatabase("QSQLITE", "my_sqlite");

    //设置数据库名称
//    db.setDatabaseName(sqlpath);
    db.setDatabaseName(sqlpath);//:memory:

    //打开数据库
    if(!db.open())
    {
        return;
    }

    QStringList list{};
    QStringList listPY{};
    QString command{};
    QTextStream in(&file);
    QString savsql("INSERT INTO WordAndGroup(PinYin, Chinese)");
//    QString savsql("INSERT INTO WordGroup(PinYin, Chinese)");
    //以下执行相关sql语句
    QSqlQuery query(db);

    //新建 PinyinChinese 表，id设置为主键，还有一个 Chinese 项
    command = "create table if not exists WordAndGroup(PinYin TEXT NOT NULL, Chinese TEXT NOT NULL, PRIMARY KEY(PinYin, Chinese))";
//    command = "create table if not exists WordGroup(PinYin TEXT NOT NULL, Chinese TEXT NOT NULL, PRIMARY KEY(PinYin, Chinese))";
    if(!query.exec(command))
    {
        qDebug()<<__func__<<command<<query.lastError();
    }
    while (!in.atEnd())
    {
        QString line = in.readLine();
        list = line.split(QRegExp("\\s"), QString::SkipEmptyParts);//匹配空白字符 (\\d)|(\\t) (\\d|\\t)配置制定的多种格式,去除空项
        listPY = list.at(1).split(QRegExp("-"));
        if(list.size()==2 && listPY.size()==2)//仅仅添加两个词的词组
        {
            QString py(listPY.at(0));

            for(int i(1); i<listPY.size(); i++)
            {
                py = py + "," + listPY.at(i);
            }
            command = QString(" VALUES('%1;', '%2')").arg(py).arg(list.at(0));
            if(!query.exec(savsql+command))qDebug()<<__func__<<command<<query.lastError();
         }
    }
    file.close();
    db.close();
    qDebug()<<__func__<<"cucessed";
}

void creatWord(QString PinYin_txt, QString sqlpath)
{
    QFile file(PinYin_txt);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    //添加数据库驱动
        QSqlDatabase db;
        if(QSqlDatabase::contains("my_sqlite"))
             db = QSqlDatabase::database("my_sqlite");
         else
             db = QSqlDatabase::addDatabase("QSQLITE", "my_sqlite");

    //设置数据库名称
//    db.setDatabaseName(sqlpath);
    db.setDatabaseName(sqlpath);//:memory:

    //打开数据库
    if(!db.open())
    {
        return;
    }

    QStringList list{};
    QString command{};
    QTextStream in(&file);
    QString savsql("INSERT INTO WordAndGroup(PinYin, Chinese)");
//    QString savsql("INSERT INTO Word(PinYin, Chinese)");
    //以下执行相关sql语句
    QSqlQuery query(db);

    //新建 PinyinChinese 表，id设置为主键，还有一个 Chinese 项
    command = "create table if not exists WordAndGroup(PinYin TEXT NOT NULL, Chinese TEXT NOT NULL, PRIMARY KEY(PinYin, Chinese))";
//  command = "create table if not exists Word(PinYin TEXT NOT NULL, Chinese TEXT NOT NULL, PRIMARY KEY(PinYin, Chinese))";
    if(!query.exec(command))
    {
        qDebug()<<__func__<<command<<query.lastError();
    }
    while (!in.atEnd())
    {
        QString line = in.readLine();
        list = line.split(QRegExp("\\s"), QString::SkipEmptyParts);//匹配空白字符 (\\d)|(\\t) (\\d|\\t)配置制定的多种格式,去除空项
//        qDebug()<<list;
//        break;
        switch (list.size())
        {
            case 5:
            {
                command = QString(" VALUES('%1;', '%2')").arg(list.at(4)).arg(list.at(0));
                if(!query.exec(savsql+command))qDebug()<<__func__<<command<<query.lastError();
                [[clang::fallthrough]];//使编译器不发出警告。
            }
            case 4:
            {
                command = QString(" VALUES('%1;', '%2')").arg(list.at(3)).arg(list.at(0));
                if(!query.exec(savsql+command))qDebug()<<__func__<<command<<query.lastError();
                [[clang::fallthrough]];
            }
            case 3:
            {
                command = QString(" VALUES('%1;', '%2')").arg(list.at(2)).arg(list.at(0));
                if(!query.exec(savsql+command))qDebug()<<__func__<<command<<query.lastError();
                [[clang::fallthrough]];
            }

            case 2:
            {
                command = QString(" VALUES('%1;', '%2')").arg(list.at(1)).arg(list.at(0));
                if(!query.exec(savsql+command))qDebug()<<__func__<<command<<query.lastError();
            }
        }
    }
    file.close();
    db.close();
    qDebug()<<__func__<<"cucessed";
}



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    creatWord("7000PinYin.txt", "ChinesePY.db");
    creatWordGroup("./phrasePinYin.txt", "ChinesePY.db");

//    QFile file("./phrasePinYin.txt");
//    if(!file.open(QIODevice::ReadWrite|QIODevice::Text))return -1;
//    QTextStream in(&file);
//    QString line{};
//    QString text{};
//    QStringList list{};
//    while(!in.atEnd())
//    {
//        line = in.readLine();
//        if(line.isEmpty())continue;
//        list = line.split(QRegExp("\\s"));
//        if(list.at(0).size()==1)
//            continue;
//        text = text+line+"\n";
//    }
//    file.close();
//    QFile file2("./phrasePinYin2.txt");
//    if(!file2.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Truncate))return -1;
//    {
//        file2.write(text.toUtf8());
//    }
//    file2.close();

    return a.exec();

}




















