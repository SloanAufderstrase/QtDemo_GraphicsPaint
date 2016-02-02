#include "paintarea.h"
#include "fdbscene.h"
#include <QDebug>

PaintAreaView::PaintAreaView(QWidget *parent) :
    QGraphicsView(parent)
{  
     setDragMode(RubberBandDrag);
}

void PaintAreaView::mouseMoveEvent ( QMouseEvent * event )
{
    if(getSceneMode() == 1){
        setDragMode(NoDrag);
        setCursor(Qt::ArrowCursor);
    }
    else{
        setDragMode(RubberBandDrag);
    }
    QGraphicsView::mouseMoveEvent(event);
}

int PaintAreaView::getSceneMode()
{
   FDBScene *scene = static_cast<FDBScene*>(this->scene());
   return scene->getMode();
}
