#include "swipescrollarea.h"
#include <QScrollArea>
#include <QScrollBar>
#include <QMouseEvent>
#include <QDebug>
SwipeScrollArea::SwipeScrollArea(QWidget *parent) :
    QScrollArea(parent)
{
    this->setAttribute(Qt::WA_AcceptTouchEvents,true);

}

/*
void SwipeScrollArea::mousePressEvent(QMouseEvent *e)
{
    yMouse = e->y();

    //scrollContentsBy (0,200);
}
void SwipeScrollArea::mouseMoveEvent(QMouseEvent *e)
{
    int dy = yMouse - e->y();
    if (dy > 0)
    {
        this->verticalScrollBar()->setValue(verticalScrollBar()->value() + 5);
        yMouse +=400;
    }
    else
    {
        verticalScrollBar()->setValue(verticalScrollBar()->value() - 5);
        yMouse -= 400;
    }
}

void SwipeScrollArea::mouseReleaseEvent(QMouseEvent *e)
{
    yMouse = 0;
}
*/
