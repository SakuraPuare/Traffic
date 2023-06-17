using namespace std;

class Node {
private:
public:
    unsigned long long id;
    string name;

    Node(unsigned long long id, const string &name) {
        this->id = id;
        this->name = name;
    }
};

class Edge {
private:
public:
    unsigned long long id;
    Node *from;
    Node *to;
    int distance;

    Edge(unsigned long long id, Node &from, Node &to, int distance) {
        this->id = id;
        this->from = &from;
        this->to = &to;
        this->distance = distance;
    }
};
class Graph {
private:
public:
    vector<Node> node;
    vector<Edge> edge;
    vector<vector<unsigned long long>> matrix{};

    void InitMatrix();
    void SetMatrix(unsigned long long i, unsigned long long j, int distance);
    unsigned long long GetMatrix(unsigned long long i, unsigned long long j);
    unsigned long long GetMatrix(const string &from, const string &to);

    Node *GetNode(const string &name);
    Node *GetNode(unsigned long long id);
    void AddNode(const string &name);
    void DeleteNode(const string &name);
    void SetNode(const string &name, const string &newName);

    Edge *GetEdge(const string &from, const string &to);
    void AddEdge(const string &from, const string &to, int distance);
    void AddBiEdge(const string &from, const string &to, int distance);
    void DeleteEdge(const string &from, const string &to);
    void SetEdge(const string &from, const string &to, int distance);

    vector<Edge> SearchPathDFS(const string &from, const string &to);
    vector<Edge> SearchPathBFS(const string &from, const string &to);
    vector<Edge> SearchPathDijkstra(const string &from, const string &to);
    vector<Edge> SearchPathFloyd(const string &from, const string &to);
    vector<Edge> SearchPathAStar(const string &from, const string &to);
    vector<Edge> SearchPathGreedy(const string &from, const string &to);
    vector<Edge> SearchPathIDAStar(const string &from, const string &to);
    vector<Edge> SearchPathIDDFS(const string &from, const string &to);
    vector<Edge> SearchPathBellmanFord(const string &from, const string &to);
    vector<Edge> SearchPathSPFA(const string &from, const string &to);
    vector<Edge> SearchPathJohnson(const string &from, const string &to);

    void GetPath(Node &from, Node &to, vector<Edge> &path, vector<vector<Edge>> &result);
    vector<vector<Edge>> GetAllPath(const string &from, const string &to);
};
