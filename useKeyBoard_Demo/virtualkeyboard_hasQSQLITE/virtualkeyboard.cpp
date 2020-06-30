#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMap>
#include <QDebug>
#include <QPushButton>
#include <QList>
#include <QRegExp>
#include <QLineEdit>
#include <QShowEvent>
#include <QHideEvent>
#include <QLabel>
#include "virtualkeyboard.h"
#include "ui_virtualkeyboard.h"

#include <QApplication>


//#include <QTime>
//void static msecSleep(int msec)
//{
//    QTime dieTime = QTime::currentTime().addMSecs(msec);
//    while( QTime::currentTime() < dieTime )
//        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
//}

//最多显示6个待选汉字
#define MAX_VISIBLE (6)

VirtualKeyBoard* VirtualKeyBoard::g_keyboard = nullptr;


VirtualKeyBoard::VirtualKeyBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VirtualKeyBoard)
{

    QFile qss(":/keyboard.qss");
    if(!qss.open(QFile::ReadOnly))
    {
        qDebug()<<"Error: void setStyleQss()"<<qss.errorString();
        //         return;
    }
    this->setStyleSheet(qss.readAll());
    qss.close();

    ui->setupUi(this);

    setWindowFlags(Qt::Tool | Qt::WindowDoesNotAcceptFocus | Qt::FramelessWindowHint);

    init1();

    connect(qApp, &QApplication::focusChanged, this, &VirtualKeyBoard::focusChangedSlot);

}

VirtualKeyBoard* VirtualKeyBoard::getKeyboard()
{
    qDebug()<<__func__;
    if (g_keyboard==nullptr) {
        g_keyboard = new VirtualKeyBoard;
    }
    return g_keyboard;
}

VirtualKeyBoard::~VirtualKeyBoard()
{
    delete ui;
    delete db_;
}


//void VirtualKeyBoard::setAutoShow(QObject *input_widget)
//{
//    if(input_widget==nullptr)
//    {
//        qDebug()<<__func__<<"error: le is nullptr";
//        return;
//    }

//    connect(input_widget, SIGNAL(curTextSignal(QString, QString)), this, SLOT(show(QString, QString)));
//    connect(this, SIGNAL(changedOutSideTextSignal(QString, QString)), input_widget, SLOT(setTextSlot(QString, QString)));
//    connect(this, SIGNAL(setDefaultColorSignal()), input_widget, SLOT( recvKeyBoardHideSlot()));
//}





void VirtualKeyBoard::showEvent(QShowEvent *event)
{
    setSymbolPage(en0);
    return QWidget::showEvent(event);
}

void VirtualKeyBoard::hideEvent(QHideEvent *event)
{

    mode_flag_map_[is_ch_mode]=false;
    symbal_page_ = en0;
    clearBuffer();
    return QWidget::hideEvent(event);
}


void VirtualKeyBoard::init(const QString& db_path)
{
    qDebug()<<__func__;
    init2(db_path);
}

