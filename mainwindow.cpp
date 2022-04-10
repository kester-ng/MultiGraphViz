#include "mainwindow.h"
// #include "moc_MainWindow.cpp"
#include "ui_mainwindow.h"
#include "graph/QGVScene.h"
#include "graph/QGVNode.h"
#include "graph/QGVEdge.h"
#include "graph/QGVSubGraph.h"
#include "graph/GraphicNode.h"
#include <QMessageBox>
#include <QtWidgets>
#include "louvain/main_convert.h"
#include "louvain/main_community.h"
#include "dnppr/dnppr.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <QStringList>
#include <string>
#include "Graphics_view_zoom.h"
#include "graph/customgraphicsscene.h"

typedef unsigned char BYTE;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _scene = new QGVScene("DEMO", this);
    scene = new CustomGraphicsScene("DEMO", this);

    ui->graphicsView->setScene(scene);

    Graphics_view_zoom* z = new Graphics_view_zoom(ui->graphicsView);
    z->set_modifiers(Qt::NoModifier);

    connect(scene, SIGNAL(nodeContextMenu(GraphicNode*)), SLOT(nodeContextMenu(GraphicNode*)));
    connect(scene, SIGNAL(nodeDoubleClick(GraphicNode*)), SLOT(nodeDoubleClick(GraphicNode*)));

    // connect(_scene, SIGNAL(nodeContextMenu(QGVNode*)), SLOT(nodeContextMenu(QGVNode*)));
    // connect(_scene, SIGNAL(nodeDoubleClick(QGVNode*)), SLOT(nodeDoubleClick(QGVNode*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drawGraph()
{
    /*
    _scene->loadLayout("digraph test{node [style=filled,fillcolor=white];N1 -> N2;N2 -> N3;N3 -> N4;N4 -> N1;}");
    connect(_scene, SIGNAL(nodeContextMenu(QGVNode*)), SLOT(nodeContextMenu(QGVNode*)));
    connect(_scene, SIGNAL(nodeDoubleClick(QGVNode*)), SLOT(nodeDoubleClick(QGVNode*)));
    ui->graphicsView->setScene(_scene);
    return;
    */

    //Configure scene attributes
    /*
    _scene->setGraphAttribute("label", "DEMO");

    _scene->setGraphAttribute("splines", "ortho");
    _scene->setGraphAttribute("rankdir", "LR");
    //_scene->setGraphAttribute("concentrate", "true"); //Error !
    _scene->setGraphAttribute("nodesep", "0.4");

    _scene->setNodeAttribute("shape", "circle");
    _scene->setNodeAttribute("style", "filled");
    _scene->setNodeAttribute("fillcolor", "white");
    _scene->setNodeAttribute("height", "1.2");
    _scene->setEdgeAttribute("minlen", "3");
    //_scene->setEdgeAttribute("dir", "both");

    //Add some nodes
    QGVNode *node1 = _scene->addNode("Node 1");
    node1->setIcon(QImage(":/icons/cat.jpeg"));
    QGVNode *node2 = _scene->addNode("Node 2");
    node2->setIcon(QImage(":/icons/cat.jpeg"));
    QGVNode *node3 = _scene->addNode("Node 3");
    node3->setIcon(QImage(":/icons/cat.jpeg"));
    QGVNode *node4 = _scene->addNode("Node 4");
    node4->setIcon(QImage(":/icons/cat.jpeg"));
    QGVNode *node5 = _scene->addNode("Node 5");
    node5->setIcon(QImage(":/icons/cat.jpeg"));
    */
    //Add some edges
    /*
    QGVEdge *edgeTest = _scene->addEdge(node1, node2, "TTL");
    edgeTest->setAttribute("color", "red");
    _scene->addEdge(node1, node2, "Edge 1");
    _scene->addEdge(node1, node3, "Edge 2")->setAttribute("color", "blue");
    _scene->addEdge(node2, node3, "Edge 3");

    _scene->addEdge(node2, node4, "Edge 4")->setAttribute("color", "red");

    _scene->addEdge(node4, node3, "Edge 5")->setAttribute("color", "red");

    _scene->addEdge(node4, node2, "Edge 6");
    _scene->addEdge(node4, node2, "Edge 7");
    _scene->addEdge(node4, node2, "Edge 8");

    _scene->addEdge(node4, node5, "Edge 9");
    _scene->addEdge(node2, node5, "Edge 10");
    */

    /*
    QGVSubGraph *sgraph = _scene->addSubGraph("SUB1");
    sgraph->setAttribute("label", "OFFICE");

    QGVNode *snode1 = sgraph->addNode("PC0152");
    QGVNode *snode2 = sgraph->addNode("PC0153");

    _scene->addEdge(snode1, snode2, "RT7");

    _scene->addEdge(node3, snode1, "GB8");
    _scene->addEdge(node3, snode2, "TS9");*/

    /*
    QGVSubGraph *ssgraph = sgraph->addSubGraph("SUB2");
    ssgraph->setAttribute("label", "DESK");
    _scene->addEdge(snode1, ssgraph->addNode("PC0155"), "S10");*/

    //Layout scene
    _scene->applyLayout();

    //Fit in view
    ui->graphicsView->fitInView(_scene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::nodeContextMenu(GraphicNode *node)
{
    //Context menu exemple
    QString qstr = QString::fromStdString(node->name);
    QMenu menu(qstr);
    selected_node_to_zoom_in = node->name;
    menu.addSeparator();
    QAction* zoom_in_action = menu.addAction(tr("Zoom In"));
    connect(zoom_in_action, SIGNAL(triggered()), this, SLOT(zoom_in()));

    QAction *action = menu.exec(QCursor::pos());
    if(action == 0)
        return;
}

void MainWindow::nodeDoubleClick(GraphicNode *node)
{
    QString qstr = QString::fromStdString(node->name);
    QMessageBox::information(this, tr("Super Node Details"), tr("Node %1\nRadius: %2\nx: %3\ny: %4").arg(qstr).arg(node->radius).arg(node->x).arg(node->y));
}

// Simple load from file function.
// TODO Integrate with graph viz algo code.
void MainWindow::loadFromFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Edge List"), "", tr("Edge List (*.abk);;All Files (*)"));
    std::string utf8_text = fileName.toUtf8().constData();
    std::vector<std::string> args = {"-i", utf8_text, "-o", "test.bin"};
    std::vector<char*> cstrings;
    cstrings.reserve(args.size());
    for(size_t i = 0; i < args.size(); ++i)
        cstrings.push_back(const_cast<char*>(args[i].c_str()));
    // convert_edgelist_to_binary(4, &cstrings[0]);

    // Now we use louvain algorithm to get clustering information
    std::vector<std::string> louvain_args = {"-f", "6", "-a", "1", "-k", "25", "-v"};
    std::vector<char*> louvain;
    louvain.reserve(louvain_args.size());
    for (size_t i = 0; i < louvain_args.size(); ++i)
        louvain.push_back(const_cast<char*>(louvain_args[i].c_str()));
    // louvain_algorithm(louvain_args.size(), &louvain[0]);


    // grab the root of the cluster
    std::string name;
    std::ifstream rotifs;
    rotifs.open("rootname.root");

    while(!rotifs.eof())
    {
        rotifs >> name;
        std::cerr << name << std::endl;
    }

    std::string path = name;
    current_super_node = path;

    // now it is the dnppr algorithm
    std::vector<std::string> dnppr_args = {"approx_dnppr", "-f", "6", "-alg", "fpsn", "-build", "0", "-path", path}; // for now we only support taupush
    std::vector<char*> dnppr_cstrings;
    dnppr_cstrings.reserve(dnppr_args.size());
    for (size_t i = 0; i < dnppr_args.size(); i++)
        dnppr_cstrings.push_back(const_cast<char*>(dnppr_args[i].c_str()));
    // invoke method
    std::vector<std::vector<double>> coordinates = dnppr(dnppr_args.size(), &dnppr_cstrings[0]);

    std::string delimiter = "_";
    size_t pos = 0;
    std::string token;
    std::vector<std::string> nodes_name(coordinates.size());
    std::vector<std::string> parts(2);
    int index = 0;
    while ((pos = path.find(delimiter)) != std::string::npos) {
        token = path.substr(0, pos);
        parts[index] = token;
        index += 1;
        path.erase(0, pos + delimiter.length());
    }

    char level_chr =  parts[1][1];
    int level = level_chr - '0';
    level--; // decrease level by 1
    std::string lvl = parts[1];
    lvl[1] = '0' + level; // change level to 1 lower

    for (int i = 0; i < coordinates.size(); i++) {
        double node_label = coordinates[i][3];
        int node_label_int = (int) node_label;
        std::string super_node_name = parts[0] + delimiter + lvl + delimiter + std::to_string(node_label_int);
        // std::cerr << super_node_name << std::endl;
        nodes_name[i] = super_node_name;
    }

    // plot graph
    for (int i = 0; i < coordinates.size(); i++) {
        GraphicNode* node = new GraphicNode(coordinates[i][0], coordinates[i][1], coordinates[i][2], nodes_name[i]);
        scene->addItem(node);
    }
    std::string command = "python3 /home/kester/MultiGraphViz/load-superppr-viz.py --supernode=" + name;
    std::system(command.c_str());
    std::string graph_file = name + std::string("-edge-list.txt");
    FILE *fin = fopen(graph_file.c_str(), "r");
    int t1, t2;
    // 1 9, 8 14, 9 24
    while (fscanf(fin, "%d%d", &t1, &t2) != EOF) {
        if(t1 == t2) continue;
        // plot edges
        double x = coordinates[t1][0];
        double y = coordinates[t1][1];
        double radius = coordinates[t1][2];

        double x2 = coordinates[t2][0];
        double y2 = coordinates[t2][1];
        double radius2 = coordinates[t2][2];

        QGraphicsLineItem* line = new QGraphicsLineItem();
        QPen _Pen;
        _Pen.setColor(Qt::black);
        _Pen.setWidth(0.5);
        line->setLine(x + radius, y + radius, x2 + radius2, y2 + radius2);
        line->setPen(_Pen);
        scene->addItem(line);
    }
    this->setWindowTitle(current_super_node.c_str());
}

void MainWindow::zoom_in() {
    std::string path = selected_node_to_zoom_in;
    std::string prev = current_super_node;
    stack.push(prev); // add to history to pop when zooming out

    current_super_node = selected_node_to_zoom_in;
    std::string delimiter = "_";
    size_t pos = 0;
    std::string token;
    std::vector<std::string> parts(2);
    int index = 0;
    while ((pos = path.find(delimiter)) != std::string::npos) {
        token = path.substr(0, pos);
        parts[index] = token;
        index += 1;
        path.erase(0, pos + delimiter.length());
    }

    char level_chr =  parts[1][1];
    int level = level_chr - '0';

    if (level == 0) {
        return; // cannot zoom in anymore
    }

    level--; // decrease level by 1
    std::string lvl = parts[1];
    lvl[1] = '0' + level; // change level to 1 lower
    std::cerr << "Zooming in into " + selected_node_to_zoom_in << std::endl;

    // clear the scene first
    scene->clear();
    std::string temp = selected_node_to_zoom_in;
    std::vector<std::vector<double>> coordinates = visualize(temp);
    std::vector<std::string> nodes_name(coordinates.size());


    for (int i = 0; i < coordinates.size(); i++) {
        double node_label = coordinates[i][3];
        int node_label_int = (int) node_label;
        std::string super_node_name = parts[0] + delimiter + lvl + delimiter + std::to_string(node_label_int);
        std::cerr << super_node_name << std::endl;
        nodes_name[i] = super_node_name;
    }

    // plot graph
    for (int i = 0; i < coordinates.size(); i++) {
        GraphicNode* node = new GraphicNode(coordinates[i][0], coordinates[i][1], coordinates[i][2], nodes_name[i]);
        scene->addItem(node);
    }

    std::string command = "python3 /home/kester/MultiGraphViz/load-superppr-viz.py --supernode=" + selected_node_to_zoom_in;
    std::system(command.c_str());
    std::string graph_file = selected_node_to_zoom_in + "-edge-list.txt";
    FILE *fin = fopen(graph_file.c_str(), "r");
    int t1, t2;
    // 1 9, 8 14, 9 24
    while (fscanf(fin, "%d%d", &t1, &t2) != EOF) {
        if(t1 == t2) continue;
        // plot edges
        double x = coordinates[t1][0];
        double y = coordinates[t1][1];
        double radius = coordinates[t1][2];

        double x2 = coordinates[t2][0];
        double y2 = coordinates[t2][1];
        double radius2 = coordinates[t2][2];

        QGraphicsLineItem* line = new QGraphicsLineItem();
        QPen _Pen;
        _Pen.setColor(Qt::black);
        _Pen.setWidth(0.5);
        line->setLine(x + radius, y + radius, x2 + radius2, y2 + radius2);
        line->setPen(_Pen);
        scene->addItem(line);
    }
    this->setWindowTitle(current_super_node.c_str());
}

void MainWindow::on_actionLoad_triggered()
{
    loadFromFile();
}


void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionConvert_to_IMG_triggered()
{
    QString filename = "c0_l2_838.png";
    QPixmap pixMap = ui->graphicsView->grab();
    pixMap.save(filename);
}

void MainWindow::on_actionZoom_Out_triggered() {
    if (stack.size() == 0)
        return; // nothing to zoom out

    std::string path = stack.top();
    stack.pop();
    current_super_node = path; // set back current super_node
    std::string temp = path;

    std::string delimiter = "_";
    size_t pos = 0;
    std::string token;
    std::vector<std::string> parts(2);
    int index = 0;
    while ((pos = path.find(delimiter)) != std::string::npos) {
        token = path.substr(0, pos);
        parts[index] = token;
        index += 1;
        path.erase(0, pos + delimiter.length());
    }

    char level_chr =  parts[1][1];
    int level = level_chr - '0';

    if (level == 0) {
        return; // cannot zoom in anymore
    }

    level--; // decrease level by 1
    std::string lvl = parts[1];
    lvl[1] = '0' + level; // change level to 1 lower
    std::cerr << "Zooming in into " + temp << std::endl;

    // clear the scene first
    scene->clear();
    std::vector<std::vector<double>> coordinates = visualize(temp);
    std::vector<std::string> nodes_name(coordinates.size());


    for (int i = 0; i < coordinates.size(); i++) {
        double node_label = coordinates[i][3];
        int node_label_int = (int) node_label;
        std::string super_node_name = parts[0] + delimiter + lvl + delimiter + std::to_string(node_label_int);
        std::cerr << super_node_name << std::endl;
        nodes_name[i] = super_node_name;
    }

    // plot graph
    for (int i = 0; i < coordinates.size(); i++) {
        GraphicNode* node = new GraphicNode(coordinates[i][0], coordinates[i][1], coordinates[i][2], nodes_name[i]);
        scene->addItem(node);
    }

    std::string command = "python3 /home/kester/MultiGraphViz/load-superppr-viz.py --supernode=" + temp;
    std::system(command.c_str());
    std::string graph_file = temp + "-edge-list.txt";
    FILE *fin = fopen(graph_file.c_str(), "r");
    int t1, t2;
    // 1 9, 8 14, 9 24
    while (fscanf(fin, "%d%d", &t1, &t2) != EOF) {
        if(t1 == t2) continue;
        // plot edges
        double x = coordinates[t1][0];
        double y = coordinates[t1][1];
        double radius = coordinates[t1][2];

        double x2 = coordinates[t2][0];
        double y2 = coordinates[t2][1];
        double radius2 = coordinates[t2][2];

        QGraphicsLineItem* line = new QGraphicsLineItem();
        QPen _Pen;
        _Pen.setColor(Qt::black);
        _Pen.setWidth(0.5);
        line->setLine(x + radius, y + radius, x2 + radius2, y2 + radius2);
        line->setPen(_Pen);
        scene->addItem(line);
    }
    this->setWindowTitle(current_super_node.c_str());
}

