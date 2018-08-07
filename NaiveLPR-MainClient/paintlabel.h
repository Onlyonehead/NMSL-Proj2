#ifndef PAINTLABEL_H
#define PAINTLABEL_H

#include <QLabel>
#include <QPoint>
#include <QPainter>
#include <QMouseEvent>

class PaintLabel:public QLabel
{
    Q_OBJECT
public:
    explicit PaintLabel(QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent (QMouseEvent *);
    void mouseMoveEvent (QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
public slots:
    void painterClear();
signals:
    void setPointStart(QPoint);
    void setPointEnd(QPoint);
private:
    QPoint pStart, pEnd, pEnd_index;
    bool mousePress_l, mousePress_r, mouseRelease_r;
};

#endif // PAINTLABEL_H
