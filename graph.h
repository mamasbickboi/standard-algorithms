#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef int64_t i64;

static const int edgeVertexListSize = 2;
const bool directed = true;
const bool undirected = false;
const i64 UNDEFINED = -1 * LLONG_MAX;

static struct __vertex__
{
    char name;
    struct __vertex__** adjacentVertices;
    size_t adjacentVertexCount;
    size_t graphIndex;
}__vertex__; //done to silence an annoying warning
typedef struct __vertex__ Vertex;

Vertex* newVertex (char n_name)
{
    Vertex* temp = (Vertex*)malloc(sizeof(Vertex));
    temp->name = n_name;
    temp->adjacentVertices = NULL;
    temp->adjacentVertexCount = 0;
    temp->graphIndex = 0;

    return temp;
}

typedef struct
{
    Vertex** v;
    int val;
}
Edge;

Edge* newEdge (Vertex* x, Vertex* y, int v)
{
    Edge* temp = (Edge*)malloc(sizeof(Edge));
    temp->v = (Vertex**)malloc(sizeof(Vertex*) * edgeVertexListSize);
    temp->v[0] = x;
    temp->v[1] = y;
    temp->val = v;

    return temp;
}

static struct __graph__
{
    Edge** edgeArr;
    size_t edgeCount;
    Vertex** adjacenyList;
    size_t vertexCount;
}
NULLGRAPH = {NULL, 0, NULL, 0};
typedef struct __graph__ Graph;

void printGraph (Graph* G)
{
    if (G->vertexCount == 0)
    {
        printf("empty graph\n");
        return;
    }

    for (size_t i = 0; i < G->vertexCount; ++i)
    {
        printf("%c -> ", G->adjacenyList[i]->name);
        
        for (size_t j = 0; j < G->adjacenyList[i]->adjacentVertexCount; ++j)
            printf("%c -> ", G->adjacenyList[i]->adjacentVertices[j]->name);
        printf("NULL\n");
    }
}

Vertex* haveVertex (Graph* G, char v)
{
    for (size_t i = 0; i < G->vertexCount; ++i)
        if (G->adjacenyList[i]->name == v)
            return G->adjacenyList[i];

    return NULL;
}

//add_vertex(G, x): adds the vertex x, if it is not there;
bool add_vertex (Graph* G, Vertex* x)
{
    if (haveVertex(G, x->name) != NULL)
        return false;

    G->adjacenyList = (Vertex**)realloc(G->adjacenyList, sizeof(Vertex*) * (G->vertexCount + 1));
    x->graphIndex = G->vertexCount;
    G->adjacenyList[G->vertexCount] = x;
    ++G->vertexCount;

    return true;
}

//add_edge(G, x, y, z): adds the edge z from the vertex x to the vertex y, if it is not there;
void add_edge (Graph* G, char src_name, char dest_name, int z, bool edgeType)
{
    for (size_t i = 0; i < G->edgeCount; ++i)
    {
        if (
            ((G->edgeArr[i]->v[0]->name == src_name) && (G->edgeArr[i]->v[1]->name == dest_name))
            ||
            ((G->edgeArr[i]->v[0]->name == dest_name) && (G->edgeArr[i]->v[1]->name == src_name))
        )
        {
            //printf("'%c' to '%c' edge already exists\n", src_name, dest_name);
            return;
        }
    }

    Vertex* src = haveVertex(G, src_name);
    Vertex* dest = haveVertex(G, dest_name);

    G->edgeArr = (Edge**)realloc(G->edgeArr, sizeof(Edge*) * (G->edgeCount + 1));
    G->edgeArr[G->edgeCount] = newEdge(src, dest, z);
    ++G->edgeCount;

    bool v1 = false,
         v2 = false;

    for (size_t i = 0; i < G->vertexCount; ++i)
    {
        if (G->adjacenyList[i]->name == src_name)
        {
            Vertex* itr = G->adjacenyList[i];
            itr->adjacentVertices = (Vertex**)realloc(itr->adjacentVertices,
                                                            sizeof(Vertex*) * (itr->adjacentVertexCount + 1));
            itr->adjacentVertices[itr->adjacentVertexCount] = dest;
            ++itr->adjacentVertexCount;

            v1 = true;
            if (edgeType || (v1 && v2))
                break;
        }

        if ((G->adjacenyList[i]->name == dest_name) && (!edgeType))
        {
            Vertex* itr = G->adjacenyList[i];
            itr->adjacentVertices = (Vertex**)realloc(itr->adjacentVertices,
                                                            sizeof(Vertex*) * (itr->adjacentVertexCount + 1));
            itr->adjacentVertices[itr->adjacentVertexCount] = src;
            ++itr->adjacentVertexCount;
            
            v2 = true;
            if (v1 && v2)
                break;
        }
    }
}

