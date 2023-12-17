#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <limits>
#include <stack>
#include <functional>
#include <string>

using namespace std;

const int INF = numeric_limits<int>::max();

// Node Structure.
struct Node {
    int id;
    vector<pair<int, int>> neighbors;

    Node(int _id) : id(_id) {}

    // Add Edge.
    void addEdge(int weight, int neighborId) {
        neighbors.push_back({weight, neighborId});
        return;
    }

    // Delete Specific Edge.
    void deleteEdge(int neighborId) {
        auto it = find_if(neighbors.begin(), neighbors.end(),
                          [neighborId](const pair<int, int>& edge) {
                              return edge.second == neighborId;
                          });

        if (it != neighbors.end()) {
            neighbors.erase(it);
        }
        return;
    }
};

// util data structure for loading from file.
struct linkedListNode{
    int u;
    int v;
    int weight;
    linkedListNode* prev;
    linkedListNode* next;
};

// util data structure for loading messages.
struct MessagelinkedListNode{
    int src;
    int dest;
    string message;
    MessagelinkedListNode* prev;
    MessagelinkedListNode* next;
};

// Class Graph
class Graph {
public:
    vector<Node> nodes;
    vector<int> distances;
    vector<int> predecessors;
    linkedListNode* topoChangeInfo;
    MessagelinkedListNode* msgInfo;

    // add & Delete undirected edge in graph
    void addUndirectedEdge(int u, int v, int weight) {
        nodes[u].addEdge(weight, v);
        nodes[v].addEdge(weight, u);
        return;
    }

    void deleteUndirectedEdge(int u, int v){
        nodes[u].deleteEdge(v);
        nodes[v].deleteEdge(u);
        return;
    }

    // Dijkstra Algorithm
    void dijkstra(int start) {
        int n = nodes.size();
        distances.assign(n, INF);
        predecessors.assign(n, -1);
        distances[start] = 0;

        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

        pq.push({0, start});

        while (!pq.empty()) {
            int u = pq.top().second;
            int dist_u = pq.top().first;
            pq.pop();

            if (dist_u > distances[u]) {
                continue;
            }

            for (const pair<int, int>& neighbor : nodes[u].neighbors) {
                int v = neighbor.second;
                int weight_uv = neighbor.first;

                if (distances[u] + weight_uv < distances[v]) {
                    distances[v] = distances[u] + weight_uv;
                    predecessors[v] = u;
                    pq.push({distances[v], v});
                }
            }
        }
        return;
    }

    // Print out the shortest path
    void printShortestPaths(int start) {
        int n = distances.size();

        for (int i = 1; i < n; ++i) {
            cout << "Shortest distance from node " << start << " to node " << i << " is: ";
            if (distances[i] == INF) {
                cout << "INFINITY" << endl;
            } else {
                cout << distances[i] << " and the path is: ";
                stack<int> path;
                int current = i;
                while (current != -1) {
                    path.push(current);
                    current = predecessors[current];
                }
                while (!path.empty()) {
                    cout << path.top() << " ";
                    path.pop();
                }
                cout << endl;
            }
        }
        return;
    }

