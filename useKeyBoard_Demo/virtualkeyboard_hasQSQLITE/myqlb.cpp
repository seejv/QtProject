#include "myqlb.h"
#include <QStyle>
MyqLb::MyqLb(QWidget *parent) : QLabel(parent)
{

}

void MyqLb::setText(const QString &text)
{
    QLabel::setText(text);

    this->style()->unpolish(this);
    this->style()->polish(this);
    this->update();
}

void MyqLb::clear()
{
    QLabel::setText("");
    this->style()->unpolish(this);
    this->style()->polish(this);
    this->update();
}
