#include "widget.h"
#include "ui_widget.h"

#include <QMessageBox>
#include <QTimer>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    timer_ = new QTimer;

    connect(timer_,&QTimer::timeout, [&](){
        if (--count_ <= 0) {
            QString taskname = ui->taskname->text();
            QMessageBox msgBox;
            msgBox.setText(QString("定时任务 \n %1 \n 已经结束").arg(taskname));
            msgBox.exec();
            on_stop_clicked();
            return;
        }

        int hh = count_ / 3600;
        int mm = count_ % 3600 / 60;
        int ss = count_ % 60;

        ui->countdown->setText(QString::asprintf("%02d:%02d:%02d", hh, mm, ss));
    });

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_start_clicked()
{
    if (timer_->isActive()) return;
    int hh{},mm{},ss{};

    if (!ui->hhcount->text().isEmpty()) hh = ui->hhcount->text().toInt();
    if (!ui->mmcount->text().isEmpty()) mm = ui->mmcount->text().toInt();
    if (!ui->sscount->text().isEmpty()) ss = ui->sscount->text().toInt();
    ui->countdown->setText(QString::asprintf("%02d:%02d:%02d", hh, mm, ss));

    count_ = hh * 3600 + mm * 60 + ss;

    timer_->start(1000);
}


void Widget::on_stop_clicked()
{
    timer_->stop();
    count_ = 0;
    int hh{}, mm{}, ss{};
    ui->countdown->setText(QString::asprintf("%02d:%02d:%02d", hh, mm, ss));
}

