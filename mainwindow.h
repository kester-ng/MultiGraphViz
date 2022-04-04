#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "graph/QGVScene.h"
#include <QWidget>
#include <QMap>
#include "graph/GraphicNode.h"

QT_BEGIN_NAMESPACE
class QPushButton;
class QLabel;
class QLineEdit;
class QTextEdit;
QT_END_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void drawGraph();

public slots:
    void loadFromFile();

private slots:
    void nodeContextMenu(GraphicNode* node);
    void nodeDoubleClick(GraphicNode* node);

    void on_actionLoad_triggered();
    void on_actionQuit_triggered();
    void on_actionConvert_to_IMG_triggered();

private:
    Ui::MainWindow *ui;
    QGVScene *_scene;  
    QGraphicsScene *scene;
    QPushButton *loadButton;
};

#endif // MAINWINDOW_H
