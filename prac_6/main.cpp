#include "Graph.h"
#include <map>
#include <fstream>
int main()
{
    std::ifstream fin1;
    std::ifstream fin2;

    srand(time(NULL));

    Graph graph(2);
    Graph new_graph(2);

    int src, dst, len, quantity;
    int* array;
    bool* was;
    std::pair<int, int> pair;

    int oper_type = -1;
    while (true)
    {
        std::cout << "Choose your operation:\n1) Print matrix\n2) Add edge to graph\n3) Add vertex to graph\n4) Find graph center\n5) Print graph "
                     "tree\n6) Input graph 1\n7) Input graph 2\n0) Exit program\nYour choice is: ";
        std::cin >> oper_type;
        switch (oper_type)
        {
            case 1: graph.print_matrix(); break;
            case 2:
                std::cin >> src >> dst >> len;
                graph.add_edge(src, dst, len);
                graph.add_edge(dst, src, len);
                break;
            case 3: graph.add_vertex(); break;
            case 4:
                quantity = graph.find_center(array);
                if (quantity > 1)
                {
                    std::cout << "Center: ";
                    for (int i = 0; i < quantity; ++i)
                    {
                        if (i > 0)
                            std::cout << ", ";
                        std::cout << array[i];
                    }
                    std::cout << std::endl;
                }
                else
                    std::cout << "Center: " << array[0] << std::endl;
                break;
            case 5:
                new_graph = graph.kruskal();
                std::cout << "Tree matrix: " << std::endl;
                new_graph.print_matrix();

                was = new bool[new_graph.get_size()];
                array = new int[new_graph.get_size()];

                for (int i = 0; i < new_graph.get_size(); ++i)
                {
                    array[i] = 0;
                    was[i] = 0;
                }

                for (int i = 0; i < new_graph.get_size(); ++i)
                {
                    for (int j = 0; j < new_graph.get_size(); ++j)
                    {
                        if (new_graph.matrix[i][j] != 100)
                            array[i]++;
                    }
                }

                pair = {0, 0};

                for (int i = 0; i < new_graph.get_size(); ++i)
                {
                    if (array[i] > pair.second)
                    {
                        pair.first = i;
                        pair.second = array[i];
                    }
                }
                std::cout << "Graph tree: " << std::endl;
                new_graph.print_tree(pair.first, was);
                break;
            case 6:
                graph = Graph(5);
                fin1.open("input1.txt");
                while (!fin1.eof())
                {
                    fin1 >> src >> dst >> len;
                    graph.add_edge(src, dst, len);
                    graph.add_edge(dst, src, len);
                }
                fin1.close();
                break;
            case 7:
                graph = Graph(6);
                fin2.open("input2.txt");
                while (!fin2.eof())
                {
                    fin2 >> src >> dst >> len;
                    graph.add_edge(src, dst, len);
                    graph.add_edge(dst, src, len);
                }
                fin2.close();
                break;
            case 0: exit(0); break;
            default: std::cout << "Incorrect, try again" << std::endl;
        }
    }

    return 0;
}