#ifndef DRAWINGBOARD_H
#define DRAWINGBOARD_H
#include <QtDebug>
#include <QWidget>


/*
   功能： 可以擦除，可保存，可恢复画板
   说明： 该画板在 qt5.9.8 下测试通过，目前在 ARM Cortex A7 双核 @1.3GHz 画图尚且流畅
    有些是没有用到，例如

    offsetPoint_

*/




namespace DrawingBoardNameSpace {

#define LOGD qDebug()<<__func__

class DrawingBoard : public QWidget
{
    Q_OBJECT

public:
    DrawingBoard(QWidget* parent = nullptr);
    ~DrawingBoard();


    void setPix(const QString &path, const QByteArray &orPixData = {});
    void setEraseMode(bool isErase);
    void setPen(const QPen &pen);

    //TODO
    void setPixOffsetPoint(const QPoint &point);

protected:

    void paintEvent(QPaintEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
//    void resizeEvent(QResizeEvent *e) override;

private:
//background picture,  confirm tmp changed,  tmp changed.
    // 背景图片， 提交临时变更， 临时绘图区域
    QPixmap *pixSrc_{},  *pix_{}, *pixCache_{};
    QPoint offsetPoint_{};
    QPoint topLeftPoint_{}, bottomRightPoint_{};//mark udpate area, save the cpu

    bool isPress_ = false, isMove_ = false, isRelease_ = false;
    QVector<QPoint> points_;

    QPen *drawPen_;
    QBrush *drawBrush_;
    bool isErase_ = false;
    bool isDrawBack_ = true;


// 当画板大小与可见区域不一致时候，需要进行位置重定位
    void drawInPix(const QVector<QPoint> points, QPixmap *pixBoard); //在 paintervent 调用，就不 begin 了

private:
    const QRect updateUpdateArea(const QPoint &curPoint);

public slots:

    void onRestoreChanged();
    void onSaveChanged();
    void onSavePix(QString path);

};
}


#endif // DRAWINGBOARD_H
