#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE


class QTimer;

class Widget : public QWidget
{
    Q_OBJECT


    QTimer *timer_{};
    int count_{};
    QDialog *dialog_{};

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:

    void on_start_clicked();
    void on_stop_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
