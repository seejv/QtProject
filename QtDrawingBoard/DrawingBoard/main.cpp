#include "drawingboard.h"

#include <QApplication>

#include <QStackedWidget>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QStackedWidget stack;


    stack.resize(500,500);

    DrawingBoardNameSpace::DrawingBoard b;
    b.resize(400,400);
    QWidget c;
    c.resize(300,300);
    c.setStyleSheet("QWidget{background:red;}");

    stack.addWidget(&b);
    stack.addWidget(&c);

    stack.show();


//    int i = 0;

//    QTimer t;
//    QObject::connect(&t, &QTimer::timeout, [&](){
//        b.onRestoreChanged();
//    });

//    t.start(2000);

    return a.exec();
}
