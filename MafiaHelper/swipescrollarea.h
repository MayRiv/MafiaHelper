#ifndef SWIPESCROLLAREA_H
#define SWIPESCROLLAREA_H

#include <QScrollArea>

class SwipeScrollArea : public QScrollArea
{
    Q_OBJECT

public:
    explicit SwipeScrollArea(QWidget *parent = 0);

signals:

public slots:
private:
    int yMouse;
/*
    void mousePressEvent ( QMouseEvent * e );
    void mouseMoveEvent ( QMouseEvent * e );
    void mouseReleaseEvent ( QMouseEvent * e ) ;
*/
};

#endif // SWIPESCROLLAREA_H
