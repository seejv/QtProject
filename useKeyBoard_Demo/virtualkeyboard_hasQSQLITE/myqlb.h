#ifndef MYQLB_H
#define MYQLB_H

#include <QLabel>
class MyqLb : public QLabel
{
    Q_OBJECT
public:
    explicit MyqLb(QWidget *parent = nullptr);
    void setText(const QString &text);
signals:

public slots:

    void clear();

};

#endif // MYQLB_H
