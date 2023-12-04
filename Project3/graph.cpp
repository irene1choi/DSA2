#include "graph.h"

// Load the graph from an input file, creating vertices and edges based on file content
void graph::loadGraph(std::string infile) {
    std::string txtLine, startingV, endingV;
    int cost;
    vertex *pv;
    std::ifstream input(infile);
    
    // Read each line in the file to extract edge information
    while (std::getline(input, txtLine)) {
        std::stringstream ss(txtLine);
        // Extract starting vertex, ending vertex, and cost for each edge
        ss >> startingV >> endingV >> cost;
        edge e = edge(startingV, endingV, cost);

        // Check and add starting vertex if it hasn't been added yet
        if (!vertices.contains(startingV)) {
            pv = new vertex(startingV);
            vertices.insert(startingV, pv);
            visited.push_back(startingV);
            size++;
        } else { 
            // Retrieve existing vertex
            pv = static_cast<vertex *>(vertices.getPointer(startingV)); 
        }

        // Add the edge to the vertex's adjacency list
        pv->adj.push_back(e);

        // Check and add ending vertex if it hasn't been added yet
        if (!vertices.contains(endingV)) {
            pv = new vertex(endingV);
            vertices.insert(endingV, pv);
            visited.push_back(endingV);
            size++;
        }
    }
}

// Check if a specified vertex exists in the graph
bool graph::validVertex(std::string v) {
    return vertices.contains(v);
}

// Implementation of Dijkstra's algorithm
void graph::dijkstra(std::string start) {
    vertex *pv, *pend;
    pv = static_cast<vertex *>(vertices.getPointer(start));
    // Distance to start vertex is zero
    pv->dv = 0;
    pv->known = true;

    heap graphHeap = heap(size);

    // Initialize all vertices in the heap with their initial distances
    for (const auto &vId : visited) {
        vertex *v = static_cast<vertex *>(vertices.getPointer(vId));
        int initialDist = (vId == start) ? 0 : INT_MAX;
        graphHeap.insert(vId, initialDist);
        v->dv = initialDist;
        v->pred = nullptr;
    }

    std::string id;
    int dv;

    // Process each vertex using the heap
    while (!graphHeap.deleteMin(&id, &dv)) {
        pv = static_cast<vertex *>(vertices.getPointer(id));
        pv->known = true;

        // Update distances for adjacent vertices
        for (auto &edge : pv->adj) {
            pend = static_cast<vertex *>(vertices.getPointer(edge.endingV));
            int newDist = pv->dv + edge.cost;
            if (newDist < pend->dv) {
                pend->dv = newDist;
                pend->pred = pv;
                graphHeap.setKey(edge.endingV, newDist);
            }
        }
    }
}

// Build paths for each vertex from the start vertex
void graph::buildPaths() {
    for (const auto &vertexId : visited) {
        vertex *v = static_cast<vertex *>(vertices.getPointer(vertexId));
        std::list<std::string> path;

        // Construct the path from start to the current vertex
        for (vertex *curr = v; curr != nullptr; curr = curr->pred) {
            path.push_front(curr->id);
        }

        // Convert the path list into a string representation
        v->path = std::accumulate(std::next(path.begin()), path.end(), path.front(),
                                  [](const std::string &a, const std::string &b) { return a + ", " + b; });
    }
}

// Output the shortest paths and distances to an output file
void graph::outputPaths(std::string outputFile) {
    buildPaths(); // Ensure paths are built before outputting

    std::ofstream output(outputFile);
    // Write each vertex's shortest path information to the file
    for (const auto &vertexId : visited) {
        vertex *pv = static_cast<vertex *>(vertices.getPointer(vertexId));
        output << pv->id << ": ";
        if (pv->dv == INT_MAX) {
            // Indicate no path if distance is infinity
            output << "NO PATH";
        } else {
            // Output distance and path
            output << pv->dv << " [" << pv->path << "]";
        }
        output << std::endl;
    }
}
