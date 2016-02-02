#ifndef INITCABLELINE_H
#define INITCABLELINE_H

#include <QGraphicsLineItem>

class InitCableLine : public QGraphicsLineItem
{
public:
    InitCableLine();
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void setFilterItem(QGraphicsItem *item);
private:
    QGraphicsItem *filterItem;
};

#endif // INITCABLELINE_H