i64 edgeval (Graph* G, char src_name,  char dest_name)
{
    for (size_t i = 0; i < G->edgeCount; ++i)
        if (
            (G->edgeArr[i]->v[0]->name == src_name) && (G->edgeArr[i]->v[1]->name == dest_name)
            ||
            (G->edgeArr[i]->v[0]->name == dest_name) && (G->edgeArr[i]->v[1]->name == src_name)
        ) return G->edgeArr[i]->val;

    return UNDEFINED;
}

void __dfs_recursive__ (Graph* G, bool* visited, Vertex* v)
{
    visited[v->graphIndex] = true;
    printf("%c - > ", v->name);

    for (size_t i = 0; i < v->adjacentVertexCount; ++i)
        if (!visited[v->adjacentVertices[i]->graphIndex])
            __dfs_recursive__(G, visited, v->adjacentVertices[i]);
}

void dfs_recursive (Graph* G, char start)
{
    Vertex* itr = haveVertex(G, start);
    if (itr == NULL)
    {
        printf("vertex doesn't exist\n");
        return;
    }

    bool* visited;
    visited = (bool*)calloc(G->vertexCount, sizeof(bool));

    __dfs_recursive__(G, visited, itr);
    printf("NULL\n");
}

/*typedef struct
{
    i64* distances;
    Vertex** prevs;
}
dijkstraAns;*/

i64* dijkstra (Graph* G, char src_name)
{
    Vertex* src;
    if (!(src = haveVertex(G, src_name)))
        return NULL;

    Vertex** Q = (Vertex**)malloc(sizeof(Vertex*) * G->vertexCount);
    size_t Q_size = G->vertexCount;

    i64* dist = (i64*)malloc(sizeof(i64) * G->vertexCount);
    //Vertex** prev = (Vertex**)malloc(sizeof(Vertex*) * G->vertexCount);

    for (size_t i = 0; i < G->vertexCount; ++i)
    {
        dist[i] = UNDEFINED;
        //prev[i] = NULL;
        Q[i] = G->adjacenyList[i];
    }
    dist[src->graphIndex] = 0;

    while (Q_size)
    {
        i64 minIdx = 0;
        for (size_t i = 0; i < Q_size; ++i)
            if ((dist[Q[i]->graphIndex] < dist[minIdx]) && (dist[Q[i]->graphIndex] != UNDEFINED))
                minIdx = i;

        
        Vertex* u = Q[minIdx];
        Q[minIdx] = Q[Q_size - 1];
        Q = (Vertex**)realloc(Q, sizeof(Vertex*) * (Q_size - 1));
        --Q_size;

        for (size_t i = 0; i < u->adjacentVertexCount; ++i)
        {
            size_t alt = dist[u->graphIndex] + edgeval(G, u->name, u->adjacentVertices[i]->name);
            if (alt < dist[u->adjacentVertices[i]->graphIndex])
            {
                dist[u->adjacentVertices[i]->graphIndex] = alt;
                //prev[u->adjacentVertices[i]->graphIndex] = u;
            }
        }
    }

    //dijkstraAns* ans = (dijkstraAns*)malloc(sizeof(dijkstraAns));
    //ans->distances = dist;
    //ans->prevs = prev;

    return dist;
}

//edge should know their type ?
//union for size_t and i64 ???

/* TODO
A*
dfs stack
bfs queue
dfs search
bfs search
*/

/*
// wrapper macro for passing the source file name, line number, and function name to the actual wrapper function
#define xcalloc(a, b) (xcalloc)((a), (b), __FILE__, __func__, __LINE__)

static inline void *xcalloc(size_t n, size_t size, char const *fIle, char const *func, int line)
{
    void *ret = calloc(n, size);
    if (!ret) {
        fprintf(stderr, "%s:%d:%s: calloc: %s\n", file, line, func, strerror(errno));
        abort();
    }
    return ret;
}
*/