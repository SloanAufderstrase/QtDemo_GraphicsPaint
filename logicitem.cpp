#include <QtGui>
#include <QtWidgets>
#include "logicitem.h"


LogicItem::LogicItem(LogicType logicType, QWidget *parent)
{
    myLogicType = logicType;
    lineNum = 3;
    myOutlineColor = Qt::darkBlue;
    myBackgroundColor = Qt::white;
    setFlags(ItemIsMovable | ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    nameList<<"AND"<<"OR"<<"NOT"<<"NAND"<<"XOR"<<"SR"<<"ONS";
}

QRectF LogicItem::boundingRect() const
{
    return QRectF(0,0,120,120);
}

//QPainterPath LogicItem::shape() const
//{

//}


//void LogicItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
//{

//}

//void LogicItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
//{
//    QGraphicsItem::mousePressEvent(event);
//    update();
//}

//void LogicItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
//{
//    QGraphicsItem::mouseReleaseEvent(event);
//    update();
//}

//QRectF LogicItem::outLineRect() const
//{
//    const int Padding = 5;
//    QFontMetricsF metrics = qApp->font();
//    QRectF rect = metrics.boundingRect(myText);
//    rect.adjust(-Padding,-Padding,+Padding,+Padding);
//    return rect;
//}

QRectF LogicItem::setText(QString text)
{
    prepareGeometryChange();
    myText = text;
    update();
}

void LogicItem::setNum(int num)
{
    lineNum = num;
}

void LogicItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    QPen pen(myOutlineColor);

    if(item->state & QStyle::State_Selected)
    {
        pen.setStyle(Qt::DotLine);
        pen.setWidth(1);
    }
    const qreal lod = item->levelOfDetailFromTransform(painter->worldTransform());
    painter->setPen(pen);
    painter->setBrush(myBackgroundColor);
    painter->drawRect(QRectF(30,20,60,80));
    painter->drawText(30,15,myText);

    switch(myLogicType){
        case ANDGate:
            painter->drawText(50,30,"AND");
            painter->drawText(30,15,myText);
            drawPinLines(painter,lineNum);
            break;
        case ORGate:
            painter->drawText(50,30,"OR");
            painter->drawText(30,15,myText);
            drawPinLines(painter,lineNum);
            break;
        case NOTGate:
            painter->drawText(50,30,"NOT");
            painter->drawText(30,15,myText);
            drawPinLines(painter,lineNum);
            break;
        default:
            break;
    }
}

void LogicItem::drawPinLines(QPainter *painter,int lineNum)
{

//    if (lod >= 1) {
//        painter->setPen(QPen(myOutlineColor, 1));
//        painter->drawLine(31, 101, 91, 101);
//        painter->drawLine(91, 21, 91, 101);
//        painter->setPen(pen);
//    }
    QVarLengthArray<QLineF,3> lines;
    lines.append(QLineF(4,42,29,42));
    lines.append(QLineF(4,78,29,78));
    lines.append(QLineF(91,60,116,60));

    painter->drawLines(lines.data(),lines.size());
}


LogicItem:: ~LogicItem(){}
