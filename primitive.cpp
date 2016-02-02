#include <QtWidgets>
#include "primitive.h"
#include "lineitem.h"
#include "fdbscene.h"
/********************************************************************************************************
 *
//PrimitiveRect

PrimitiveRect::PrimitiveRect(QGraphicsItem *parent):QGraphicsItem(parent)
{
    myOutlineColor = Qt::darkBlue;
    myBackgroundColor = Qt::white;
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setFlags(ItemIsMovable | ItemIsSelectable);

}

void PrimitiveRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QPen pen(myOutlineColor);
//    const qreal lod = item->levelOfDetailFromTransform(painter->worldTransform());
    painter->setPen(pen);
    painter->setBrush(myBackgroundColor);
    //painter->drawRect(QRectF(30,20,60,80));
    painter->drawRect(0,0,60,80);
}

QRectF PrimitiveRect::boundingRect() const
{
   // return QRectF(0,0,120,120);
    return QRectF(0,0,60,80);
}
*********************************************************************************************************/

//PrimitivePin

PrimitivePin::PrimitivePin(PinOrientation orientation, QGraphicsItem *parent):QGraphicsObject(parent)
{
    this->pinOrientation = orientation;
    anchorShow = false;
    isFlooting = true;
    pinLineColor = Qt::darkBlue;
    initialCableLine = 0;
    myPrimitive = qgraphicsitem_cast<Primitive *>(parent);

    setAcceptHoverEvents (true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

Primitive *PrimitivePin::primitive()
{
    return myPrimitive;
}

void PrimitivePin::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QPen pen(pinLineColor);
    painter->setPen(pen);

    switch(pinOrientation)
    {
    case Input:
        painter->drawLine(8,8,25,8);
        anchorRect = QRectF(0,4,8,8);
        if(anchorShow){
            drawAnchorRect(painter);
        }
        break;
    case Output:
        painter->drawLine(0,8,19,8);
        anchorRect = QRectF(19,4,8,8);
        if(anchorShow){
            drawAnchorRect(painter);
        }
        break;
    default:
        break;
    }
    painter->setPen(pen);

}

void PrimitivePin::drawAnchorRect(QPainter *painter)
{
    QPen pen(Qt::red);
    pen.setWidth(1);
    painter->setPen(pen);
    painter->drawRect(anchorRect);
}

void PrimitivePin::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    updateShapeAndCursor(false);
    QGraphicsItem::hoverLeaveEvent(event);
}

void PrimitivePin::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QPointF point = event->pos();
    if(isContainedByFloatAnchor(point)){
      updateShapeAndCursor(true);
      setCursor(Qt::CrossCursor);
    }
    else if(!isContainedByFloatAnchor(point) && anchorShow){
      updateShapeAndCursor(false);
    }
    qDebug()<<"HOVER:"<<point;

    QGraphicsItem::hoverMoveEvent(event);
}

bool PrimitivePin::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == this->scene()){
        qDebug()<<"FILTER:"<<event->type();
        if(event->type() == QEvent::GraphicsSceneMouseMove){
            QGraphicsSceneMouseEvent *sceneEvent = static_cast<QGraphicsSceneMouseEvent *>(event);
            QPointF point = sceneEvent->scenePos();
            updatePin(point);
        }
    }
    return QGraphicsObject::eventFilter(obj,event);
}

int PrimitivePin::getSceneMode()
{
   FDBScene *scene = static_cast<FDBScene*>(this->scene());
   return scene->getMode();
}

void PrimitivePin::updateShapeAndCursor(bool hoverOnAnchor)
{
    anchorShow = hoverOnAnchor;
//    setCursor(hoverOnAnchor ? Qt::CrossCursor : Qt::ArrowCursor);
    update();
}

QRectF PrimitivePin::boundingRect()const
{
    return QRectF(0,0,35,16);
}

QPointF PrimitivePin::pinEndPoint()
{
    if(pinOrientation == Input)
        return QPointF(8,8);
    else
        return QPointF(19,8);
}
void PrimitivePin::setInitialCableLine(QGraphicsLineItem *initialLine)
{
    initialCableLine = initialLine;
}

void PrimitivePin::setFinalCableLine(LineItem *finalLine)
{
    cableLine = finalLine;
    primitive()->addLine(finalLine);
    isFlooting = false;
}

void PrimitivePin::removeCableLine()
{
    if(cableLine !=0)
        this->primitive()->removeLine(cableLine);
    cableLine = 0;
    isFlooting = true;
}

bool PrimitivePin::isPinLegal()
{
    if(initialCableLine != 0)
    {
        QList<QGraphicsItem* > items = scene()->items(initialCableLine->line().p1());
        PrimitivePin *startPin = qgraphicsitem_cast<PrimitivePin *>(items.first());
        if(this->pinOrientation == startPin->pinOrientation
        || primitive() == startPin->primitive())
        {
            return false;
        }
    }
    return true;
}