void VirtualKeyBoard::init1()
{
    qDebug()<<__func__;
    lb_display_text_ = new QLabel(nullptr);
    lb_display_text_->setWindowFlags( Qt::WindowDoesNotAcceptFocus | Qt::FramelessWindowHint);
    lb_display_text_->setObjectName("lb_display_text");
    lb_display_text_->setScaledContents(true);
    lb_display_text_->setText(" "); //使能样式生效
    lb_display_text_->setText("");


    mode_flag_map_[is_ch_mode]=false;
    mode_flag_map_[is_capital_mode]=false;


    ////添加每行的按钮
    QList<QPushButton*>btn_list{};
    //汉字显示行
    btn_list<<(ui->btn0_left)<<(ui->btn0_02)<<(ui->btn0_03)<<(ui->btn0_04)<<(ui->btn0_05)
           <<(ui->btn0_06)<<(ui->btn0_07)<<(ui->btn0_08)<<(ui->btn0_09)<<(ui->btn0_right);
    btn_list_map_[row_1] = btn_list;
    btn_list.clear();

    //键盘第一行
    btn_list<<(ui->btn1_01)<<(ui->btn1_02)<<(ui->btn1_03)<<(ui->btn1_04)<<(ui->btn1_05)
           <<(ui->btn1_06)<<(ui->btn1_07)<<(ui->btn1_08)<<(ui->btn1_09)<<(ui->btn1_10)
          <<(ui->btn1_backspace);
    btn_list_map_[row_2] = btn_list;
    btn_list.clear();

    //键盘第二行
    btn_list<<(ui->btn2_01)<<(ui->btn2_02)<<(ui->btn2_03)<<(ui->btn2_04)<<(ui->btn2_05)
           <<(ui->btn2_06)<<(ui->btn2_07)<<(ui->btn2_08)<<(ui->btn2_09)<<(ui->btn2_return);
    btn_list_map_[row_3] = btn_list;
    btn_list.clear();

    //键盘第三行
    btn_list<<(ui->btn3_Lshirt)<<(ui->btn3_02)<<(ui->btn3_03)<<(ui->btn3_04)<<(ui->btn3_05)
           <<(ui->btn3_06)<<(ui->btn3_07)<<(ui->btn3_08)<<(ui->btn3_09)<<(ui->btn3_10)
          <<(ui->btn3_Rshirt);
    btn_list_map_[row_4] = btn_list;
    btn_list.clear();

    //键盘第四行
    btn_list<<(ui->btn4_symbol)<<(ui->btn4_switch)<<(ui->btn4_space)<<(ui->btn4_01)<<(ui->btn4_hide);
    btn_list_map_[row_5] = btn_list;
    btn_list.clear();

    ////信号和槽
    foreach(int key, btn_list_map_.keys())
    {
        QList<QPushButton*> list(btn_list_map_[key]);
        for(int i(0); i<list.size(); i++)
        {
            if(key==row_1 && i>0 && i<list.size()-1)//待选汉字
            {
                //              connect(list.at(i), SIGNAL(clicked(QString)), this, SLOT(selectChinese(QString)));
                connect(list.at(i), &QPushButton::clicked, this, &VirtualKeyBoard::selectChinese);
            }else
            {
                //              connect(list.at(i), SIGNAL(clicked(QString)), this, SLOT(recKeyClicked(QString)));
                connect(list.at(i), &QPushButton::clicked, this, &VirtualKeyBoard::recKeyClickedSlot);
            }
        }
    }

}

