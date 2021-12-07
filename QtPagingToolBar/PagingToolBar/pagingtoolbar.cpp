#include "pagingtoolbar.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QtDebug>
#include <QStyle>
#include <QSpacerItem>
#include <QLabel>

PagingToolBar::PagingToolBar(QWidget *parent)
    : QWidget(parent)
{
    this->resize(600, 50);
    this->setObjectName("PagingToolBarwt");

    //    QStringList qss{};
    //    qss.append(R"(
    //               QWidget#PagingToolBarwt QPushButton{
    //               border:1px solid #999999;
    //               border-radius: 4px;
    //               outline:none;
    //               background-color:white;
    //               font: 16pt;
    //               color:#999999;
    //               }
    //               )");


    //    qss.append("QWidget#PagingToolBarwt QPushButton[selected=true]{background-color:green;color:white;}");//btn_1, ..., btn_max

    //    //    btn_prev
    //    qss.append(R"(
    //               QWidget#PagingToolBarwt QPushButton#btn_prev{
    //               image-position:center left;
    //               padding-right:8px;
    //               text-align:center;
    //               }
    //               )");
    //    qss.append(R"(
    //               QWidget#PagingToolBarwt QPushButton#btn_prev:pressed{
    //               background-color:green;
    //               color:white;
    //               }
    //               )");

    //    //btn_next
    //    qss.append(R"(
    //               QWidget#PagingToolBarwt QPushButton#btn_next{
    //               image-position:center right;
    //               padding-left:8px;
    //               text-align:center;
    //               }
    //               )");

    //    qss.append(R"(
    //               QWidget#PagingToolBarwt QPushButton#btn_next:pressed{
    //               background-color:green;
    //               color:white;
    //               }
    //               )");



    //    qss.append("QWidget#btn_wt QPushButton#btn_prev{background-color:red;}");//btn_prev
    //    qss.append("QWidget#btn_wt QPushButton#btn_prev:pressed{background-color:black;color:red;}");//btn_prev
    //    qss.append("QWidget#btn_wt QPushButton:checked{background-color:green;}");//btn_1, ..., btn_max
    //    qss.append("QWidget#btn_wt QPushButton#btn_1:checked{background-color:green;}"); //btn_1,btn_2 ... btn_num
    //    qss.append("QWidget#btn_wt QPushButton#btn_next{background-color:blue;}");//btn_next
    //    qss.append("QWidget#btn_wt QPushButton#btn_next:pressed{background-color:black;color:blue;}");//btn_next


    //    this->setStyleSheet(qss.join(""));
    //    this->style()->unpolish(this);
    //    this->style()->polish(this);
    //    this->update();
}

void PagingToolBar::Init(const int index_count, const bool auto_hide_btn_prev_next, const Qt::AlignmentFlag align)
{

    auto layout1 = this->findChild<QHBoxLayout*>();
    if (nullptr != layout1) {
        qDebug()<<__func__<<"Inited";
        return;
    }
    max_btn_index_ = index_count;

    auto_hide_btn_prev_next_ = auto_hide_btn_prev_next;
    btn_index_aligin_ = align;

    layout1 = new QHBoxLayout;
    auto layout2 = new QHBoxLayout;
    btn_wt_ = new QWidget(this);

    layout1->setMargin(0);
    layout1->setSpacing(0);
    layout1->addWidget(btn_wt_);

    this->setLayout(layout1);

    layout2->setMargin(0);
    layout2->setSpacing(10);

    btn_wt_->setObjectName("btn_wt");  // 1 2 3 4 5 6 ...
    btn_wt_->setLayout(layout2);

    int btn_total = max_btn_index_ + 2;

    for (int i = 0; i < btn_total; ++i) {
        QPushButton *btn = new QPushButton;
        QWidget *add_wt = btn;


        btn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        btn->setText(QString::number(i));

        btn->setObjectName("btn_" + QString::number(i));
        if (0 == i) {
            btn->setObjectName("btn_prev");
        }else if (btn_total - 1  == i) {
            btn->setObjectName("btn_next");
        }

        if (i > 0 && i < max_btn_index_ + 1) {
            btn->setProperty("btn_index", i);

        }else if (i == 0) {
            btn->setProperty("btn_index", i); //代表上一页, 外嵌套 一个 QWidget， hide 的时候不会影响布局

            auto layout_tmp = new QHBoxLayout;
            auto wt = new QWidget;

            layout_tmp->setMargin(0);
            layout_tmp->addWidget(btn);
            wt->setLayout(layout_tmp);
            add_wt = wt;


        }else if (i == max_btn_index_ + 1) {
            btn->setProperty("btn_index", i); //代表下一页

            auto layout_tmp = new QHBoxLayout;
            auto wt = new QWidget;


            layout_tmp->setMargin(0);
            layout_tmp->addWidget(btn);
            wt->setLayout(layout_tmp);
            add_wt = wt;
        }

        layout2->addWidget(add_wt);
        btns_.append(btn);
        connect(btn, &QPushButton::clicked, this, &PagingToolBar::SlotBtnClicked);

        if (0 == i || max_btn_index_ + 1 == i) {
            layout2->setStretch(i, btn_prev_next_strech_);
        }else {
            layout2->setStretch(i, 10);
        }

    }

    max_page_lb_ = new QLabel;
    max_page_lb_->setObjectName("max_page_lb");
    max_page_lb_->setAlignment(Qt::AlignCenter);
    max_page_lb_->setText(QString("共 %1 页").arg(maxpage_));
    layout2->addWidget(max_page_lb_);
    layout2->setStretch(layout2->count() - 1, 10);

    btns_.first()->setText("上一页"); //上一页
    btns_.last()->setText("下一页");//下一页


    layout1->insertSpacerItem(0, new QSpacerItem(QSizePolicy::Expanding, QSizePolicy::Expanding));
    layout1->addItem(new QSpacerItem(QSizePolicy::Expanding, QSizePolicy::Expanding));

    layout1->setStretch(0, 0);
    layout1->setStretch(1, btn_prev_next_strech_ + btn_prev_next_strech_ + 10 + max_btn_index_ * 10); //btn_prev, btn_next, max_page_lb
    layout1->setStretch(2, 0);

    SetCurPage(1);
}

