#include <fstream>
#include <sstream>
#include <list>
#include <iterator>
#include "heap.h"
#include <climits>
#include <numeric>

class graph {
public:
    // Loads the graph from a file, creating vertices and edges based on the file content
    void loadGraph(std::string infile);
    // Verifies if a given vertex id exists in the graph
    bool validVertex(std::string v);
    // Implements Dijkstra's algorithm
    void dijkstra(std::string start);
    // Constructs the paths from start vertex to all other vertices
    void buildPaths(); 
    // Generates the output file with shortest paths and distances from the start vertex
    void outputPaths(std::string outfile);

private:
    // Hash table for storing vertices
    hashTable vertices;
    // Keeps track of the order in which vertices are processed
    std::list<std::string> visited;
    // Size of the graph
    int size = 0;
    // Graph edge struct (starting/ending vertices + cost)
    struct edge {
        std::string startingV;
        std::string endingV;
        int cost;
        edge(std::string startVertex, std::string endVertex, int w) : startingV(startVertex), endingV(endVertex), cost(w) {}
    };

    // Represents a graph vertex with its ID, adjacency list, and shortest path info
    struct vertex {
        std::string id;
        std::list<edge> adj;
        bool known = false;
        int dv = INT_MAX;
        vertex *pred = nullptr;
        std::string path = "";

        // Constructor for creating a vertex with a given ID
        vertex(std::string s) : id(s) {}
    }; 
};