void VirtualKeyBoard::init2(const QString& db_path)
{
    qDebug()<<__func__;
    qDebug()<<QSqlDatabase::drivers();
    ////设置数据库，用于汉字输出
    QString runPath = QCoreApplication::applicationDirPath();
    db_ = new QSqlDatabase;
    //添加数据库驱动
    if(QSqlDatabase::contains("my_sqlite"))
        *db_ = QSqlDatabase::database("my_sqlite");
    else
        *db_ = QSqlDatabase::addDatabase("QSQLITE", "my_sqlite");

    //设置数据库名称/指定数据库
    db_->setDatabaseName(db_path);//:memory:

    //打开数据库
    if(!db_->open())
    {
        qDebug()<<__func__<<db_->lastError();
        if (!db_path.isEmpty()) //表示暂时不需要用到汉字
        {
            exit(-1);
        }

    }

    ////设置汉字显示个数
    QList<QPushButton*>btn_ch_list = btn_list_map_[row_1];
    for(int i(MAX_VISIBLE+1); i<btn_ch_list.size()-1; i++)//"<<" ">>"两个按钮
    {
        btn_ch_list.at(i)->hide();
    }
    clearBuffer();
    QStringList en0list{};en0list<<"q"<<"w"<<"e"<<"r"<<"t"<<"y"<<"u"<<"i"<<"o"<<"p"<<"BS"
                                <<"a"<<"s"<<"d"<<"f"<<"g"<<"h"<<"j"<<"k"<<"l"<<"ok"//return -- OK
                               <<"Ls"<<"z"<<"x"<<"c"<<"v"<<"b"<<"n"<<"m"<<","<<"."<<"Rs"//Ls:Lshift, Rs:RLshift
                              <<"?123"<<"sw"<<"English"<<":"<<"hi";//sw:switch hi:hide
    QStringList en1list{};en1list<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8"<<"9"<<"0"<<"BS"
                                <<"."<<"@"<<"~"<<"-"<<","<<":"<<"*"<<"?"<<"!"<<"_"
                               <<"Le"<<"#"<<"/"<<"="<<"+"<<"﹉"<<"&&"<<"^"<<";"<<"%"<<"Ri"
                              <<"ABC"<<"sw"<<"English"<<","<<"hi";//sp:space
    QStringList en2list{};en2list<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8"<<"9"<<"0"<<"BS"
                                <<"…"<<"$"<<"("<<")"<<".."<<"<"<<">"<<"|"<<"."<<"¥"
                               <<"Le"<<"["<<"]"<<"\""<<"{"<<"}"<<"–"<<"'"<<"€"<<"\\"<<"Ri" //Le:left  Ri:Right
                              <<"ABC"<<"sw"<<"English"<<"."<<"hi";
    //    QStringList en2list{};en2list<<"…"<<"$"<<"("<<")"<<".."<<"<"<<">"<<"|"<<"."<<"¥"<<"BS"
    //                                 <<"["<<"]"<<"\""<<"{"<<"}"<<"–"<<"'"<<"€"<<""<<""
    //                                 <<"Le"<<""<<""<<""<<""<<""<<""<<""<<""<<""<<"Ri"//Le:left  Ri:Right
    //                                 <<"ABC"<<"sw"<<"English"<<""<<"hi";
    //，。？！～、：＃；％＊——……＆·＄（）‘’“”[]『』〔〕｛｝【】￥￡‖〖〗《》「」

    QStringList ch0list{};ch0list<<"q"<<"w"<<"e"<<"r"<<"t"<<"y"<<"u"<<"i"<<"o"<<"p"<<"BS"
                                <<"a"<<"s"<<"d"<<"f"<<"g"<<"h"<<"j"<<"k"<<"l"<<"ok"
                               <<"Ls"<<"z"<<"x"<<"c"<<"v"<<"b"<<"n"<<"m"<<"，"<<"。"<<"Rs"
                              <<"?123"<<"sw"<<"Chinese"<<"/"<<"hi";
    QStringList ch1list{};ch1list<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8"<<"9"<<"0"<<"BS"
                                <<"，"<<"。"<<"！"<<"～"<<"、"<<"："<<"＃"<<"；"<<"％"<<"＊"
                               <<"Le"<<"——"<<"……"<<"＆"<<"·"<<"＄"<<"（"<<"）"<<"‘"<<"’"<<"Ri"
                              <<"ABC"<<"sw"<<"Chinese"<<"‖"<<"hi";
    QStringList ch2list{};ch2list<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8"<<"9"<<"0"<<"BS"
                                <<"“"<<"”"<<"["<<"]"<<"『"<<"』"<<"〔"<<"〕"<<"｛"<<"｝"
                               <<"Le"<<"【"<<"】"<<"￡"<<"〖"<<"〗"<<"《"<<"》"<<"「"<<"」"<<"Ri"
                              <<"ABC"<<"sw"<<"Chinese"<<"￥"<<"hi";
    //    QStringList ch2list{};ch2list<<"“"<<"”"<<"["<<"]"<<"『"<<"』"<<"〔"<<"〕"<<"｛"<<"｝"<<"BS"
    //                                 <<"【"<<"】"<<"￥"<<"￡"<<"‖"<<"〖"<<"〗"<<"《"<<"》"<<"「"
    //                                 <<"Le"<<"」"<<""<<""<<""<<""<<""<<""<<""<<""<<"Ri"
    //                                 <<"ABC"<<"sw"<<"Chinese"<<"」"<<"hi";
    symbol_list_map_[en0]=en0list;
    symbol_list_map_[en1]=en1list;
    symbol_list_map_[en2]=en2list;
    symbol_list_map_[ch0]=ch0list;
    symbol_list_map_[ch1]=ch1list;
    symbol_list_map_[ch2]=ch2list;


}

