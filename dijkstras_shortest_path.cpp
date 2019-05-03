#include <vector>
#include <fstream>
#include <sstream>
#include <limits>
#include <list>
#include <queue>
#include <stack>
#include <utility>

#include "GraphComponents.h"

using std::cout;
using std::endl;
using std::string;
using std::cin;
using std::pair;

// Comparator struct/operator adapted from that by jotik of Stack Overflow.
// (https://stackoverflow.com/users/3919155/jotik)
// (https://stackoverflow.com/questions/37318537/deciding-priority-in-case-of-pairint-int-inside-priority-queue)

struct CompareByPairFirst
{
    constexpr bool operator()(pair<double*, int> const& _v1, pair<double*, int> const& _v2) const noexcept
    { return *_v1.first > *_v2.first; }
};

// Tested. Loading correctly.
void LoadFromFile(GraphData& _gd)
{
    string filename;
    bool fileloaded = false;

    while(!fileloaded)
    {
        cout << "Enter the name/path of the file to load from:" << endl << endl;
        getline(cin, filename);

        string line;
        int edges, vertices;
        std::ifstream ifs(filename);

        if (ifs.good())
        {
            getline(ifs, line);
            std::istringstream iss(line);

            iss >> vertices;
            iss >> edges;

            std::list<Edge> tmpel;

            while(getline(ifs, line))
            {
                std::istringstream iss2(line);

                Edge edge;

                iss2 >> edge.v1;
                iss2 >> edge.v2;
                iss2 >> edge.weight;

                --edge.v1;
                --edge.v2;

                tmpel.push_back(edge);
            }

            _gd.SetUp(vertices, edges, tmpel);

            fileloaded = true;
        }
        else
        { cout << "File load error, could not load file of specified name." << endl << endl; }
    }


}//

int GetVertice(int _numbvertices, string _msg)
{
    cout << _msg << endl;

    int inputchoice = 0;

    while (true)
    {
        cin >> inputchoice;

        if (cin.fail()
         || inputchoice < 1
         || inputchoice > _numbvertices)
        {
            cin.clear();
            inputchoice = 0;

            cout << "Invalid vertex number. Enter a vertex number between 1 and # of vertices: "<< endl;
        }
        else
        { break; }

        cin.ignore();
    }

    return inputchoice;

}//


void PrintAllPaths(std::vector<pair<double, std::stack<int>>> _routes, int _source)
{
    for (auto itr = _routes.begin(); itr != _routes.end(); ++itr)
    {
        cout << endl;

        std::stack<int> outputQ;

        while (!(*itr).second.empty())
        {
            outputQ.push((*itr).second.top() + 1);
            (*itr).second.pop();
        }

        cout << _source + 1;

        while(!outputQ.empty())
        {
            cout << " -> " << outputQ.top();
            outputQ.pop();
        }

        cout << ", Route Cost: " << (*itr).first << endl<< endl;
    }

}//

void PrintShortestPaths(GraphData& _gd, const std::vector<Vertex>& _vertices)
{
    // Print shortest PATHS.
    for (int i = 0; i < _gd.NumbVertices(); ++i)
    {
        if (i == _gd.source)
        { continue; }

        cout << "Shortest path, v" << i + 1;

        string path = "", tbs;

        int tb = _vertices[i].traceback;

        while (tb != _gd.source
            && tb < _vertices.size()
            && _vertices[i].traceback != std::numeric_limits<double>::infinity())
        {
            std::stringstream conv;
            conv << (tb + 1);
            tbs = conv.str();
            path = tbs  + "->" + path;
            tb = _vertices[tb].traceback;
        }

        if (path != "" || _vertices[i].traceback == _gd.source)
        { cout << " from source: " << _gd.source + 1 << "->" << path << i + 1 << ", Distance: " << _vertices[i].disttosource; }
        else
        { cout << " from source: NO Path from " << _gd.source + 1 << " to " << i + 1; }

        cout << endl << endl;
    }

}//