void PagingToolBar::SetCurPage(const int cur_page)
{
    if (cur_page < 1 || cur_page > maxpage_) {
        return;
    }

    cur_page_ = cur_page;
    const int max_btn_index = btns_.count() - 2;
    const int cur_btn_index = ((cur_page - btns_.at(1)->text().toInt() + 1) % max_btn_index == 0) ?
                (max_btn_index) :
                ((cur_page - btns_.at(1)->text().toInt() + 1) % max_btn_index); //当前被按下的按钮
    int after_btn_index = cur_btn_index;//接下来应该被选中的按钮
    int last_btn_index = 1; //当前处于被选中的按钮
    const int middle_btn_index = (max_btn_index % 2 > 0) ? (max_btn_index / 2 + 1) : (max_btn_index / 2); //偶数时候，选择左侧的为中点


    for (int i = 1; i < btns_.count() - 1; ++i) {
        if (btns_.at(i)->property("selected").toBool()) {
            last_btn_index = i;
            break;
        }
    }

    if (cur_btn_index > middle_btn_index) { // 靠近 “下一页”
        int cur_right_maxpage = btns_.at(max_btn_index)->text().toInt();
        const int step = cur_btn_index - middle_btn_index;

        for (int i = step; i > 0 && cur_right_maxpage < maxpage_; --i) {  //获取最右边可以取到的最大值
            ++cur_right_maxpage;
            --after_btn_index;
        }

        for (int i = max_btn_index, j = 0; i > 0 ; --i, ++j) {
            btns_.at(i)->setText(QString::number(cur_right_maxpage - j));
        }

    }else if (cur_btn_index < middle_btn_index) { // 靠近 “上一页”
        int cur_left_minpage = btns_.at(1)->text().toInt();
        const int step = middle_btn_index - cur_btn_index;

        for (int i = step; i > 0 && cur_left_minpage > 1; --i) {  //获取最左边可以取到的最小值
            --cur_left_minpage;
            ++after_btn_index;
        }

        for (int i = 1, j = 0; i < btns_.count() - 1; ++i, ++j) {
            btns_.at(i)->setText(QString::number(cur_left_minpage + j));
        }
    }


    //样式的选中与取消
    auto btn_last = btns_.at(last_btn_index);
    auto btn_after = btns_.at(after_btn_index);

    if (after_btn_index <= 1) {

        if (maxpage_ == 1 || auto_hide_btn_prev_next_) {
            btns_.at(0)->setVisible(false);
            btns_.at(1)->setVisible(false);
        }

    }else {
        btns_.at(0)->setVisible(true);
        btns_.at(1)->setVisible(true);
    }


    if ((max_btn_index == after_btn_index || maxpage_ == after_btn_index)) {

        if (maxpage_ == 1 || auto_hide_btn_prev_next_) {
            btns_.at(btns_.count() - 1)->setVisible(false);
        }


    }else if (maxpage_ > 1 || auto_hide_btn_prev_next_) {
        btns_.at(btns_.count() - 1)->setVisible(true);
    }


    if (btn_last != btn_after) {
        btn_last->setProperty("selected", false);
        btn_last->style()->unpolish(btn_last);
        btn_last->style()->polish(btn_last);
        btn_last->update();
    }

    btn_after->setProperty("selected", true);
    btn_after->style()->unpolish(btn_after);
    btn_after->style()->polish(btn_after);
    btn_after->update();


    //布局对齐方式
    if (maxpage_ < max_btn_index) {
        for (int i = maxpage_ + 1; i <= max_btn_index; ++i) {
            btns_.at(i)->setVisible(false);
        }

        int btn_index_visible = maxpage_;

        auto layout1 = this->findChild<QHBoxLayout*>();
        int free_spacing = ((max_btn_index - btn_index_visible) * 10);
        int middle = btn_index_visible * 10 + btn_prev_next_strech_ + btn_prev_next_strech_ + 10; //btn_prev, btn_next, max_page_lb

        QWidget *next_wt = static_cast<QWidget *>(btns_.last()->parent());
        QWidget *prev_wt = static_cast<QWidget *>(btns_.first()->parent());



        switch (btn_index_aligin_) {
        case (Qt::AlignLeft): {

            if (maxpage_ == 1) {
                prev_wt->setVisible(false);
                next_wt->setVisible(false);
                free_spacing = free_spacing + 10 + 10;
                middle = middle - 10 - 10;
            }else if (maxpage_ > 1){
                if (auto_hide_btn_prev_next_) {
                    if (after_btn_index > 1 & after_btn_index < maxpage_) {
                        prev_wt->setVisible(true);
                    }
                    if (after_btn_index < maxpage_) {
                        next_wt->setVisible(true);
                    }
                }else {
                    prev_wt->setVisible(true);
                    next_wt->setVisible(true);
                }
            }

            layout1->setStretch(0, 0);
            layout1->setStretch(1, middle);
            layout1->setStretch(2, free_spacing);
            break;
        }
        case (Qt::AlignCenter): {

            layout1->setStretch(0, free_spacing / 2);
            layout1->setStretch(1, middle);
            layout1->setStretch(2, free_spacing / 2);

            break;
        }
        case (Qt::AlignRight): {

            if (maxpage_ == 1) {
                next_wt->setVisible(false);
                free_spacing = free_spacing + 10;
                middle = middle - 10;
            }else if (maxpage_ > 1){
                if (auto_hide_btn_prev_next_) {
                    if (after_btn_index < maxpage_) {
                        next_wt->setVisible(true);
                    }
                }else {
                    next_wt->setVisible(true);
                }
            }

            layout1->setStretch(0, free_spacing);
            layout1->setStretch(1, middle);
            layout1->setStretch(2, 0);
            break;
        }
        default: {

        }

        }

    }else {
        for (int i = 1; i <= max_btn_index; ++i) {
            if (!btns_.at(i)->isVisible())
                btns_.at(i)->setVisible(true);
        }
    }

}