QMap<int, QStringList> VirtualKeyBoard::getChineseListMap(const QString &cur_text)
{
    //    QStringList chinese_list{};
    QMap<int, QStringList>list_map{};
    int page(0);
    int index(-1);//计数是否达到最大显示数

    if(cur_text.isEmpty())
        return list_map;
    if(QSqlDatabase::contains("my_sqlite"))
        *db_ = QSqlDatabase::database("my_sqlite");
    else
        *db_ = QSqlDatabase::addDatabase("QSQLITE", "my_sqlite");
    //打开数据库
    if(!db_->open())
    {
        qDebug()<<__func__<<db_->lastError();
        return list_map;
    }
    QString py(cur_text);
    QSqlQuery query(*db_);
    int count(0);
    if( cur_text.size()<last_py_text_.size() && !last_py_.isEmpty())//按了退格键
    {
        last_py_.remove(last_py_.size()-3,1);
    }

    while(1)//根据拼音获取对因汉字/词组
    {
        const QString command = "select PinYin, Chinese from WordAndGroup where PinYin like ""'"+py+"%;'";
        if(!query.exec(command))
        {
            qDebug()<<__func__<<command<<db_->lastError();
            goto error;
        }

        while(query.next())
        {
            if(index++<MAX_VISIBLE-1)
            {
                list_map[page].append(query.value(1).toString());
            }else
            {
                page++;
                index = -1;
            }
        }
        if(list_map[page].size()>0)
        {
            last_py_=py+"%,";
            break;
        }else
        {
            QString str = last_py_;
            str.remove(QRegExp("%,"));
            if(++count==1)//当查询不匹配时，将新加入的词汇当作新词的开头(eg1:guoj--> guo%,j eg2:guoji -->guo%,j%,i)
            {
                py=last_py_+cur_text.mid(str.size());
            }
            else if(count==2)//若之前可以匹配现在不行，则将新加入的字母当作新词的一部分拼音(eg1:guoj-->guo%,j eg2:guoji -->guo%ji)
            {
                QString str2 = last_py_;
                str2.remove(str2.lastIndexOf("%,"), 2);
                py=str2+cur_text.mid(str.size());

            }else if(count==3)//将"last_py_"最后3个字符替换成"%,"，并补充当前拼音"arg1"尚未补充在"py"上的内容
            {
                QString str2 = last_py_;
                str2.remove(str2.lastIndexOf("%,"), str2.size()-str2.lastIndexOf("%,"));
                str2.insert(str2.size()-1, "%,");
                py=str2+cur_text.mid(str.size());
            }else
            {
                break;
            }
        }
    }
    //        qDebug()<<chinese_list;
error:
    db_->close();
    last_py_text_=cur_text;

    return list_map;
}


void VirtualKeyBoard::clearBuffer()
{
    cur_py_text_.clear();
    cur_py_page_=-1;
    //    ui->lb_display_text->setText("");
    lb_display_text_->setText("");
    lb_display_text_->adjustSize();
    last_py_ = "";
    last_py_text_="";
    showChinese(">>", getChineseListMap(cur_py_text_));
}

void VirtualKeyBoard::switchLetterStatus()
{
    foreach(int key, btn_list_map_.keys())
    {
        QList<QPushButton*> list(btn_list_map_[key]);
        int i(0), listsize(list.size());
        if(key==row_2)//除去删除按钮
        {
            listsize--;
        }else if(key==row_3)//除去确定按钮
        {
            listsize--;
        }else if(key==row_4)//除去两个shift及两个符号按钮
        {
            i++;
            listsize -= 3;
        }else if(key==row_5)
        {
            break;
        }

        for(; i<listsize; i++)
        {
            if(mode_flag_map_[is_capital_mode])//如果原先是大写
            {
                list.at(i)->setText(list.at(i)->text().toLower());
            }else
            {
                list.at(i)->setText(list.at(i)->text().toUpper());
            }
        }
    }
    mode_flag_map_[is_capital_mode] = !mode_flag_map_[is_capital_mode];
}

