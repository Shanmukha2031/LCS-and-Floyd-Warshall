#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <limits>
#include <iterator>

const int MAX_DISTANCE = std::numeric_limits<int>::max();

// Clears the contents of the output file.
void ClearOutputFile() {
    std::ofstream outputFile("output.txt", std::ofstream::out | std::ofstream::trunc);
    outputFile.close();
}

// Recursively prints the path from node q to node r using the predecessor matrix.
void PrintPath(const std::vector<std::vector<int>>& predecessorMatrix, int source, int destination, std::vector<int>& path, std::ofstream& outputFile) {
    if (predecessorMatrix[source][destination] != 0) {
        PrintPath(predecessorMatrix, source, predecessorMatrix[source][destination], path, outputFile);
        outputFile << "V" << predecessorMatrix[source][destination] << " ";
        path.push_back(predecessorMatrix[source][destination]);
        PrintPath(predecessorMatrix, predecessorMatrix[source][destination], destination, path, outputFile);
    }
}

// Prints the total distance covered by the path.
void PrintTotalDistance(const std::vector<int>& path, const std::vector<std::vector<int>>& distanceMatrix, std::ofstream& outputFile) {
    int totalLength = 0;
    for (size_t i = 1; i < path.size(); i++) {
        totalLength += distanceMatrix[path[i - 1]][path[i]];
    }
    outputFile << " " << totalLength;
}

// Prints the predecessor matrix.
void PrintPredecessorMatrix(const std::vector<std::vector<int>>& predecessorMatrix, int vertexCount, std::ofstream& outputFile) {
    outputFile << "PMatrix:\n";
    for (int i = 1; i <= vertexCount; ++i) {
        for (int j = 1; j <= vertexCount; ++j) {
            outputFile << predecessorMatrix[i][j] << " ";
        }
        outputFile << std::endl;
    }
    outputFile << std::endl;
}

// Prints all pairs shortest paths and their distances.
void PrintAllPairsShortestPaths(const std::vector<std::vector<int>>& distanceMatrix, const std::vector<std::vector<int>>& predecessorMatrix, int vertexCount, std::ofstream& outputFile) {
    for (int i = 1; i <= vertexCount; i++) {
        outputFile << "V" << i << "-Vj:shortest path and length\n"; // Print header for each vertex
        for (int j = 1; j <= vertexCount; j++) {
            std::vector<int> path;
            outputFile << "V" << i << " ";
            path.push_back(i);
            PrintPath(predecessorMatrix, i, j, path, outputFile);
            outputFile << "V" << j << ":";
            path.push_back(j);
            PrintTotalDistance(path, distanceMatrix, outputFile);
            outputFile << std::endl;
        }
        outputFile << std::endl;
    }
}


// Implements the Floyd-Warshall algorithm to find the shortest paths between all pairs of vertices.
void FloydWarshallAlgorithm(std::vector<std::vector<int>>& distanceMatrix, std::vector<std::vector<int>>& predecessorMatrix, int vertexCount, std::ofstream& outputFile) {
    for (int k = 1; k <= vertexCount; k++) {
        for (int i = 1; i <= vertexCount; i++) {
            for (int j = 1; j <= vertexCount; j++) {
                if (distanceMatrix[i][k] < MAX_DISTANCE && distanceMatrix[k][j] < MAX_DISTANCE && distanceMatrix[i][j] > distanceMatrix[i][k] + distanceMatrix[k][j]) {
                    distanceMatrix[i][j] = distanceMatrix[i][k] + distanceMatrix[k][j];
                    predecessorMatrix[i][j] = k;
                }
            }
        }
    }
    PrintPredecessorMatrix(predecessorMatrix, vertexCount, outputFile);
    PrintAllPairsShortestPaths(distanceMatrix, predecessorMatrix, vertexCount, outputFile);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Invalid number of arguments. Usage: <program> <graph-file>" << std::endl;
        return 1;
    }

    std::string graphFilePath = argv[1];
    ClearOutputFile();

    std::ifstream graphFile(graphFilePath);
    if (!graphFile) {
        std::cerr << "Error: Unable to open file " << argv[1] << std::endl;
        return 1;
    }

    std::ofstream outputFile("output.txt", std::ios::app);
    if (!outputFile) {
        std::cerr << "Error: Unable to create/open output.txt" << std::endl;
        return 1;
    }

    int problemCount = 0;
    std::string line;

    while (getline(graphFile, line)) {
        std::istringstream lineStream(line);
        if (lineStream >> std::ws && lineStream.peek() == EOF) {
            continue; // Stop reading if the line is empty and we have reached EOF
        }

        if (!line.empty()) {
            problemCount++;
            std::vector<std::string> tokens((std::istream_iterator<std::string>(lineStream)), std::istream_iterator<std::string>());
            int vertices = std::stoi(tokens.back());
            std::vector<std::vector<int>> graph(vertices + 1, std::vector<int>(vertices + 1, MAX_DISTANCE));
            std::vector<std::vector<int>> predecessorMatrix(vertices + 1, std::vector<int>(vertices + 1, 0));

            for (int i = 1; i <= vertices; i++) {
                getline(graphFile, line);
                std::istringstream rowStream(line);
                for (int j = 1; j <= vertices; j++) {
                    rowStream >> graph[i][j];
                    if (graph[i][j] == 0 && i != j) {
                        graph[i][j] = MAX_DISTANCE; // Assuming 0 means no path.
                    }
                }
            }
            outputFile << "Problem " << problemCount << ": n = " << vertices << std::endl;
            FloydWarshallAlgorithm(graph, predecessorMatrix, vertices, outputFile);
        }
    }
    
    outputFile.close();
    graphFile.close();

    return 0;
}