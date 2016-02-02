#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QGraphicsView>


class PaintAreaView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit PaintAreaView(QWidget *parent = 0);
    void mouseMoveEvent ( QMouseEvent * event );
private:
    int getSceneMode();
};

#endif // PAINTAREA_H
