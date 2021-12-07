#include "pagingtoolbar.h"


#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PagingToolBar w;
    w.Init(5, true, Qt::AlignCenter);
    w.SetMaxPage(15);
    w.SetCurPage(1);
    w.show();





    return a.exec();
}
