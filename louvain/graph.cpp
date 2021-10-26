#include "graph.h"

using namespace std;

Graph::Graph(char *filename, int type) {
  ifstream finput;
  finput.open(filename,fstream::in);

  int nb_links=0;

  while (!finput.eof()) {
    int src, dest;
    double weight=1.;

    if (type==WEIGHTED) {
      finput >> src >> dest >> weight;
    } else {
      finput >> src >> dest;
    }

    if (finput) {
      if (links.size()<=max(src,dest)+1) {
        links.resize(max(src,dest)+1);
      }

      links[src].push_back(make_pair(dest,weight));
      if (src!=dest)
        links[dest].push_back(make_pair(src,weight));

      nb_links++;
    }
  }

  finput.close();
}
