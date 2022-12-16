#include "Graph.h"

Graph::Graph(int size)
{
    matrix = new int*[size];
    for (int i = 0; i < size; ++i)
    {
        matrix[i] = new int[size];
        for (int j = 0; j < size; ++j)
        {
            matrix[i][j] = 100;
        }
    }
    this->size = size;
}

void Graph::add_vertex()
{
    int** new_matrix = new int*[size + 1];
    for (int i = 0; i < size + 1; ++i)
    {
        new_matrix[i] = new int[size + 1];
        for (int j = 0; j < size + 1; ++j)
        {
            (j == size || i == size) ? new_matrix[i][j] = 100 : new_matrix[i][j] = matrix[i][j];
        }
    }
    for (int i = 0; i < size; ++i)
    {
        delete[] matrix[i];
    }
    delete[] matrix;
    size++;
    matrix = new_matrix;
}

void Graph::print_matrix()
{
    std::cout << "    ";
    for (int i = 1; i <= size; ++i)
    {
        std::cout << std::setw(2) << i << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < size; ++i)
    {
        std::cout << std::setw(2) << i + 1 << ": ";
        for (int j = 0; j < size; ++j)
        {
            std::cout << std::setw(2) << ((matrix[i][j] == 100) ? 0 : matrix[i][j]) << " ";
        }
        std::cout << std::endl;
    }
}

void Graph::add_edge(int src, int dst, int length)
{
    src--;
    dst--;
    int tmp1 = src, tmp2 = dst;
    src = std::min(src, dst);
    dst = std::max(tmp1, tmp2);
    if (src < 0 || src > size || dst < 0 || dst > size && length <= 0)
    {
        std::cout << "Incorrect input, vertex numbers should be 1-" << size + 1 << ", length should be > 0" << std::endl;
    }
    else if (matrix[src][dst] == 100)
    {
        matrix[src][dst] = length;
        matrix[dst][src] = length;
    }
}

int Graph::find_center(int*& array)
{
    int** new_matrix = new int*[size];
    for (int i = 0; i < size; ++i)
    {
        new_matrix[i] = new int[size];
        for (int j = 0; j < size; ++j)
        {
            new_matrix[i][j] = matrix[i][j];
        }
    }
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            for (int k = 0; k < size; ++k)
            {
                new_matrix[j][k] = std::min(new_matrix[j][k], new_matrix[j][i] + new_matrix[i][k]);
            }
        }
    }
    int num = 0, max = 100;
    array = new int[size];
    for (int i = 0; i < size; ++i)
    {
        int lmax = -1;
        for (int j = 0; j < size; ++j)
        {
            lmax = std::max(lmax, new_matrix[i][j]);
        }
        if (lmax < max)
        {
            array = new int[size];
            num = 1;
            array[0] = i + 1;
            max = lmax;
        }
        else if (lmax == max)
        {
            array[num] = i + 1;
            num++;
        }
    }

    for (int i = 0; i < size; ++i)
    {
        delete[] new_matrix[i];
    }
    delete[] new_matrix;

    return num;
}

int get_parent(int x, int* parents);
void union_nodes(int x, int y, int* parents, int* ranks);

Graph Graph::kruskal()
{
    Graph new_graph(size);

    int cnt = 0;

    std::pair<int, std::pair<int, int>>* edges = new std::pair<int, std::pair<int, int>>[100];

    for (int i = 0; i < size; ++i)
    {
        for (int j = i; j < size; ++j)
        {
            if (matrix[i][j] != 100)
            {
                edges[cnt] = std::make_pair(matrix[i][j], std::make_pair(i, j));
                cnt++;
            }
        }
    }

    // bubble sort, my bad :c
    for (int i = 0; i < cnt; ++i)
    {
        for (int j = 0; j < cnt - 1; ++j)
        {
            if (edges[j].first > edges[j + 1].first)
            {
                std::swap(edges[j], edges[j + 1]);
            }
        }
    }

    int *parents = new int[size], *ranks = new int[size];

    for (int i = 0; i < size; ++i)
    {
        parents[i] = i;
        ranks[i] = 0;
    }
    for (int i = 0; i < cnt; ++i)
    {
        if (get_parent(edges[i].second.first, parents) != get_parent(edges[i].second.second, parents))
        {
            new_graph.add_edge(edges[i].second.first + 1, edges[i].second.second + 1, edges[i].first);
            union_nodes(edges[i].second.first, edges[i].second.second, parents, ranks);
        }
    }

    return new_graph;
}

int get_parent(int x, int* parents)
{
    if (parents[x] != x)
        return get_parent(parents[x], parents);
    return x;
}

void union_nodes(int x, int y, int* parents, int* ranks)
{
    x = get_parent(x, parents);
    y = get_parent(y, parents);
    if (x != y)
    {
        if (ranks[x] == ranks[y])
            ranks[x]++;
        if (ranks[x] < ranks[y])
            parents[x] = y;
        else
            parents[y] = x;
    }
}

void Graph::print_tree(int vertex, bool* array, int length, int depth)
{
    if (array[vertex])
        return;
    array[vertex] = true;
    for (int i = 0; i < depth - 1; ++i)
    {
        std::cout << "   ";
    }
    std::cout << (depth > 0 ? "|->" : "") << vertex + 1;
    if (length != 100)
        std::cout << " (length: " << length << ")";
    std::cout << std::endl;
    for (int j = 0; j < size; ++j)
    {
        if (matrix[vertex][j] != 100)
            print_tree(j, array, matrix[vertex][j], depth + 1);
    }
}