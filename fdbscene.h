#ifndef FDBSCENE_H
#define FDBSCENE_H

#include <QGraphicsScene>
#include "logicitem.h"
#include "primitive.h"
#include "initcableline.h"


class FDBScene : public QGraphicsScene
{
    Q_OBJECT
public:
    enum Mode { InsertItem, InsertLine, InsertText, MoveItem };
    explicit FDBScene(QObject *parent = 0);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    void removePrimitiveItem(Primitive *primitiveItem);
    int getMode();

signals:
    void itemInserted(LogicItem *item);

public slots:
    void setItemType(LogicItem::LogicType logicType);
    void setMode(Mode mode);

private:
    void addPrimitive(QPointF pos);
    void addInitialCableLineAtPoint(QPointF pos);
    PrimitivePin *pinOnCursor(QPointF cursorScenePos);
    void creatInitialCableLine();
    void updatePin(QPointF cursorScenePos);
    void updateInitialLine(QPointF cursorScenePos);
    void linkItemsWithFinalCableLine();
    void removeInitialCableLine();
    bool isLinkLegal();
    void addFinalCableLine();

    PrimitivePin *linkedStartPin;
    PrimitivePin *cursorHoverPin;
    Primitive *primitiveItem;

    bool enableDrawLine;//使能画线功能
    Mode mode;
    LogicItem::LogicType logicType;
    QGraphicsLineItem *initialCableLine;
    QColor myLineColor;

    QList<Primitive*> primitiveItems;
    QList<QGraphicsItem* > itemsOnStartPointOfCableLine;
    QList<QGraphicsItem* > itemsOnEndPointOfCableLine;
};

#endif // FDBSCENE_H
