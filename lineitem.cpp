#include "lineitem.h"
#include <QtWidgets>
#include <QDebug>
LineItem::LineItem()
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    QPen pen;
    pen.setWidth(0);
    setPen(pen);

    myStartPin = 0;
    myEndPin = 0;
    myStartItem = 0;
    myEndItem = 0;


}

void LineItem::setItem(Primitive *startItem,Primitive *endItem)
{
   myStartItem = startItem;
   myEndItem = endItem;
}

void LineItem::setStartPin(PrimitivePin *startPin)
{
    myStartPin = startPin;
}

void LineItem::setEndPin(PrimitivePin *endPin)
{
    myEndPin = endPin;
}

void LineItem::updatePosition()
{
    QPainterPath path;
    path.addPolygon(getPoints());
    this->setPath(path);
}

QPolygonF LineItem::getPoints()
{
    QPointF outputPoint;
    QPointF inputPoint;

    qreal topOfInputPrimitive;
    qreal bottomOfInputPrimitive;

    qreal topOfOutputPrimitive;
    qreal bottomOfOutputPrimitive;


    if(myStartPin->getOrientation() == PrimitivePin::Output) {
         outputPoint = mapFromItem(myStartPin,myStartPin->pinEndPoint());
         inputPoint = mapFromItem(myEndPin,myEndPin->pinEndPoint());

         topOfInputPrimitive = QPointF(mapFromItem(myEndPin->primitive(),QPointF(0,0))).ry();
         bottomOfInputPrimitive = QPointF(mapFromItem(myEndPin->primitive(),QPointF(60,80))).ry();
         topOfOutputPrimitive = QPointF(mapFromItem(myStartPin->primitive(),QPointF(0,0))).ry();
         bottomOfOutputPrimitive = QPointF(mapFromItem(myStartPin->primitive(),QPointF(60,80))).ry();
    }
    else {
         outputPoint = mapFromItem(myEndPin,myEndPin->pinEndPoint());
         inputPoint = mapFromItem(myStartPin,myStartPin->pinEndPoint());

         topOfInputPrimitive = QPointF(mapFromItem(myStartPin->primitive(),QPointF(0,0))).ry();
         bottomOfInputPrimitive = QPointF(mapFromItem(myStartPin->primitive(),QPointF(60,80))).ry();
         topOfOutputPrimitive = QPointF(mapFromItem(myEndPin->primitive(),QPointF(0,0))).ry();
         bottomOfOutputPrimitive = QPointF(mapFromItem(myEndPin->primitive(),QPointF(60,80))).ry();
    }

    if(outputPoint.rx() <= inputPoint.rx()){

        QPointF wendePoint1 = QPointF((outputPoint.rx()+inputPoint.rx())/2.0,outputPoint.ry());
        QPointF wendePoint2 = QPointF((outputPoint.rx()+inputPoint.rx())/2.0,inputPoint.ry());
        QPolygonF polygon;
        polygon << outputPoint << wendePoint1 << wendePoint2 << inputPoint;
        return polygon;
    }


    if(outputPoint.rx() > inputPoint.rx() /*&& outputPoint.ry() > inputPoint.ry()*/
      && (topOfOutputPrimitive-bottomOfInputPrimitive > 5.0
            || topOfInputPrimitive-bottomOfOutputPrimitive > 5.0))
    {
         QPointF wendePoint1 = QPointF((inputPoint.rx()-8),inputPoint.ry());
         QPointF wendePoint2 = QPointF((inputPoint.rx()-8),((topOfOutputPrimitive+bottomOfInputPrimitive)/2.0));
         QPointF wendePoint3 = QPointF(outputPoint.rx(),((topOfOutputPrimitive+bottomOfInputPrimitive)/2.0));
         QPolygonF polygon;
         polygon << inputPoint << wendePoint1 << wendePoint2 << wendePoint3 << outputPoint;
         return polygon;
    }

    if(outputPoint.rx() > inputPoint.rx() && (topOfOutputPrimitive-bottomOfInputPrimitive <= 5.0)
        && (bottomOfOutputPrimitive-bottomOfInputPrimitive) > 0.0)
    {
        QPointF wendePoint1 = QPointF((inputPoint.rx()-8),inputPoint.ry());
        QPointF wendePoint2 = QPointF((inputPoint.rx()-8),bottomOfOutputPrimitive+5);
        QPointF wendePoint3 = QPointF((outputPoint.rx()),bottomOfOutputPrimitive+5);
        QPolygonF polygon;
        polygon << inputPoint << wendePoint1 << wendePoint2 << wendePoint3 << outputPoint;
        return polygon;
    }

    if(outputPoint.rx() > inputPoint.rx() && bottomOfInputPrimitive-bottomOfOutputPrimitive >= 0.0
                                              && topOfInputPrimitive-bottomOfOutputPrimitive <= 5.0)
    {
        QPointF wendePoint1 = QPointF((inputPoint.rx()-8),inputPoint.ry());
        QPointF wendePoint2 = QPointF((inputPoint.rx()-8),bottomOfInputPrimitive+5);
        QPointF wendePoint3 = QPointF((outputPoint.rx()),bottomOfInputPrimitive+5);
        QPolygonF polygon;
        polygon << inputPoint << wendePoint1 << wendePoint2 << wendePoint3 << outputPoint;
        return polygon;
    }

    return QPolygonF();

}

void LineItem::paint (QPainter *painter,
            const QStyleOptionGraphicsItem *option,
            QWidget * widget)
{
    QPen pen;
    if(option->state & QStyle::State_Selected)
    {
        pen.setStyle(Qt::DashDotLine);
        pen.setWidth(1);
    }

    QGraphicsPathItem::paint(painter,option,widget);
}
