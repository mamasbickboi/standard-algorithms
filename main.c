#include "graph.h"

int main (void)
{
    Graph graph = NULLGRAPH;

    for (int i = 0; i < 7; ++i)
        if(!add_vertex(&graph, newVertex('A' + i)))
            printf("failed to add vertex\n");

    add_edge(&graph, 'A', 'B', 5, undirected);
    add_edge(&graph, 'A', 'C', 2, undirected);
    add_edge(&graph, 'A', 'D', 10, undirected);

    add_edge(&graph, 'B', 'A', 5, undirected);
    add_edge(&graph, 'B', 'E', 2, undirected);

    add_edge(&graph, 'C', 'A', 2, undirected);
    add_edge(&graph, 'C', 'D', 8, undirected);

    add_edge(&graph, 'D', 'A', 10, undirected);
    add_edge(&graph, 'D', 'C', 8, undirected);
    add_edge(&graph, 'D', 'G', 4, undirected);

    add_edge(&graph, 'E', 'B', 2, undirected);
    add_edge(&graph, 'E', 'F', 8, undirected);
    add_edge(&graph, 'E', 'G', 12, undirected);

    add_edge(&graph, 'F', 'E', 8, undirected);
    add_edge(&graph, 'F', 'G', 1, undirected);
    
    add_edge(&graph, 'G', 'D', 4, undirected);
    add_edge(&graph, 'G', 'E', 12, undirected);
    add_edge(&graph, 'G', 'F', 1, undirected);

    char src = 'A';
    i64* test = dijkstra(&graph, src);

    for (size_t i = 0; i < graph.vertexCount; ++i)
        printf("%c -> '%c' takes %d\n", src, graph.adjacenyList[i]->name, test[i]);
}