#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "graph/QGVScene.h"
#include <QWidget>
#include <QMap>
#include "graph/GraphicNode.h"
#include <stack>
#include <map>
#include "graph/visualisation.h"

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
    void zoom_in();

    void on_actionLoad_triggered();
    void on_actionQuit_triggered();
    void on_actionConvert_to_IMG_triggered();
    void on_actionZoom_Out_triggered();

    void plot_graph_using_cache(std::string name);

private:
    Ui::MainWindow *ui;
    QGVScene *_scene;  
    QGraphicsScene *scene;
    QPushButton *loadButton;
    std::string selected_node_to_zoom_in;
    std::string current_super_node;
    std::stack<std::string> stack;
    std::map<std::string, Visualisation> cache;
};

#endif // MAINWINDOW_H
