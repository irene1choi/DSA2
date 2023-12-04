#include <iostream>
#include <chrono>
#include "graph.h"

int main() {
    std::string inputFile, startVertex, outputFile;
    graph myGraph;
    std::cout << "Enter name of graph file: ";
    std::cin >> inputFile;
    // Use information in input file to create a corresponding graph
    myGraph.loadGraph(inputFile);
    do {
        std::cout << "Enter name of starting vertex: ";
        std::cin >> startVertex;
    } while (!myGraph.validVertex(startVertex));
    // Start the timer
    auto startTime = std::chrono::steady_clock::now();
    // Find the shortest path from the specified start point to each other vertex
    myGraph.dijkstra(startVertex);
    // Stop timer
    auto endTime = std::chrono::steady_clock::now();
    auto timeDiff = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);
    std::cout << "Total time (in seconds) to apply Dijkstra's algorithm: " << timeDiff.count() << std::endl;
    std::cout << "Enter name of output file: ";
    std::cin >> outputFile;
    // Put shortest paths in order into output file
    myGraph.outputPaths(outputFile);
    return 0;
}
