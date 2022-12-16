#pragma once
#include <iostream>
#include <iomanip>

class Graph
{

public:
    int** matrix;
    int size = 0;

    Graph(int size);

    int get_size()
    {
        return size;
    }

    void add_vertex();
    void print_matrix();
    void print_tree(int vertex, bool* array, int length = 100, int depth = 0);
    void add_edge(int src, int dst, int length);

    int find_center(int*& array);
    Graph kruskal();
};