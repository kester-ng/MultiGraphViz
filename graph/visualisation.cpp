#include "visualisation.h"

Visualisation::Visualisation(vector<vector<double>> matrix, vector<string> names, vector<vector<int>> edges)
{
    // deep copy
    vector<vector<double>> matrix_copy(matrix);
    this->coordinates = matrix_copy;
    vector<string> names_copy(names);
    this->node_names = names_copy;
    vector<vector<int>> edges_copy(edges);
    this->edgelist = edges_copy;
}
