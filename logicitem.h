#ifndef LOGICITEM_H
#define LOGICITEM_H

#include <QGraphicsItem>

QT_BEGIN_NAMESPACE
class QRectF;
class QString;
class QColor;
QT_END_NAMESPACE

class LogicItem : public QGraphicsItem
{
public:
    enum LogicType {ANDGate,ORGate,NOTGate,XORGate,SRGate,ONSGate};

    LogicItem(LogicType logicType, QWidget *parent = 0);
    ~LogicItem();

    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    QRectF setText(QString text);
    LogicType itemType() const {return myLogicType;}

public slots:
    void setNum(int num);

protected:


private:
    void drawPinLines(QPainter *painter,int lineNum);
    QColor myOutlineColor;
    QColor myBackgroundColor;
    QString myText;
    QStringList nameList;
    LogicType myLogicType;
    int lineNum;
};

#endif //  LOGICITEM_H