void VirtualKeyBoard::setSymbolPage(int enNum_chNum)
{
    int count(0);
    foreach(int key, btn_list_map_.keys())
    {
        QList<QPushButton*> list(btn_list_map_[key]);
        for(int i(0); i<list.size(); i++)
        {
            if(key==row_1)continue;
            list.at(i)->setText(symbol_list_map_[enNum_chNum].at(count++));//en0例如
        }
    }
}

void VirtualKeyBoard::ShowPinYinTip()
{

}
#include <QKeyEvent>
void VirtualKeyBoard::InsertTextToCurFocusWt(const QString &str)
{
    if (nullptr != cur_focus_wt_) {
        QKeyEvent keyPress(QEvent::KeyPress, 0, Qt::NoModifier, QString(str));
        QApplication::sendEvent(cur_focus_wt_, &keyPress);
    }
}

void VirtualKeyBoard::DeleteTextFromCurFocusWt()
{
    if (nullptr == cur_focus_wt_) {
        return;
    }

    QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier, QString());

    QApplication::sendEvent(cur_focus_wt_, &keyPress);
}

void VirtualKeyBoard::recKeyClicked(const QString &str)
{
    //    qDebug()<<__func__<<str<<str.contains(QRegExp("[a-z]"));
    if(str.size()==1)
    {
        if( mode_flag_map_[is_ch_mode] && (str.contains(QRegExp("([a-z])")) || (!cur_py_text_.isEmpty())) )//正常输入拼音
        {
            cur_py_text_+=str;
            //            ui->lb_display_text->setText(cur_py_text_);
            lb_display_text_->setText(cur_py_text_);
            lb_display_text_->adjustSize();
            cur_py_page_=-1;
            showChinese(">>", getChineseListMap(cur_py_text_));
        }else//直接输出
        {

            InsertTextToCurFocusWt(str);
            clearBuffer();
        }
    }else//接收的是命令
    {
        if(str=="&&")//&为QT快捷键,因此用 && 代替 &
        {
            return recKeyClicked("&");
        }
        else if(str=="sw")//中英文切换
        {
            mode_flag_map_[is_ch_mode] = !mode_flag_map_[is_ch_mode];
            if(mode_flag_map_[is_ch_mode])
            {
                symbal_page_=ch0;

            }
            else
            {
                symbal_page_=en0;
            }
            //            qDebug()<<__func__<<"chinese"<<mode_flag_map_[is_ch_mode];
            setSymbolPage(symbal_page_);
            clearBuffer();
        }else if ((str=="<<" || str==">>") && mode_flag_map_[is_ch_mode])//待选汉字切换
        {
            showChinese(str, getChineseListMap(cur_py_text_));
        }else if(str=="BS")//退格按钮
        {
            if(!cur_py_text_.isEmpty())//若处于中文输入状态
            {
                cur_py_text_.remove(cur_py_text_.size()-1, 1);
                //                ui->lb_display_text->setText(cur_py_text_);
                lb_display_text_->setText(cur_py_text_);
                lb_display_text_->adjustSize();
                showChinese(">>", getChineseListMap(cur_py_text_));
            }else
            {
                DeleteTextFromCurFocusWt();
            }

            if(cur_py_text_.isEmpty())
                clearBuffer();

        }else if(str == "English" || str == "Chinese")//空格按钮
        {
            if(!cur_py_text_.isEmpty())//若处于中文输入状态
            {
                cur_py_text_.append(" ");
                //                ui->lb_display_text->setText(cur_py_text_);
                lb_display_text_->setText(cur_py_text_);
                lb_display_text_->adjustSize();
                showChinese(">>", getChineseListMap(cur_py_text_));
            }else
            {
                InsertTextToCurFocusWt(" ");
                clearBuffer();
            }
        }else if(str=="Ls"||str=="Rs")//大小写切换
        {
            switchLetterStatus();
            if(mode_flag_map_[is_ch_mode])
            {
                clearBuffer();
            }
        }else if(str=="Le")
        {
            if( mode_flag_map_[is_ch_mode]&&(symbal_page_>ch1) )
            {
                symbal_page_--;
                setSymbolPage(symbal_page_);
            }else if ( (!mode_flag_map_[is_ch_mode])&&(symbal_page_>en1) )
            {
                symbal_page_--;
                setSymbolPage(symbal_page_);
            }
        }else if(str=="Ri")
        {

            if( mode_flag_map_[is_ch_mode] && (symbal_page_<ch2) )
            {
                symbal_page_++;
                setSymbolPage(symbal_page_);
            }else if ( (!mode_flag_map_[is_ch_mode])&&(symbal_page_<en2) )
            {
                symbal_page_++;
                setSymbolPage(symbal_page_);
            }
        }else if(str=="?123" || str=="ABC")
        {
            recKeyClicked("ok");//除去拼音输入时遗留下来的拼音
            if(str=="?123")
            {
                return recKeyClicked("Ri"); //切换到下一页
            }else if(str=="ABC")
            {
                mode_flag_map_[is_ch_mode] = !mode_flag_map_[is_ch_mode];
                return recKeyClicked("sw");
            }
        }else if(str == "ok")
        {
            if(!cur_py_text_.isEmpty())
            {
                InsertTextToCurFocusWt(cur_py_text_);
                clearBuffer();
            }
        }else if(str == "hi")
        {
            this->hide();
        }
    }
}