void DSP_AllShortest(GraphData& _gd)
{
    std::vector<Vertex> vertices(_gd.NumbVertices());

    // (dist to source, v)
    std::priority_queue<pair<double, int>, std::vector<pair<double*, int>>, CompareByPairFirst> processingQ;

    _gd.source = GetVertice(_gd.NumbVertices(), "Choose a source vertex number (1-#vertices): ") - 1;

    vertices[_gd.source].disttosource = 0;
    vertices[_gd.source].traceback = _gd.source;

    for (int i = 0; i < _gd.NumbVertices(); ++i)
    {
        processingQ.push(pair<double*, int>(&vertices[i].disttosource, i));
    }

    while (!processingQ.empty())
    {
        // cv is vertice index number, not vertex id number.
        int cv = processingQ.top().second;
        processingQ.pop();

        // For each neighbor of cv, current vertice.
        for (auto itr = _gd.Edges.begin(); itr != _gd.Edges.end(); ++itr)
        {
            // Only vertices on an edge with cv are adjacent. Use those edges.
            // Edge: v1 v2 weight
            if ((*itr).v1 == cv)
            {
                if (vertices[(*itr).v2].disttosource > vertices[cv].disttosource + (*itr).weight)
                {
                    vertices[(*itr).v2].disttosource = vertices[cv].disttosource + (*itr).weight;
                    vertices[(*itr).v2].traceback = cv;
                }
            }
        }// for

    }// whl

    PrintShortestPaths(_gd, vertices);

}//


// Used as a base:
// (https://stackoverflow.com/questions/40381872/how-to-search-all-paths-between-two-nodes-in-linked-list-graph)

void Explore(const std::list<Edge>& _edges, const int& _a, const int& _b, std::vector<pair<double, std::stack<int>>>& _routes, int _numbnodes = 0)
{
    static std::stack<int> currroute;
    static std::vector<bool> visited(_numbnodes, false);
    static double routecost = 0;

    visited[_a] = true;

    for (auto itr = _edges.begin(); itr != _edges.end(); ++itr)
    {
        // Only vertices on an edge with _currnode are adjacent. Use those edges.
        if ((*itr).v1 == _a)
        {
            currroute.push((*itr).v2);
            routecost += (*itr).weight;

            if ((*itr).v2 == _b)
            {
                // Target found, end of current path, log it.
                _routes.push_back(pair<double, std::stack<int>>(routecost, currroute));
            }
            else if (!visited[(*itr).v2])
            {  Explore(_edges, (*itr).v2, _b, _routes); }

            currroute.pop();
            routecost -= (*itr).weight;
        }
    }

    visited[_a] = false;

}//

void FindAllPaths(GraphData& _gd)
{
    int a = GetVertice(_gd.NumbVertices(), "Enter vertice A (1-#vertices): ") - 1;
    int b = GetVertice(_gd.NumbVertices(), "Enter vertice B (1-#vertices): ") - 1;

    // pair( cost_of_route, nodes_of_rout)
    std::vector<pair<double, std::stack<int>>> routes;

    Explore(_gd.Edges, a, b, routes, _gd.NumbVertices());

    PrintAllPaths(routes, a);

}//



void MainMenu(int& _inputchoice)
{
    do
    {
        cout << "1) Open input file." << endl
             << "2) Enter Source for shortest paths." << endl
             << "3) Find paths from A to B." << endl
             << "4) Quit." << endl << endl;

        cin >> _inputchoice;

        if (cin.fail())
        {
            cin.clear();
            _inputchoice = 0;
        }

        cin.ignore();
    }
    while (_inputchoice < 1 || _inputchoice > 4);

}//


void OperationDispatch(int& _inputchoice, GraphData& _gd)
{
    switch(_inputchoice)
    {
        case 1:

            LoadFromFile(_gd);

        break;

        case 2:

            if (!_gd.IsSetUp())
            { cout << "No graph data loaded." << endl << endl; }
            else
            { DSP_AllShortest(_gd); }

        break;

        case 3:

            if (!_gd.IsSetUp())
            { cout << "No graph data loaded." << endl << endl; }
            else
            { FindAllPaths(_gd); }

        break;

        default:
        break;

    }

}//


int main()
{
    GraphData graphdata;

    while (true)
    {
        int inputchoice = 0;

        MainMenu(inputchoice);

        if (inputchoice == 4)
        { break; }

        OperationDispatch(inputchoice, graphdata);

    }

    return 0;

    /*

    Test graphs used:
    (based on the ones used in lectures):

    8 12
    1 4 3
    1 2 2
    1 3 8
    3 2 3
    4 7 10
    4 3 4
    5 3 3
    5 7 5
    6 4 5
    6 8 2
    7 6 4
    7 8 7

    and:

    7 17
    1 2 1
    2 5 4
    7 6 6
    3 1 5
    1 3 5
    6 7 6
    7 5 3
    5 2 4
    2 1 1
    3 4 1
    4 3 1
    4 7 2
    7 4 2
    4 2 2
    2 4 2
    6 3 3
    5 7 3


    */


}//
