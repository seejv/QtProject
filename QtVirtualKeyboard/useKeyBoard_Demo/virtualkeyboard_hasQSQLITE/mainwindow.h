#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class QSqlDatabase;

namespace Ui {
class Widget;
}

class VirtualKeyBoard : public QWidget
{
    Q_OBJECT

public:
    explicit VirtualKeyBoard(QWidget *parent = nullptr);
    QStringList getChineseList();
    ~VirtualKeyBoard();

    void showChinese(const QString &key);

private slots:
    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::Widget *ui;
    QSqlDatabase* db_;
    QStringList chinese_list_;
    QString last_py_;
    QString last_py_text_;
    bool is_last_ok_;
};

#endif // MAINWINDOW_H