void VirtualKeyBoard::showChinese(const QString &key, const QMap<int, QStringList> &chinese_list_map)
{
    //    qDebug()<<__func__<<"chinese_list_map is empty:"<<chinese_list_map.isEmpty();

    QStringList chlist{};
    if(chinese_list_map.isEmpty())
    {
        //        ui->widget_showChinese->show();
    }
    else
    {
        //        ui->widget_showChinese->show();
        int map_size(chinese_list_map.size());
        //        qDebug()<<__func__<<map_size<<cur_py_page_<<chinese_list_map<<"\n\n";
        //        return;
        if(key == "<<" && cur_py_page_>0)
        {
            cur_py_page_--;
        }
        else if(key == ">>" && (cur_py_page_<map_size-1))
        {
            cur_py_page_++;
        }
        chlist=chinese_list_map[cur_py_page_];
    }

    QList<QPushButton*>btn_list(btn_list_map_[row_1]);
    for(int i(0); (i<btn_list.size()-2)||i<MAX_VISIBLE; i++)//除去左右切换两个按钮
    {
        btn_list.at(i+1)->setText("");
        if(i<chlist.size())
        {
            btn_list.at(i+1)->setText(chlist.at(i));
        }
    }
}

void VirtualKeyBoard::recKeyClickedSlot()
{
    const QPushButton* btn = (QPushButton*)sender();
    const QString &str = btn->text();
    return recKeyClicked(str);
}

void VirtualKeyBoard::selectChinese()
{
    const QPushButton* btn = (QPushButton*)sender();
    const QString &ch = btn->text();


    InsertTextToCurFocusWt(ch);
    clearBuffer();
}

void VirtualKeyBoard::focusChangedSlot(QWidget *old, QWidget *now)
{
    Q_UNUSED(old)

    if (nullptr != now && !this->isAncestorOf(now)) {

        if (now->inherits("QWidget")) {

            QStringList accept_wts;
            accept_wts<<"QLineEdit";//哪些类型控件响应键盘
            cur_focus_wt_ = nullptr;

            for(QString key : accept_wts) {
                if (now->inherits(key.toUtf8())) {

                    //该控件是否响应键盘,       //setProperty("haveInput", false);表示该控件不响应键盘
                    if (now->property("haveInput").isNull() || now->property("haveInput").toBool()) {
                        cur_focus_wt_ = now;
                        break;
                    }
                }
            }

            if (nullptr != cur_focus_wt_) {
                this->show();
            }

        }
    }

}


void VirtualKeyBoard::show()
{

    qDebug()<<geometry();
    QWidget::show();

    lb_display_text_->adjustSize();
    lb_display_text_->move(this->x(), this->y() - lb_display_text_->height());
    lb_display_text_->show();
    //    qDebug()<<__FILE__<<__func__;
}

void VirtualKeyBoard::hide()
{
    emit setDefaultColorSignal();
    lb_display_text_->hide();
    QWidget::hide();
}






















