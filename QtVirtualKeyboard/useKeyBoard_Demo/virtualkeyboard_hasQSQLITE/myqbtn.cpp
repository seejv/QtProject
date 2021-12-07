#include "myqbtn.h"
#include <QMouseEvent>
#include "myqproxystyle.h"
//#include <QDebug>
#include <QTimer>//控制连续点击按钮的效果


MyqBtn::MyqBtn(QWidget *parent) : QPushButton(parent)
{
    timer_ = new QTimer(this);
    connect(timer_, SIGNAL(timeout()), this, SLOT(updatePressStatus()));

    is_press_on_timer_ = new QTimer(this);
    connect(is_press_on_timer_, SIGNAL(timeout()), this, SLOT(pressOnSlot()));

    this->setStyle(new MyQProxyStyle);
    this->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
}

void MyqBtn::setText(const QString &text)
{
    QPushButton::setText(text);

    this->style()->unpolish(this);
    this->style()->polish(this);
    this->update();
}

void MyqBtn::mousePressEvent(QMouseEvent *event)
{
    is_press_on_timer_->start(100);
    return QPushButton::mousePressEvent(event);
}


void MyqBtn::mouseReleaseEvent(QMouseEvent *event)
{
    is_press_on_=false;
    if(is_press_on_timer_->isActive())
        is_press_on_timer_->stop();

    pos2_ = event->pos();
    if(this->rect().contains(pos2_))
    {
        if(is_ok_)
        {
            is_ok_ = false;
            timer_->start(100);//限制连续点击速度
            emit clicked(this->text());
        }
    }
QPushButton::mouseReleaseEvent(event);
}


void MyqBtn::updatePressStatus()
{
    is_ok_ = true;
    timer_->stop();
}

void MyqBtn::pressOnSlot()
{
    if(is_press_on_)emit clicked(this->text());//模拟连续点击
    is_press_on_=true;
}


