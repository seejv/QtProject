
#include <QApplication>
#include "virtualkeyboard_hasQSQLITE/virtualkeyboard.h"
#include <QLineEdit>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto keyboard = VirtualKeyBoard::getKeyboard();
    keyboard->setGeometry(300, 200, 432, 185);
    keyboard->init("virtualkeyboard_hasQSQLITE/Resource/ChinesePY.db");

    auto le = new QLineEdit(nullptr);
    le->setGeometry(100, 0, 100,50);
    le->show();
    le = new QLineEdit(nullptr);
    le->setGeometry(100, 200, 100,50);

    keyboard->show();
    le->show();


    return a.exec();
}
