#include "initcableline.h"

InitCableLine::InitCableLine()
{
    filterItem = 0;
}

void InitCableLine::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(filterItem)
        this->installSceneEventFilter(filterItem);
    QGraphicsItem::mouseMoveEvent(event);
}

void InitCableLine::setFilterItem(QGraphicsItem *item)
{
    filterItem = item;
}
