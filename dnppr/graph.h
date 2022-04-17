#ifndef GRAPH_H
#define GRAPH_H
#include "lib.h"
#include <set>
using namespace std;

bool sortdesc(const pair<double,int> &a,
              const pair<double,int> &b)
{
    return (a.first > b.first);
}

class Graph {
public:

    vector<vector<int>> g;
    vector<int> deg;
    string data_folder;
    vector<int> nodes;
    int n;
    long long m;
    bool isOnline;
    int max_level;
    double dbar;
    double alpha;
    double delta;
    double pfail;
    double epR;
    double tau;
    int k;
    // sum square of degree
    Graph() = default;
    Graph(string &datapath, double a,int cluster_sze) {
        this->data_folder = datapath;
        init_graph();
        dbar = double(2*m) / double(n);
        alpha = a;
        pfail = 1/(double)n;
        double delta_scale =250.0;
        delta = 1.0/delta_scale/log(n);
        k=cluster_sze;
        tau = 1.0/pow(k*n,0.5);
        isOnline = false;
        max_level=-1;
        epR = 0.5;
        if (verboses)
            cout << "init graph n: " << this->n << " m: " << this->m << endl;
    }

    void init_nm() {
        // string attribute_file = data_folder + FILECONNECT + "attribute.txt";
        /*
        string attribute_file = "/home/kester/youtube_attribute.txt"; // attribute file?
        ifstream attr(attribute_file);
        string line1, line2;
        char c;
        while (true) {
            attr >> c;
            if (c == '=') break;
        }
        attr >> n;
        while (true) {
            attr >> c;
            if (c == '=') break;
        }
        attr >> m;*/

        // string graph_file = "/home/kester/youtube.txt"; // the original file
        std::cerr << this->data_folder;
        FILE *fin = fopen(this->data_folder.c_str(), "r");
        int t1, t2;
        set<int> nodes;
        int edges = 0;
        while (fscanf(fin, "%d%d", &t1, &t2) != EOF) {
            if(t1 == t2) continue;
            edges++;
            nodes.insert(t1);
            nodes.insert(t2);
        }

        std::cerr << nodes.size() << std::endl;
        std::cerr << edges << std::endl;

        n = nodes.size();
        m = edges;
    }

    void init_graph() {
        init_nm();
        nodes.resize(n);
        deg.resize(n);
        iota (begin(nodes), end(nodes), 0);
        g = vector<vector<int>>(n, vector<int>());
        // string graph_file = "/home/kester/youtube.txt"; // the original file
        std::cerr << this->data_folder;
        FILE *fin = fopen(this->data_folder.c_str(), "r");
        int t1, t2;
        while (fscanf(fin, "%d%d", &t1, &t2) != EOF) {
            assert(t1 < n);
            assert(t2 < n);
            if(t1 == t2) continue;
            g[t1].push_back(t2);
            g[t2].push_back(t1);
        }

        for (int i = 0; i < n; ++i) {
            int d = g[i].size();
            deg[i] = d;
        }
    }


    double get_avg_degree() const {
        return dbar;
    }


};

#endif // GRAPH_H
