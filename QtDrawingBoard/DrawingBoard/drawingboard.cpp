#include "drawingboard.h"
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPixmap>
#include <QPainter>
#include <QPoint>
#include <QLabel>

using namespace DrawingBoardNameSpace;
//QLabel *lb;

DrawingBoard::DrawingBoard(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowFlags(Qt::Sheet |  Qt::BypassWindowManagerHint);// 禁用所有插件
    this->setAttribute(Qt::WA_OpaquePaintEvent); //每次直接绘制指定范围内的全部像素，对不支持双缓冲的设备进行了优化

    drawPen_ = new QPen();
    drawBrush_ = new QBrush();

    drawPen_->setWidth(2);
    drawPen_->setBrush(Qt::SolidPattern);
    drawPen_->setColor(Qt::green);

    drawBrush_->setStyle(Qt::NoBrush); //Qpainter 填充使用的

    pixCache_ = new QPixmap;
    pix_ = new QPixmap;
    pixSrc_ = new QPixmap;

    setPix(":/screen.png");
}


DrawingBoard::~DrawingBoard()
{

}

void DrawingBoard::setPix(const QString &path, const QByteArray &orPixData)
{
    if (path.isEmpty() && orPixData.isEmpty()) return;

    if (path.isEmpty()) pixSrc_->loadFromData(orPixData);
    else pixSrc_->load(path);

    *pix_ = *pixSrc_;
    *pixCache_ = *pix_;
    pixCache_->fill(Qt::transparent);
}

void DrawingBoard::setEraseMode(bool isErase)
{
    isErase_ = isErase;
}

void DrawingBoard::setPen(const QPen &pen)
{
    *drawPen_ = pen;
}

void DrawingBoard::setPixOffsetPoint(const QPoint &point)
{
    offsetPoint_ = point;
}

void DrawingBoard::paintEvent(QPaintEvent *e)
{
    if (pixSrc_->isNull()) return;

    (void) e;

    QPainter painter(this);

    //draw in the cache drawBoard
    drawInPix(points_, pixCache_);

//    if (isDrawBack_ || this->rect() == e->rect()) {   // 发现在 arm 设备没有添加这个会 透明化
//        isDrawBack_ = false;
        painter.drawPixmap(e->rect(), *pix_, e->rect());
//    }


        // 如果不需要擦除操作，可以直接将 *pixCache_ = *pix_； 然后 直接在 pixCache 上面画图, 可以减少一次画背景图的操作


    QRect pixRect(e->rect());

    pixRect.setTopLeft(offsetPoint_ + pixRect.topLeft());
    pixRect.setBottomRight(offsetPoint_ + pixRect.bottomRight());

    painter.drawPixmap(e->rect(), *pixCache_, pixRect);

    if (isRelease_) {
        points_.clear();
    }

}

void DrawingBoard::mouseMoveEvent(QMouseEvent *e)
{
    if (isPress_) {
        //        isMove_ = true;
        points_.append(e->pos());
        this->update(updateUpdateArea(e->pos()));
    }

}

void DrawingBoard::mousePressEvent(QMouseEvent *e)
{
    isRelease_ = false;
    isPress_ = true;

    topLeftPoint_ = e->pos();
    bottomRightPoint_ = e->pos();

    points_.clear();
    points_.append(e->pos());
}

void DrawingBoard::mouseReleaseEvent(QMouseEvent *e)
{
    isRelease_ = true;
    isPress_ = false;
    if (this->rect().contains(e->pos())) {
        this->update(updateUpdateArea(e->pos()));
    }

}

void DrawingBoard::drawInPix(const QVector<QPoint> points, QPixmap *pixBoard)
{
    QPainter painter(pixBoard);
    painter.setPen(*drawPen_);
    painter.setBrush(*drawBrush_);

    if (isErase_) {
        painter.setCompositionMode(QPainter::CompositionMode_Clear);
    }

    if (1 == points.count()) painter.drawPoint(points.first());

    auto offsetX = offsetPoint_.x();
    auto offsetY = offsetPoint_.y();

    for (int i = 1; i < points.count(); ++i) {
        auto p1 = points.at(i - 1);
        auto p2 = points.at(i);
        p1.setX(p1.x() + offsetX);
        p1.setY(p1.y() + offsetY);

        p2.setX(p2.x() + offsetX);
        p2.setY(p2.y() + offsetY);

        painter.drawLine(p1, p2);
    }

}

const QRect DrawingBoard::updateUpdateArea(const QPoint &curPoint)
{
    int curX = curPoint.x();
    int curY = curPoint.y();

    QPoint p1 = topLeftPoint_;
    QPoint p2 = bottomRightPoint_;


    int minX = qMin(qMin(curX, p1.x()), p2.x());
    int minY = qMin(qMin(curY, p1.y()), p2.y());

    int maxX = qMax(qMax(curX, p1.x()), p2.x());
    int maxY = qMax(qMax(curY, p1.y()), p2.y());

    if (minX < 0) minX = 0;
    if (minY < 0) minY = 0;
    if (maxX > this->width()) maxX = this->width();
    if (maxY > this->height()) maxY = this->height();


    topLeftPoint_ = {minX, minY};
    bottomRightPoint_ = {maxX, maxY};

    return QRect(topLeftPoint_, bottomRightPoint_);

}

void DrawingBoard::onRestoreChanged()
{
    pixCache_->fill(Qt::transparent);
    *pix_ = *pixSrc_;
    this->update();
}

void DrawingBoard::onSaveChanged()
{
    QPainter p(pix_);
    if (!p.isActive()) p.begin(pix_);
    p.drawPixmap(0, 0, *pixCache_);
    *pixSrc_ = *pix_;
}

void DrawingBoard::onSavePix(QString path)
{
    pixSrc_->save(path);
}

