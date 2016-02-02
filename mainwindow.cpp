#include "mainwindow.h"
#include "lineitem.h"
#include <QtWidgets>
#include <QList>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent)
{

    scene = new FDBScene;
    scene->setSceneRect(0,0,5000,5000);

    creatActions();
    creatMenus();
    creatToolbar();
    creatCentralWidget();

    connect(scene,SIGNAL(itemInserted(LogicItem*)),this,SLOT(itemInserted(LogicItem*)));

}

void MainWindow::creatActions()
{
    //LogicItem

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    addAndGateAction = new QAction(tr("Insert AND"), this);
    addAndGateAction->setCheckable(true);
    addAndGateAction->setIcon(QIcon(":/images/node.png"));
    addAndGateAction->setShortcut(tr("Ctrl+N"));

    addOrGateAction = new QAction(tr("OR"), this);
    addOrGateAction->setCheckable (true);

    addNotGateAction = new QAction(tr("NOT"), this);
    addNotGateAction->setCheckable(true);
    addNotGateAction->setIcon(QIcon());

    addTextAction = new QAction(tr("Insert Text"),this);
    addTextAction->setCheckable(true);

    actionGroup = new QActionGroup(this);
    actionGroup->addAction(addAndGateAction);
    actionGroup->addAction(addOrGateAction);
    actionGroup->addAction(addNotGateAction);
    actionGroup->setExclusive(true);
    connect(actionGroup,SIGNAL(triggered(QAction*)),this,SLOT(actionGroupClicked(QAction*)));

    deleteAction = new QAction(tr("&Delete"), this);
    deleteAction->setIcon(QIcon(":/images/delete.png"));
    deleteAction->setShortcut(tr("Del"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteItem()));

    cutAction = new QAction(tr("Cu&t"), this);
    cutAction->setIcon(QIcon(":/images/cut.png"));
    cutAction->setShortcut(tr("Ctrl+X"));
    connect(cutAction, SIGNAL(triggered()), this, SLOT(cut()));

    copyAction = new QAction(tr("&Copy"), this);
    copyAction->setIcon(QIcon(":/images/copy.png"));
    copyAction->setShortcut(tr("Ctrl+C"));
    connect(copyAction, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAction = new QAction(tr("&Paste"), this);
    pasteAction->setIcon(QIcon(":/images/paste.png"));
    pasteAction->setShortcut(tr("Ctrl+V"));
    connect(pasteAction, SIGNAL(triggered()), this, SLOT(paste()));

    bringToFrontAction = new QAction(tr("Bring to &Front"), this);
    bringToFrontAction->setIcon(QIcon(":/images/bringtofront.png"));
    connect(bringToFrontAction, SIGNAL(triggered()),
            this, SLOT(bringToFront()));

    sendToBackAction = new QAction(tr("&Send to Back"), this);
    sendToBackAction->setIcon(QIcon(":/images/sendtoback.png"));
    connect(sendToBackAction, SIGNAL(triggered()),
            this, SLOT(sendToBack()));

    propertiesAction = new QAction(tr("P&roperties..."), this);
    connect(propertiesAction, SIGNAL(triggered()),
            this, SLOT(properties()));

}

void MainWindow::creatMenus()
{

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(exitAction);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(deleteAction);
    editMenu->addSeparator();
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    editMenu->addSeparator();
    editMenu->addAction(bringToFrontAction);
    editMenu->addAction(sendToBackAction);
    editMenu->addSeparator();
    editMenu->addAction(propertiesAction);
}

void MainWindow::creatToolbar()
{
    QToolButton *pointerButton = new QToolButton;
    pointerButton->setCheckable(true);
    pointerButton->setChecked(true);
    pointerButton->setIcon(QIcon(":/images/pointer.png"));

    QToolButton *linkButton = new QToolButton;
    linkButton->setCheckable(true);
    linkButton->setIcon(QIcon(":/images/link.png"));

    pointerTypeGroup = new QButtonGroup;
    pointerTypeGroup->addButton(pointerButton,FDBScene::MoveItem);
    pointerTypeGroup->addButton(linkButton,FDBScene::InsertLine);
    connect(pointerTypeGroup,SIGNAL(buttonClicked(int)),this,SLOT(pointerTypeGroupClicked(int)));

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(addAndGateAction);
    editToolBar->addAction(addOrGateAction);
    editToolBar->addAction(addOrGateAction);
    editToolBar->addAction(addNotGateAction);
    //...
    editToolBar->addSeparator();

    editToolBar->addAction(cutAction);
    editToolBar->addAction(copyAction);
    editToolBar->addAction(pasteAction);
    editToolBar->addSeparator();
    editToolBar->addAction(deleteAction);
    editToolBar->addAction(bringToFrontAction);
    editToolBar->addAction(sendToBackAction);
    editToolBar->addWidget(pointerButton);
    editToolBar->addWidget(linkButton);
}

void MainWindow::creatCentralWidget()
{
    area = new PaintAreaView;
    area->setScene(scene);
    area->setMouseTracking(true);
    area->setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);
    area->setBackgroundBrush(QColor("bisque"));
    setCentralWidget(area);
}



void MainWindow::actionGroupClicked(QAction *actionLogic)
{
    int actionIndex = -1;
    QList<QAction*> actions = actionGroup->actions();

    foreach (QAction *action, actions){
        if(actionLogic->text() != action->text())
            action->setChecked(false);
        else
            actionIndex = actions.indexOf(action);
    }

    if(actionLogic->text() == "Insert Text")
        scene->setMode(FDBScene::InsertText);
    else{
        scene->setMode(FDBScene::InsertItem);
        scene->setItemType(LogicItem::LogicType(actionIndex));
    }
}

void MainWindow::pointerTypeGroupClicked(int id)
{
   scene->setMode(FDBScene::Mode(id));
}

void MainWindow::itemInserted(LogicItem *item)
{
    pointerTypeGroup->button(FDBScene::MoveItem)->setChecked(true);
    scene->setMode(FDBScene::Mode(pointerTypeGroup->checkedId()));
    actionGroup->actions().at(item->itemType())->setChecked(false);
}

void MainWindow::addLink()
{

}

void MainWindow::deleteItem()
{
    foreach(QGraphicsItem *item,scene->selectedItems())
    {
        qDebug()<<item->type()<<LineItem::Type<<Primitive::Type;
        if(item->type() == LineItem::Type)
        {
           scene->removeItem(item);
           LineItem *line = qgraphicsitem_cast<LineItem *>(item);

           line->startPin()->removeCableLine();
           line->endPin()->removeCableLine();

           delete item;
        }
    }
    foreach(QGraphicsItem *item,scene->selectedItems())
    {
        if(item->type() == Primitive::Type)
        {
           Primitive *primitiveItem = qgraphicsitem_cast<Primitive *>(item);
           primitiveItem->removeLines();

           scene->removePrimitiveItem(primitiveItem);
           scene->removeItem(item);
           delete item;

        }
    }
}

void MainWindow::cut()
{

}

void MainWindow::copy()
{

}

void MainWindow::paste()
{

}

void MainWindow::bringToFront()
{

}

void MainWindow::sendToBack()
{

}

void MainWindow::properties()
{

}

void MainWindow::updateActions()
{

}


MainWindow::~MainWindow()
{

}
