#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>

#include <paintarea.h>

#include "fdbscene.h"

QT_BEGIN_NAMESPACE
class QAction;
class QToolBox;
class QSpinBox;
class QComboBox;
class QFontComboBox;
class QButtonGroup;
class QLineEdit;
class QGraphicsTextItem;
class QFont;
class QToolButton;
class QAbstractButton;
class QGraphicsView;
QT_END_NAMESPACE

enum LOGIC { AND, OR, NOT, NAND,XOR,SR,ONS  };

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:

private slots:
    void actionGroupClicked(QAction *actionLogic);
    void pointerTypeGroupClicked(int id);
    void addLink();
    void itemInserted(LogicItem *item);
    void deleteItem();
    void cut();
    void copy();
    void paste();
    void bringToFront();
    void sendToBack();
    void properties();
    void updateActions();
private: 

    void creatActions();
    void creatMenus();
    void creatToolbar();
    void creatCentralWidget();

    FDBScene *scene;
    PaintAreaView *area;

    QMenu *fileMenu;
    QMenu *editMenu;
    QToolBar *editToolBar;

    QButtonGroup *pointerTypeGroup;
    QActionGroup *actionGroup;

    QAction *exitAction;
    QAction *addAndGateAction;
    QAction *addOrGateAction;
    QAction *addNotGateAction;
    QAction *addTextAction;


    QAction *deleteAction;
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *bringToFrontAction;
    QAction *sendToBackAction;
    QAction *propertiesAction;



};

#endif // MAINWINDOW_H
