#ifndef LINEITEM_H
#define LINEITEM_H

#include <QGraphicsItem>
#include <primitive.h>

class LineItem : public /*QGraphicsLineItem*/QGraphicsPathItem
{
public:
    enum{Type = UserType +4};
    LineItem();

    void paint (QPainter *painter,
                const QStyleOptionGraphicsItem *option,
                QWidget * widget);

    Primitive *startItem()const {return myStartItem;}
    Primitive *endItem()const {return myEndItem;}

    PrimitivePin *startPin()const {return myStartPin;}
    PrimitivePin *endPin()const {return myEndPin;}
    int type() const{return Type;}

    void setItem(Primitive *startItem,Primitive *endItem);
    void setStartPin(PrimitivePin *startPin);
    void setEndPin(PrimitivePin *endPin);

    void updatePosition();

private:
    QPolygonF getPoints();

    Primitive *myStartItem;
    Primitive *myEndItem;

    PrimitivePin *myStartPin;
    PrimitivePin *myEndPin;
};

#endif // LINEITEM_H
