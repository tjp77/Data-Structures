#ifndef GRAPHCOMPONENTS_H
#define GRAPHCOMPONENTS_H

struct Vertex
{
    double disttosource = std::numeric_limits<double>::infinity();
    int traceback = -1;
};

struct Edge 
{
    int v1, v2;
    double weight;

};


class GraphData
{
    int numbvertices, numbedges;
    bool issetup = false;

    public:

    int source = 0;
    std::list<Edge> Edges;

    void SetUp(int _numbvertices, int _numbedges, std::list<Edge> _edges)
    {
        numbvertices = _numbvertices;
        numbedges = _numbedges;
        Edges = _edges;

        issetup = true;
    }

    int NumbVertices()
    { return numbvertices; }

    int NumbEdges()
    { return numbedges; }


    bool IsSetUp()
    { return issetup; }

};

#endif // GRAPHCOMPONENTS_H
