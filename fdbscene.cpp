#include "fdbscene.h"
#include "lineitem.h"
#include <QDebug>
#include <QApplication>
#include <QMouseEvent>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>

FDBScene::FDBScene(QObject *parent) :
    QGraphicsScene(parent)
{
    initialCableLine = 0;
    mode = MoveItem;
    enableDrawLine = false;
    myLineColor = Qt::black;
}

void FDBScene::setMode(Mode mode)
{
     this->mode = mode;
}

int FDBScene::getMode()
{
    return mode;
}

void FDBScene::setItemType(LogicItem::LogicType logicType)
{
    this->logicType = logicType;
}

void FDBScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    if(event->button() != Qt::LeftButton)
        return;

    if(enableDrawLine)
        mode = InsertLine;

    LogicItem *logicItem;

    switch(mode)
    {
        case InsertItem:
            addPrimitive(event->scenePos());
            logicItem = new LogicItem(logicType);
            emit itemInserted(logicItem);
            break;
        case InsertLine:            
            addInitialCableLineAtPoint(event->scenePos());
            break;         
        case InsertText:
            //...
            break;
        default:
            ;
    }
    QGraphicsScene::mousePressEvent(event);
}

void FDBScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    updatePin(event->scenePos());
    updateInitialLine(event->scenePos());

    if(mode == MoveItem)
        QGraphicsScene::mouseMoveEvent(event);
}

void FDBScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(mode == InsertLine && initialCableLine != 0)
        linkItemsWithFinalCableLine();

    initialCableLine = 0;

    if(linkedStartPin != 0){
        this->removeEventFilter(linkedStartPin);
        linkedStartPin = 0;
    }
    if(cursorHoverPin !=0)
    {
        this->removeEventFilter(cursorHoverPin);
        cursorHoverPin = 0;
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

void FDBScene::addPrimitive(QPointF pos)
{
    primitiveItem = new Primitive;
    primitiveItem->setPos(pos);
    this->addItem(primitiveItem);
    primitiveItems.append(primitiveItem);
}

void FDBScene::addInitialCableLineAtPoint(QPointF cursorScenePos)
{
    linkedStartPin = pinOnCursor(cursorScenePos);

    if(linkedStartPin != 0){
        this->installEventFilter(linkedStartPin);
        creatInitialCableLine();
        linkedStartPin->setInitialCableLine(initialCableLine);

    }
}

PrimitivePin *FDBScene::pinOnCursor(QPointF cursorScenePos)
{
    QList<QGraphicsItem* > itemsOnCursor = items(cursorScenePos);
    if( itemsOnCursor.count() &&
        itemsOnCursor.first()->type() == PrimitivePin::Type)
    {
       PrimitivePin *pin = qgraphicsitem_cast<PrimitivePin *>( itemsOnCursor.first());
       return pin;
    }
    return 0;
}

void FDBScene::creatInitialCableLine()
{
    initialCableLine = new QGraphicsLineItem;
    initialCableLine->setLine(QLineF(linkedStartPin->mapToScene(linkedStartPin->pinEndPoint()),
                    linkedStartPin->mapToScene(linkedStartPin->pinEndPoint())));
    initialCableLine->setPen(QPen(myLineColor,1));
    initialCableLine->setZValue(-1000);
    this->addItem(initialCableLine);
}

void FDBScene::updatePin(QPointF cursorScenePos)
{
    bool evenFilter = (linkedStartPin != 0 && cursorHoverPin != 0 && cursorHoverPin != linkedStartPin );

    if(evenFilter)
    {
        this->removeEventFilter(cursorHoverPin);
    }
    cursorHoverPin = pinOnCursor(cursorScenePos);

    if(evenFilter)
    {
        this->installEventFilter(cursorHoverPin);
    }

    if(cursorHoverPin)
        cursorHoverPin->setInitialCableLine(initialCableLine);

    enableDrawLine = (cursorHoverPin != 0 && cursorHoverPin->isShowAnchor(cursorScenePos));

}

void FDBScene::updateInitialLine(QPointF cursorScenePos)
{
    if(mode == InsertLine && initialCableLine != 0)
    {
        QLineF newLine(initialCableLine->line().p1(),cursorScenePos);
        initialCableLine->setLine(newLine);
    }
}

void FDBScene::linkItemsWithFinalCableLine()
{
    removeInitialCableLine();

    if(isLinkLegal())
        addFinalCableLine();

    enableDrawLine = false;
    setMode(MoveItem);
}

void FDBScene::removeInitialCableLine()
{
    initialCableLine->unsetCursor();
    itemsOnStartPointOfCableLine = items(initialCableLine->line().p1());
    if((itemsOnStartPointOfCableLine.count() && itemsOnStartPointOfCableLine.last() == initialCableLine))
        itemsOnStartPointOfCableLine.removeLast();

    itemsOnEndPointOfCableLine = items(initialCableLine->line().p2());
    if(itemsOnEndPointOfCableLine.count()&& itemsOnEndPointOfCableLine.last() == initialCableLine )
        itemsOnEndPointOfCableLine.removeLast();

    removeItem(initialCableLine);
    delete initialCableLine;
}

bool FDBScene::isLinkLegal()
{
    return (itemsOnStartPointOfCableLine.count() && itemsOnEndPointOfCableLine.count()
            && itemsOnStartPointOfCableLine.first()->type() == PrimitivePin::Type
            && itemsOnEndPointOfCableLine.first()->type() == PrimitivePin::Type
            && itemsOnStartPointOfCableLine.first() != itemsOnEndPointOfCableLine.first());
}

void FDBScene::addFinalCableLine()
{
    PrimitivePin *startPin = qgraphicsitem_cast<PrimitivePin *>(itemsOnStartPointOfCableLine.first());
    PrimitivePin *endPin = qgraphicsitem_cast<PrimitivePin *>(itemsOnEndPointOfCableLine.first());
    if(startPin->getOrientation()!= endPin->getOrientation()&& startPin->primitive()!= endPin->primitive())
    {
        LineItem *cableLine = new LineItem;

        startPin->setFinalCableLine(cableLine);
        endPin->setFinalCableLine(cableLine);

        cableLine->setStartPin(startPin);
        cableLine->setEndPin(endPin);
        cableLine->setZValue(-1000.0);
        addItem(cableLine);
        cableLine->updatePosition();
    }
}

void FDBScene::removePrimitiveItem(Primitive *primitiveItem)
{
    int index = primitiveItems.indexOf(primitiveItem);
    primitiveItems.removeAt(index);
}
