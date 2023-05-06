#include "widget.h"

#include <QApplication>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QTimer>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QIcon icon(":/countdown.ico");
    Widget w;

    QMenu* menu = new QMenu(&w);
    QAction* openAction = new QAction("打开", menu);
    QAction* exitAction = new QAction("退出", menu);
    QSystemTrayIcon* trayIcon = new QSystemTrayIcon(&w);


    menu->addAction(openAction);
    menu->addAction(exitAction);

    trayIcon->setContextMenu(menu);
    trayIcon->setIcon(icon);
    trayIcon->show();

    a.setWindowIcon(icon);

    w.show();


    QObject::connect(exitAction, &QAction::triggered, [&a]() {
        a.quit();
    });

    QObject::connect(openAction, &QAction::triggered, [&w]() {
        w.show();
    });

    // 托盘图标双击事件
   QObject::connect(trayIcon, &QSystemTrayIcon::activated, [&w](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::DoubleClick) {
            w.show(); // 双击托盘，显示窗口
        }
    });

    return a.exec();
}
