#include "slipwt.h"
#include <QPropertyAnimation>
#include <QPushButton>
#include <QResizeEvent>
#include <QPropertyAnimation>
#include <QPainter>
#include <QStyle>

Slipwt::Slipwt(QWidget *parent) : QWidget(parent)
{

    slipbtn_ = new QPushButton(this);

    ani_ = new QPropertyAnimation(slipbtn_, "pos", this);
    ani_->setEasingCurve(QEasingCurve::Linear);
    ani_->setDuration(100);

    connect(slipbtn_, &QPushButton::clicked, this, &Slipwt::SlotBtnClicked);

    SetChecked(false);
    SetColors();
    UpdateStyleSheet();
}


void Slipwt::SetChecked(const bool ischecked)
{
    ischecked_ = ischecked;
    if (ischecked_) {
        slipbtn_->move(this->width() - slipbtn_->width(), 0);
    }else {
        slipbtn_->move(0, 0);
    }
}

void Slipwt::SetColors(const QString &background_normal, const QString &background_checked, const QString &btn)
{
    background_normal_color_ = background_normal;
    background_checked_color_ = background_checked;
    btn_color_ = btn;
    this->update();
}



void Slipwt::resizeEvent(QResizeEvent *e)
{
    this->resize(e->size());
    slipbtn_->resize(this->height(), this->height());

    UpdateStyleSheet();
    return QWidget::resizeEvent(e);
}

void Slipwt::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::transparent);

    QString color{};
    if (ischecked_) {
        color = background_checked_color_;
    }else {
        color = background_normal_color_;
    }

    painter.setBrush(QBrush(QColor(color)));
    painter.drawRoundedRect(this->rect(), this->height() / 2, this->height() / 2);

}


void Slipwt::UpdateStyleSheet()
{
    QStringList qss{};

    //    qss.append(QString(R"(QWidget{border: 0px solid black; border-radius: %1px; background: %2;})").arg(this->height()).arg("#ff0000"));//invalid
    qss.append(QString(R"(QPushButton{border: none; outline: none; border-radius: %1px; background-color: %2;})").arg(this->height() / 2).arg(btn_color_));
    SetChecked(ischecked_);

    this->setStyleSheet(qss.join(""));
    this->style()->unpolish(this);
    this->style()->polish(this);
    this->update();
}


void Slipwt::SlotBtnClicked()
{

    if (QPropertyAnimation::Running == ani_->state()) {
        return;
    }
    ischecked_ = !ischecked_;

    if (ischecked_) {
        ani_->setStartValue(slipbtn_->pos());
        ani_->setEndValue(QPoint(this->width() - slipbtn_->width(), 0));
    }else {
        ani_->setStartValue(slipbtn_->pos());
        ani_->setEndValue(QPoint(0, 0));
    }

    ani_->start();
    this->update();

    SignalCheckedChanged(ischecked_);
}
