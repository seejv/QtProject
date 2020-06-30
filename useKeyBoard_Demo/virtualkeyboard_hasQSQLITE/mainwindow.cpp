#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMap>
#include <QDebug>
//最多显示6个待选汉字
#define MAX_VISIBLE (6)

VirtualKeyBoard::VirtualKeyBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    db_ = new QSqlDatabase;
    //添加数据库驱动
        if(QSqlDatabase::contains("my_sqlite"))
             *db_ = QSqlDatabase::database("my_sqlite");
         else
             *db_ = QSqlDatabase::addDatabase("QSQLITE", "my_sqlite");

    //设置数据库名称/指定数据库
    db_->setDatabaseName("ChinesePY.db");//:memory:
    //打开数据库
    if(!db_->open())
    {
        qDebug()<<db_->lastError();
        exit(-1);
    }
}

QStringList VirtualKeyBoard::getChineseList()
{
    return chinese_list_;
}

VirtualKeyBoard::~VirtualKeyBoard()
{
    delete ui;
    delete db_;
}

void VirtualKeyBoard::showChinese(const QString &key)
{


}

void VirtualKeyBoard::on_lineEdit_textChanged(const QString &arg1)
{
    chinese_list_.clear();
    if(arg1.isEmpty())
        return;
    if(QSqlDatabase::contains("my_sqlite"))
         *db_ = QSqlDatabase::database("my_sqlite");
     else
         *db_ = QSqlDatabase::addDatabase("QSQLITE", "my_sqlite");
    //打开数据库
    if(!db_->open())
    {
        qDebug()<<db_->lastError();
        return;
    }

    QString py(arg1);
    QSqlQuery query(*db_);
    int count(0);
        while(1)//根据拼音获取对因汉字/词组
        {
            const QString command = "select PinYin, Chinese from WordAndGroup where PinYin like ""'"+py+"%;'";
            if(!query.exec(command))
            {
                qDebug()<<command<<db_->lastError();
                goto error;
            }
            while(query.next())
            {
                chinese_list_.append(query.value(1).toString());
            }
            qDebug()<<command;
            if(chinese_list_.size()>0)
            {
                is_last_ok_=true;
                last_py_=py+"%,";
                break;
            }else
            {
                QString str = last_py_;
                str.remove(QRegExp("%,"));
                if(++count==1)//当查询不匹配时，将新加入的词汇当作新词的开头(eg1:guoj--> guo%,j eg2:guoji -->guo%,j%,i)
                {
                    py=last_py_+arg1.mid(str.size());
                }
                else if(count==2)//若之前可以匹配现在不行，则将新加入的字母当作新词的一部分拼音(eg1:guoj-->guo%,j eg2:guoji -->guo%ji)
                {
                    QString str2 = last_py_;
                    str2.remove(str2.lastIndexOf("%,"), 2);
                    py=str2+arg1.mid(str.size());

                }else if(count==3)//将"last_py_"最后3个字符替换成"%,"，并补充当前拼音"arg1"尚未补充在"py"上的内容
                {                 //(处理退格键，词组时的鼻音问题(mingu-->min%gu))
                    if(arg1.size()<last_py_text_.size())//按了退格键
                    {
                        last_py_.remove(last_py_.size()-3,1);
                    }
                    QString str2 = last_py_;
                    str2.remove(str2.lastIndexOf("%,"), str2.size()-str2.lastIndexOf("%,"));
                    str2.insert(str2.size()-1, "%,");
                    py=str2+arg1.mid(str.size());
                }else
                {
                    is_last_ok_=false;
                    break;
                }
            }
        }
error:
    db_->close();
    last_py_text_=arg1;
    return;
}



