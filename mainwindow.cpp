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

typedef unsigned char BYTE;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _scene = new QGVScene("DEMO", this);
    scene = new QGraphicsScene(this);

    ui->graphicsView->setScene(scene);
    this->setWindowTitle("c0_l2_838");

    Graphics_view_zoom* z = new Graphics_view_zoom(ui->graphicsView);
    z->set_modifiers(Qt::NoModifier);
    /*
    std::ifstream file("/home/kester/actual_idx/inputds250_25c0_l3_0_fpsn.y", std::ios::out | std::ios::binary);
    file.unsetf(std::ios::skipws);
    GraphicNode* node = new GraphicNode(0, 20, 10);
    scene->addItem(node);
    GraphicNode* node1 = new GraphicNode(200, 0, 10);
    GraphicNode* node2 = new GraphicNode(0, 0, 10);
    scene->addItem(node1);
    // scene->addItem(node2);
    QGraphicsLineItem* line = new QGraphicsLineItem();
    line->setLine(10, 30, 210, 10);
    scene->addItem(line);
    */
    // inputds250_25c0_l3_0_fpsn.r

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

void MainWindow::nodeContextMenu(QGVNode *node)
{
    //Context menu exemple
    QMenu menu(node->label());

    menu.addSeparator();
    menu.addAction(tr("Informations"));
    menu.addAction(tr("Options"));

    QAction *action = menu.exec(QCursor::pos());
    if(action == 0)
        return;
}

void MainWindow::nodeDoubleClick(QGVNode *node)
{
    QMessageBox::information(this, tr("Node double clicked"), tr("Node %1").arg(node->label()));
}

// Simple load from file function.
// TODO Integrate with graph viz algo code.
void MainWindow::loadFromFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Edge List"), "", tr("Edge List (*.abk);;All Files (*)"));
    std::string utf8_text = fileName.toUtf8().constData();
    std::vector<std::string> args = {"-i", utf8_text, "-o", "/home/kester/test.bin"};
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

    // now it is the dnppr algorithm
    std::vector<std::string> dnppr_args = {"approx_dnppr", "-f", "6", "-alg", "fpsn", "-build", "0"}; // for now we only support taupush
    std::vector<char*> dnppr_cstrings;
    dnppr_cstrings.reserve(dnppr_args.size());
    for (size_t i = 0; i < dnppr_args.size(); i++)
        dnppr_cstrings.push_back(const_cast<char*>(dnppr_args[i].c_str()));
    // invoke method
    std::vector<std::vector<double>> coordinates = dnppr(dnppr_args.size(), &dnppr_cstrings[0]);

    // plot graph
    for (int i = 0; i < coordinates.size(); i++) {
        GraphicNode* node = new GraphicNode(coordinates[i][0], coordinates[i][1], coordinates[i][2]);
        scene->addItem(node);
    }
    // std::system("python3 /home/kester/MultiGraphViz/load-superppr-viz.py --supernode=c0_l2_838");
    std::string graph_file = "/home/kester/c0_l2_838-edge-list.txt";
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
        std::cerr << t1;
        std:: cerr << " ";
        std::cerr << t2;
        std:: cerr<< " ";
        std::cerr << x;
        std::cerr << " ";
        std::cerr << y;
        std::cerr << "\n";

        QGraphicsLineItem* line = new QGraphicsLineItem();
        QPen _Pen;
        _Pen.setColor(Qt::black);
        _Pen.setWidth(0.5);
        line->setLine(x + radius, y + radius, x2 + radius, y2 + radius);
        line->setPen(_Pen);
        scene->addItem(line);
    }

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

