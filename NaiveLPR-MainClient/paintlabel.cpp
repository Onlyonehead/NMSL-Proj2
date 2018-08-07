#include "paintlabel.h"

#include <QDebug>

PaintLabel::PaintLabel(QWidget *parent):QLabel(parent)
{
    mousePress_l = false;
    mousePress_r = false;
    mouseRelease_r = false;
}

void PaintLabel::painterClear(){
    pStart = QPoint(0, 0);
    pEnd = QPoint(0, 0);
    update();
}

void PaintLabel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    if(mousePress_l){
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        // 设置画笔颜色、宽度
        painter.setPen(QPen(QColor(50, 150, 200), 2));
        // 设置画刷颜色
        painter.setBrush(QColor(255, 255, 255, 140));

        painter.eraseRect(0, 0, this->width(), this->height());
        painter.drawRect(pStart.x(), pStart.y(), pEnd.x()-pStart.x(), pEnd.y()-pStart.y());
    }

    if(mousePress_r){
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        // 设置画笔颜色、宽度
        painter.setPen(QPen(QColor(255, 0, 0), 2));

        painter.eraseRect(0, 0, this->width(), this->height());
        painter.drawLine(pStart, pEnd);

        QPainter painter2(this);
        painter2.setRenderHint(QPainter::Antialiasing, true);
        // 设置画笔颜色、宽度
        painter2.setPen(QPen(QColor(255, 0, 0, 40), 2));
        painter2.drawLine(pStart, pEnd_index);
    }

    if(mouseRelease_r){
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        // 设置画笔颜色、宽度
        painter.setPen(QPen(QColor(255, 0, 0), 2));

        painter.eraseRect(0, 0, this->width(), this->height());
        painter.drawLine(pStart, pEnd);

        mouseRelease_r = false;
    }
}

void PaintLabel::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton)
        mousePress_l = true;
    if(event->button() == Qt::RightButton)
        mousePress_r = true;
    pStart = event->pos();
    emit setPointStart(pStart);
}

void PaintLabel::mouseMoveEvent(QMouseEvent *event){
    if(mousePress_l){
        pEnd = event->pos();
        int x=pEnd.x(), y=pEnd.y();
        if(x<0) pEnd.setX(0);
        if(x>this->width()) pEnd.setX(this->width());
        if(y<0) pEnd.setY(0);
        if(y>this->height()) pEnd.setY(this->height());
    }

    if(mousePress_r){
        pEnd_index = event->pos();
        if(pEnd_index.x()<0) pEnd_index.setX(0);
        if(pEnd_index.x()>this->width()) pEnd_index.setX(this->width());
        if(pEnd_index.y()<0) pEnd_index.setY(0);
        if(pEnd_index.y()>this->height()) pEnd_index.setY(this->height());

        int _x, _y;
        _x = qAbs(pEnd_index.x()-pStart.x());
        _y = qAbs(pEnd_index.y()-pStart.y());
        if(_x>=_y){
            pEnd.setX(pEnd_index.x());
            pEnd.setY(pStart.y());
        } else{
            pEnd.setX(pStart.x());
            pEnd.setY(pEnd_index.y());
        }
    }

    emit setPointEnd(pEnd);
    update();
}

void PaintLabel::mouseReleaseEvent(QMouseEvent *event){
    Q_UNUSED(event);
    if(mousePress_r){
        mouseRelease_r = true;
        update();
    }
    mousePress_r = false;
    mousePress_l = false;
}