    // Load Graph edge and vertices from file.
    void loadGraphFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Could not open file " << filename << endl;
            return;
        }

        int u, v, weight;
        linkedListNode* lastNode;
        linkedListNode* prevNode = nullptr;
        int maxIdx = 0;
        // construct the linked list to cache the data.
        while (file >> u >> v >> weight) {
            if (u > maxIdx){
                maxIdx = u;
            }
            if (v > maxIdx){
                maxIdx = v;
            }
            lastNode = new linkedListNode;
            // handle initial condition.
            if (prevNode != nullptr) {
                prevNode->next = lastNode;
                lastNode->prev = prevNode;
            } else {
                lastNode->prev = nullptr;
            }
            lastNode->u = u;
            lastNode->v = v;
            lastNode->weight = weight;
            lastNode->next = nullptr;
            prevNode = lastNode;
        }

        file.close();

        for (int i = 0; i <= maxIdx; i++){
            nodes.push_back(Node(i));
        }

        linkedListNode* currentNode = lastNode;
        while (currentNode != nullptr){
            addUndirectedEdge(currentNode->u, currentNode->v, currentNode->weight);
            currentNode = currentNode->prev;
            if (currentNode != nullptr){
                delete currentNode->next;
            }
        }

        return;
    }

    void loadChangeFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Could not open file " << filename << endl;
            return;
        }

        int u, v, weight;
        linkedListNode* lastNode;
        linkedListNode* prevNode = nullptr;
        // construct the linked list to cache the data.
        while (file >> u >> v >> weight) {
            lastNode = new linkedListNode;
            // handle initial condition.
            if (prevNode != nullptr) {
                prevNode->next = lastNode;
                lastNode->prev = prevNode;
            } else {
                lastNode->prev = nullptr;
                topoChangeInfo = lastNode;
            }
            lastNode->u = u;
            lastNode->v = v;
            lastNode->weight = weight;
            lastNode->next = nullptr;
            prevNode = lastNode;
        }

        file.close();
        return;
    }

    void loadMessageFromFile(const string& filename){
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Could not open file " << filename << endl;
            return;
        }

        int source, destination;
        MessagelinkedListNode* lastNode;
        MessagelinkedListNode* prevNode = nullptr;
        string message;
        while (file >> source >> destination) {
            getline(file, message);
            // remove the blanks ahead of string.
            message = message.substr(1);
            lastNode = new MessagelinkedListNode;
            // handle initial condition
            if (prevNode != nullptr) {
                prevNode->next = lastNode;
                lastNode->prev = prevNode;
            } else {
                lastNode->prev = nullptr;
                msgInfo = lastNode;
            }
            lastNode->src = source;
            lastNode->dest = destination;
            lastNode->message = message;
            lastNode->next = nullptr;
            prevNode = lastNode;
        }

        file.close();
        return;
    }

    void initGraph(const string& topofile, const string& changesfile, const string& messagefile){
        loadGraphFromFile(topofile);
        loadChangeFromFile(changesfile);
        loadMessageFromFile(messagefile);
        return;
    }

    void printForwardingTable(int src, ofstream& outFile) {
        dijkstra(src);
        int n = distances.size();
        for (int i = 1; i < n; i ++) {
            if (i == src){
                cout << i << " " << i << " " << "0" << endl;
                outFile << i << " " << i << " " << "0" << endl;
            } else {
                if (distances[i] == INF) {
                    continue;
                } else {
                    cout << i << " ";
                    outFile << i << " ";
                    int current = i;
                    while (predecessors[current] != src) {
                        current = predecessors[current];
                    }
                    cout << current << " " << distances[i] << endl;
                    outFile << current << " " << distances[i] << endl;
                }
            }
        }
        return;
    }

    void printMsgTrans(ofstream& outFile){
        MessagelinkedListNode* currentMsgInfo = msgInfo;
        while (currentMsgInfo != nullptr){
            dijkstra(currentMsgInfo->src);
            if (distances[currentMsgInfo->dest] == INF) {
                cout << "from " << currentMsgInfo->src << " to " << currentMsgInfo->dest << " cost infinite hops unreachable message " << currentMsgInfo->message << endl;
                outFile << "from " << currentMsgInfo->src << " to " << currentMsgInfo->dest << " cost infinite hops unreachable message " << currentMsgInfo->message << endl;
            } else {
                cout << "from " << currentMsgInfo->src << " to " << currentMsgInfo->dest << " cost " << distances[currentMsgInfo->dest] << " hops ";
                outFile << "from " << currentMsgInfo->src << " to " << currentMsgInfo->dest << " cost " << distances[currentMsgInfo->dest] << " hops ";
                stack<int> path;
                int current = currentMsgInfo->dest;
                while (current != -1) {
                    path.push(current);
                    current = predecessors[current];
                }
                while (!path.empty()) {
                    cout << path.top() << " ";
                    outFile << path.top() << " ";
                    path.pop();
                    if (path.size() == 1){
                        path.pop();
                    }
                }
                cout << "message " << currentMsgInfo->message << endl;
                outFile << "message " << currentMsgInfo->message << endl;
            }
            currentMsgInfo = currentMsgInfo->next;
        }
        return;
    }

    void printTopoChange(ofstream& outFile){
        for (int i = 1; i < nodes.size(); i++) {
            printForwardingTable(i, outFile);
        }
        printMsgTrans(outFile);
        linkedListNode* currentTopoChange = topoChangeInfo;
        while (currentTopoChange != nullptr) {
            if (currentTopoChange->weight == -999){
                deleteUndirectedEdge(currentTopoChange->u, currentTopoChange->v);
            } else {
                deleteUndirectedEdge(currentTopoChange->u, currentTopoChange->v);
                addUndirectedEdge(currentTopoChange->u, currentTopoChange->v, currentTopoChange->weight);
            }
            for (int i = 1; i < nodes.size(); i++) {
                printForwardingTable(i, outFile);
            }
            printMsgTrans(outFile);
            currentTopoChange = currentTopoChange->next;
        }
        return;
    }
};

int main(int argc, char** argv) {
    //printf("Number of arguments: %d", argc);
    if (argc != 4) {
        printf("Usage: ./linkstate topofile messagefile changesfile\n");
        return -1;
    }

    // output file.
    std::string filename = "output.txt";
    std::ofstream outFile(filename, std::ios::out | std::ios::trunc);

    if (!outFile.is_open()) {
        outFile.open(filename, std::ios::out);
        
        if (!outFile.is_open()) {
            std::cerr << "Error: Could not open or create file for writing." << std::endl;
            return 1;
        }
    }

    // Establish Graph and add edges.
    string topofile(argv[1]);
    string changesfile(argv[3]);
    string messagefile(argv[2]);

    Graph graph;
    graph.initGraph(topofile, changesfile, messagefile);
    graph.printTopoChange(outFile);
    outFile.close();

    return 0;
}