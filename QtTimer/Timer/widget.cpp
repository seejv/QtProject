#include "widget.h"
#include "ui_widget.h"

#include <QCloseEvent>
#include <QMessageBox>
#include <QTimer>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("倒计时");

    this->setWindowFlags(Qt::WindowStaysOnTopHint);


    timer_ = new QTimer(this);
    dialog_ = new QDialog(this);

    auto btn = new QPushButton(dialog_);
    btn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    QFont font = btn->font();
    font.setPixelSize(50);
    btn->setFont(font);

    QHBoxLayout *lay = new QHBoxLayout;
    lay->addWidget(btn);

    dialog_->setLayout(lay);


    connect(btn, &QPushButton::clicked, dialog_, &QDialog::hide);


    connect(timer_,&QTimer::timeout, [=](){
        if (--count_ <= 0) {
            QString taskname = ui->taskname->text();
            btn->setText(taskname);

            dialog_->showFullScreen();
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

void Widget::closeEvent(QCloseEvent *event)
{
    event->ignore();
    this->hide();
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

