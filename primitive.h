#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <QGraphicsItem>


QT_BEGIN_NAMESPACE
class QRectF;
class QString;
class QColor;
QT_END_NAMESPACE

class LineItem;
class Primitive;



class PrimitivePin : public QGraphicsObject
{
public:
    enum PinOrientation {Input,Output};
    int getOrientation(){return pinOrientation;}

    PrimitivePin(PinOrientation orientation, /*QGraphicsItem*/QGraphicsItem *parent = 0);

    enum {Type = UserType + 10};
    int type()const {return Type;}

    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

    QRectF boundingRect()const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);


    bool isShowAnchor(QPointF point);
    QPointF pinEndPoint();

    void setFinalCableLine(LineItem *finalLine);
    void setInitialCableLine(QGraphicsLineItem *initialLine);
    void removeCableLine();

    Primitive *primitive();

private:
     bool updatePin(QPointF point);
     bool isPinLegal();
    bool isContainedByFloatAnchor(QPointF point);
    void updateShapeAndCursor(bool hoverOnAnchor);
    void drawAnchorRect(QPainter *painter);
    int getSceneMode();

    QColor pinLineColor;
    bool anchorShow;
    bool isFlooting;
    PinOrientation pinOrientation;
    QRectF anchorRect;
    LineItem *cableLine;
    Primitive *myPrimitive;
    QGraphicsLineItem *initialCableLine;
//    int sceneMode;

};


class Primitive : public QGraphicsItem
{

public:
    enum{ Type = UserType +15};
    Primitive();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect()const;
    void mouseMoveEvent( QGraphicsSceneMouseEvent * event );
    int type() const{return Type;}

    void removeLine(LineItem *line);
    void removeLines();
    void addLine(LineItem *line);

    void setPinCount(int inputPinCount, int outputPinCount);
    bool sceneEventFilter ( QGraphicsItem * watched, QEvent * event );


    QPointF pinEndPoint();
    QPointF topHorizontalPos();
    QPointF bottomHorizontalPos();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    int inputPinCount;
    int outputPinCount;
    QColor myOutlineColor;
    QColor myBackgroundColor;
    QPointF activePinEndPoint;//鼠标所触发的引脚端点的坐标值
    QList<PrimitivePin *> inputPinItemList;
    QList<PrimitivePin *> outputPinItemList;
    QList<LineItem*> lineItems;

};
#endif // PRIMIVITE_H
