#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "graph.hpp"
#include "main.h"

using namespace std;

static const string title = "Traffic Inquiry System";
static const string menu = "Feature";
static const vector<string> options = {
        "Map Information",
        "Map Node Info",
        "Map Edge Info",
        "Map Matrix Info",
        "All Edge Between Two Nodes",
        "Shortest Edge Between Two Nodes",
        "Add Node",
        "Set Node",
        "Delete Node",
        "Add Edge",
        "Set Edge",
        "Delete Edge",
        "Load Map",
        "Save Map",
        "Reset Map",
        "Quit System",
};

static Graph g;

int main() {
    //    test();
    SelectMenu();
}

void test() {
    g.AddNode("1");
    g.AddNode("2");
    g.AddNode("3");
    g.AddNode("4");
    g.AddNode("5");
    g.AddNode("6");

    g.AddBiEdge("1", "2", 7);
    g.AddBiEdge("1", "3", 9);
    g.AddBiEdge("1", "6", 14);
    g.AddBiEdge("2", "3", 10);
    g.AddBiEdge("2", "4", 15);
    g.AddBiEdge("3", "4", 11);
    g.AddBiEdge("3", "6", 2);
    g.AddBiEdge("4", "5", 6);
    g.AddBiEdge("5", "6", 9);

    //    NodeInfo();
    //    EdgeInfo();
    //    MatrixInfo();
    //    cout << graph.dijkstra(graph.GetNode("1"), graph.GetNode("5"));
}

void PrintMenu() {
    const unsigned long long padding = 4;
    const unsigned long long maxOptionWidth = []() {
        unsigned long long m = 0;
        for (auto &s: options) m = max(m, s.length());
        return m;
    }();
    const unsigned long long maxItemWidth = to_string(options.size()).size();

    const unsigned long long width = max(maxOptionWidth, title.length()) + maxItemWidth + padding * 2 + 2;

    cout << string(width, '=') << endl
         << setw((int) (width - title.length()) / 2) << setfill(' ') << ""
         << "\033[31m" << title << "\033[0m" << endl
         << setw((int) (width - menu.length()) / 2) << setfill(' ') << ""
         << menu << endl
         << string(width, '-') << endl;


    for (unsigned long long i = 0; i < options.size(); i++)
        cout << setw(padding) << setfill(' ') << ""
             << setw((int) maxItemWidth) << setfill(' ') << i << ": "
             << options[i] << endl;
    cout << string(width, '=') << endl;
}
void SelectMenu() {
    unsigned long long opt = options.size() + 1;
    do {
        system("cls");
        PrintMenu();

        while (g.node.empty() || g.edge.empty())
            ValidMap();

        cout << "Please select a option: ";
        cin >> opt;
        if (opt > options.size()) {
            //            cout << "Invalid option, please try again: ";
            continue;
        } else if (opt == options.size() - 1)
            break;

        cout << endl;
        switch (opt) {
            case 0:
                MapInfo();
                break;
            case 1:
                NodeInfo();
                break;
            case 2:
                EdgeInfo();
                break;
            case 3:
                MatrixInfo();
                break;
            case 4:
                GetAllPath();
                break;
            case 5:
                GetShortestPath();
                break;
            case 6:
                AddNode();
                break;
            case 7:
                SetNode();
                break;
            case 8:
                DeleteNode();
                break;
            case 9:
                AddEdge();
                break;
            case 10:
                SetEdge();
                break;
            case 11:
                DeleteEdge();
                break;
            case 12:
                LoadMap();
                break;
            case 13:
                SaveMap();
                break;
            case 14:
                RenewMap();
                break;
            default:
                break;
        }
        cout << endl;
        system("pause");
    } while (true);
}

