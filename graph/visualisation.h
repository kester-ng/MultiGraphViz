#ifndef VISUALISATION_H
#define VISUALISATION_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

// class to cache past visualisation
// since embedding into position matrix is not deterministic
class Visualisation
{
public:
    vector<vector<double>> coordinates;
    vector<string> node_names;
    vector<vector<int>> edgelist;
    Visualisation(vector<vector<double>> matrix, vector<string> names, vector<vector<int>> edges);
};

#endif // VISUALISATION_H