void PagingToolBar::SetMaxPage(const int maxpage)
{
    maxpage_ = maxpage;
    max_page_lb_->setText(QString("共 %1 页").arg(maxpage_));

    if (0 == maxpage) {
        btn_wt_->setVisible(false);
    }else if (maxpage > 0) {
        btn_wt_->setVisible(true);
        if (maxpage > 1) {
            if (1 != cur_btn_index_) {
                btns_.first()->setVisible(true);
            }

            if (max_btn_index_ != cur_btn_index_) {
            btns_.last()->setVisible(true);
            }
        }

        for (int i = 1; i < btns_.count() - 2 && i < max_btn_index_; ++i) {
            btns_.at(i)->setVisible(true);
        }
    }

}

int PagingToolBar::GetMaxPage() const
{
    return maxpage_;
}

int PagingToolBar::GetCurPage() const
{
    return cur_page_;
}


PagingToolBar::~PagingToolBar()
{

}



void PagingToolBar::SlotBtnClicked()
{

    QPushButton *btn = static_cast<QPushButton*>(sender());

    int btn_index = btn->property("btn_index").toInt();
    int  cur_page = cur_page_;


    if (btn_index > 0 && btn_index < btns_.count() - 1) {
        cur_page = btn->text().toInt();
    }else if (btn_index == 0) {//上一页

        if (cur_page > 1) {
            --cur_page;
        }else {
            return;
        }

    }else if (btn_index == btns_.count() - 1) {//下一页

        if (cur_page < maxpage_) {
            ++cur_page;
        }else {
            return;
        }
    }

    SetCurPage(cur_page);
    emit SignalCurPageChanged(cur_page);
}


