//bool PrimitivePin::showAnchor(QPointF point, int mode)
//{
//    QPointF myPoint = mapFromScene(point);

//    if(anchorRect.contains(myPoint) && isFlooting )
//    {

//        if(mode == 1)//InsertLine = 1
//        {
//    //            anchorShow = true;
//    //            update();
//            UpdateShapeAndCursor(true);
//        }
//        return true;
//    }

//    if(!anchorRect.contains(myPoint) && anchorShow)
//    {
//        if(mode == 1)
//        {
//    //            anchorShow  = false;
//    //            update();
//            UpdateShapeAndCursor(false);
//        }
//        return false;
//    }
//    return false;
//}

bool PrimitivePin::isContainedByFloatAnchor(QPointF point)
{
    return (anchorRect.contains(point) && isFlooting);
}

bool PrimitivePin::updatePin(QPointF point)
{
    if(getSceneMode() == 1 && isShowAnchor(point)){//InsertLine = 1
        updateShapeAndCursor(true);
        return true;
    }
    if(anchorShow && !isContainedByFloatAnchor(mapFromScene(point)) ){
        updateShapeAndCursor(false);
        return false;
    }
}

bool PrimitivePin::isShowAnchor(QPointF point )
{
   QPointF myPoint = mapFromScene(point);
   return (isPinLegal() && isContainedByFloatAnchor(myPoint));
}

//Primitive

Primitive::Primitive()
{
    myOutlineColor = Qt::darkBlue;
    myBackgroundColor = Qt::white;
    inputPinCount = 2;
    outputPinCount =1;
    this->setAcceptHoverEvents (true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setFlag(ItemIsMovable,true);
    setFlag(ItemIsSelectable,true);

    for(int i=1;i<=inputPinCount;i++)
    {
      QPoint pos = QPoint(-26,this->boundingRect().height()/(inputPinCount+1)*i);
      PrimitivePin *pinItem = new PrimitivePin(PrimitivePin::Input,this);
      inputPinItemList.append(pinItem);
      pinItem->setPos(pos);
    }

    for(int i=1;i<=outputPinCount;i++)
    {
      QPoint pos = QPoint(60,this->boundingRect().height()/(outputPinCount+1)*i);
      PrimitivePin *pinItem = new PrimitivePin(PrimitivePin::Output,this);
      outputPinItemList.append(pinItem);
      pinItem->setPos(pos);
    }
}
void Primitive::setPinCount(int inputPinCount, int outputPinCount)
{
    this->inputPinCount = inputPinCount;
    this->outputPinCount = outputPinCount;
}

QPointF Primitive::pinEndPoint()
{
    return this->mapToParent(activePinEndPoint);
}

QPointF Primitive::topHorizontalPos()
{
   QPointF pointInScene = mapToScene(QPoint(0,0));
   return pointInScene;
}

QPointF Primitive::bottomHorizontalPos()
{
    QPointF pointInScene = mapToScene(QPoint(60,80));
    return  pointInScene;
}


void Primitive::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QPen pen(myOutlineColor);

    if(option->state & QStyle::State_Selected)
    {
        pen.setStyle(Qt::DotLine);
        pen.setWidth(1);
    }

//    const qreal lod = item->levelOfDetailFromTransform(painter->worldTransform());
    painter->setPen(pen);
    painter->setBrush(myBackgroundColor);
    //painter->drawRect(QRectF(30,20,60,80));
    painter->drawRect(0,0,60,80);
}

QRectF Primitive::boundingRect()const
{
    return QRectF(0,0,60,80);
}

void Primitive::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
{
    QGraphicsItem::mouseMoveEvent(event);
}

bool Primitive::sceneEventFilter ( QGraphicsItem * watched, QEvent * event )
{
     qDebug()<<"fuckkkkkkk";
        if(event->type() == QEvent::MouseMove){
            return true;
        }
    return QGraphicsItem::sceneEventFilter(watched, event);
}


QVariant Primitive::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if(change == QGraphicsItem::ItemPositionChange && scene())
    {
        foreach (LineItem *line, lineItems) {
            line->updatePosition();
        }
    }
    return QGraphicsItem::itemChange(change, value);
}

//当与当前图元所连接的图元被删除时，删除两者之间的连接线
void Primitive::removeLine(LineItem *line)
{
   int index = lineItems.indexOf(line);

   if(index > -1)
       lineItems.removeAt(index);
}

//当前图元被删除后，删除其所有的连接线
void Primitive::removeLines()
{
    foreach(LineItem *line,lineItems)
    {
        line->startPin()->removeCableLine();
        line->endPin()->removeCableLine();
        scene()->removeItem(line);
    }
}

void Primitive::addLine(LineItem *line)
{
    lineItems.append(line);
}
