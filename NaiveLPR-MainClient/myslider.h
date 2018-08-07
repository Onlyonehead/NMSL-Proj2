#ifndef MYSLIDER_H
#define MYSLIDER_H

#include <QWidget>
#include <QSlider>
#include <QMouseEvent>

class MySlider : public QSlider
{
    Q_OBJECT
public:
    explicit MySlider(QWidget *parent = 0);


protected:
    void mousePressEvent(QMouseEvent *ev);//重写QSlider的mousePressEvent事件
signals:
    void mySliderClicked();//自定义的鼠标单击信号，用于捕获并处理
};

#endif // MYSLIDER_H