void ValidMap() {
    if (g.node.empty()) {
        cout << "No map data, Renew or Load? (R/L): ";

        char opt;
        cin >> opt;
        switch (opt) {
            case 'R':
            case 'r':
                RenewMap();
                break;
            case 'L':
            case 'l':
                LoadMap();
                break;
            default:
                break;
        }
    }
}
void MapInfo() {
    NodeInfo();
    EdgeInfo();
    MatrixInfo();
}
void NodeInfo() {
    cout << "Map Node info:" << endl;
    for (auto &n: g.node)
        cout << n.name << " ";
}
void EdgeInfo() {
    cout << endl
         << "Map Edge info:" << endl;
    for (auto &e: g.edge)
        cout << e.from->name << " -> " << e.to->name << " : " << e.distance
             << endl;
}
void MatrixInfo() {
    // max matrix value
    unsigned long long maxDistance = 0;
    for (auto &n: g.matrix)
        for (auto &m: n)
            if (m > maxDistance)
                maxDistance = m;

    // matrix width
    unsigned long long matrixWidth = to_string(maxDistance).length();

    cout << endl
         << "Map Matrix info:" << endl;
    for (auto &n: g.matrix) {
        for (auto &m: n)
            cout << setw((int) matrixWidth) << setfill(' ') << m << " ";
        cout << endl;
    }
}
void GetAllPath() {
    NodeInfo();
    string from, to;
    cout << "Where do you want to start?" << endl;
    cin >> from;
    cout << "Where do you want to go?" << endl;
    cin >> to;

    auto result = g.GetAllPath(from, to);

    vector<pair<int, vector<Edge>>> PathDistance;

    // calc every path distance to sort
    for (auto &path: result) {
        int distance = 0;
        for (auto &edge: path)
            distance += edge.distance;
        PathDistance.emplace_back(distance, path);
    }

    // sort to cout
    sort(PathDistance.begin(), PathDistance.end(), [](const pair<int, vector<Edge>> &a, const pair<int, vector<Edge>> &b) {
        return a.first < b.first;
    });
    for (unsigned long long i = 0; i < PathDistance.size(); i++) {
        cout << "Path " << i + 1 << ": ";
        for (auto &edge: PathDistance[i].second)
            cout << edge.from->name << " -> ";
        cout << PathDistance[i].second.back().to->name
             << " Distance: " << PathDistance[i].first << endl;
    }
}
void GetShortestPath() {
    string from, to;
    cout << "Where do you want to start: ";
    cin >> from;
    cout << "Where do you want to go: ";
    cin >> to;

    cout << "What algorithm do you want to use?" << endl;
    const vector<string> algorithm = {
            "DFS",
            "BFS",
            "Dijkstra",
            "Floyd",
            "A*",
            "Greedy",
            "IDA*",
            "IDDFS",
            "Bellman-Ford",
            "SPFA",
            "Johnson",
    };

    for (unsigned long long i = 0; i < algorithm.size(); i++) {
        // right align
        cout << setw(2) << setfill(' ') << i << ". "
             << algorithm[i] << endl;
    }

    unsigned long long opt = algorithm.size() + 1;
    while (cin >> opt, opt > algorithm.size() - 1)
        ;
    vector<Edge> result;
    switch (opt) {
        case 0:
            result = g.SearchPathDFS(from, to);
            break;
        case 1:
            result = g.SearchPathBFS(from, to);
            break;
        case 2:
            result = g.SearchPathDijkstra(from, to);
            break;
        case 3:
            result = g.SearchPathFloyd(from, to);
            break;
        case 4:
            result = g.SearchPathAStar(from, to);
            break;
        case 5:
            result = g.SearchPathGreedy(from, to);
            break;
        case 6:
            result = g.SearchPathIDAStar(from, to);
            break;
        case 7:
            result = g.SearchPathIDDFS(from, to);
            break;
        case 8:
            result = g.SearchPathBellmanFord(from, to);
            break;
        case 9:
            result = g.SearchPathSPFA(from, to);
            break;
        case 10:
            result = g.SearchPathJohnson(from, to);
            break;
        default:
            break;
    }

    // print result
    int distance = 0;
    for (unsigned long long i = 0; i < result.size(); i++) {
        auto edge = result[i];
        cout << edge.from->name << " -> ";
        distance += edge.distance;
        if (i == result.size() - 1)
            cout << edge.to->name << " : " << distance << endl;
    }
}
void AddNode() {
    cout << "Please input the name of the node: ";
    string name;
    cin >> name;
    g.AddNode(name);
}
void AddEdge() {
    cout << "Add Edge From Node: ";
    string from;
    cin >> from;
    cout << "Add Edge To Node: ";
    string to;
    cin >> to;
    cout << "Add Edge Distance: ";
    int distance;
    cin >> distance;
    g.AddBiEdge(from, to, distance);
}
void SetNode() {
    string Name, newName;
    cout << "Please input the name of the node: ";
    cin.ignore();
    getline(cin, Name);
    cout << "Please input the new name of the node: ";
    cin.ignore();
    getline(cin, Name);
    g.SetNode(Name, newName);
}
void SetEdge() {
    cout << "Set Edge From Node: ";
    string from;
    cin >> from;
    cout << "Set Edge To Node: ";
    string to;
    cin >> to;

    cout << "Set Edge Distance: ";
    int distance;
    cin >> distance;

    g.SetEdge(from, to, distance);
}
void DeleteNode() {
    cout << "Please input the name of the node: ";
    string name;
    cin >> name;
    g.DeleteNode(name);
}
void DeleteEdge() {
    cout << "Delete Edge From Node: ";
    string from;
    cin >> from;
    cout << "Delete Edge To Node: ";
    string to;
    cin >> to;
    g.DeleteEdge(from, to);
}
void LoadMap() {
    cout << "Where do you want to load the map (Default to Enter): " << endl;
    cin.ignore();
    string Edge;
    getline(cin, Edge);

    if (Edge.empty()) Edge = "map.txt";

    fstream file(Edge, ios::in);
    if (!file) {
        cout << "Error: Cannot open file." << endl;
        return;
    } else {
        g.node.clear();
        g.edge.clear();

        int nodeSize;
        file >> nodeSize;
        for (int i = 0; i < nodeSize; i++) {
            string name;
            file >> name;
            g.AddNode(name);
        }

        int edgeSize;
        file >> edgeSize;
        for (int i = 0; i < edgeSize; i++) {
            string from, to;
            int distance;
            file.ignore();
            getline(file, from, '|');
            getline(file, to, '|');
            file >> distance;
            g.AddEdge(from, to, distance);
        }

        file.close();
        cout << "Map loaded from " << Edge << endl;
    }
}
void SaveMap() {
    cout << "Where do you want to save the map (Default to Enter): " << endl;
    cin.ignore();
    string Edge;
    getline(cin, Edge);

    if (Edge.empty()) Edge = "map.txt";

    fstream file(Edge, ios::out);
    if (!file) {
        cout << "Error: Cannot open file." << endl;
        return;
    } else {
        file << g.node.size() << endl;
        for (auto &i: g.node) file << i.name << endl;
        file << g.edge.size() << endl;
        for (auto &i: g.edge) file << i.from->name << "|" << i.to->name << "|" << i.distance << endl;
        file.close();

        cout << "Map saved to " << Edge << endl;
    }
}
void RenewMap() {
    cout << "Map data cleared." << endl;
    g.node.clear();
    g.edge.clear();
    g.matrix.clear();

    int nodeNum, edgeNum;
    cout << "Please input the number of nodes: ";
    cin >> nodeNum;
    for (int i = 0; i < nodeNum; i++) AddNode();
    cout << "Please input the number of edges: ";
    cin >> edgeNum;
    for (int i = 0; i < edgeNum; i++) AddEdge();

    g.InitMatrix();
}