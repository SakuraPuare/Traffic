#include "graph.h"
#include <algorithm>
#include <queue>
#include <stack>

using namespace std;

void Graph::InitMatrix() {
    matrix.resize(node.size(), vector<unsigned long long>(node.size(), 0));
}
void Graph::SetMatrix(unsigned long long int i, unsigned long long int j, int distance) {
    if (i >= matrix.size() || j >= matrix[i].size())
        InitMatrix();
    matrix[i][j] = distance;
}
unsigned long long Graph::GetMatrix(unsigned long long int i, unsigned long long int j) {
    if (i >= matrix.size() || j >= matrix[i].size())
        InitMatrix();
    return matrix[i][j];
}
unsigned long long Graph::GetMatrix(const string &from, const string &to) {
    Node *f = GetNode(from);
    Node *t = GetNode(to);
    if (f == nullptr || t == nullptr)
        return 0;

    return GetMatrix(f->id, t->id);
}
Node *Graph::GetNode(const string &name) {
    for (auto &n: node) {
        if (n.name == name)
            return &n;
    }
    return nullptr;
}
Node *Graph::GetNode(unsigned long long int id) {
    if (id >= node.size())
        return nullptr;
    return &node[id];
}
void Graph::AddNode(const string &name) {
    if (GetNode(name) != nullptr)
        return;

    Node n(node.size(), name);
    node.push_back(n);
}
void Graph::DeleteNode(const string &name) {
    Node *n = GetNode(name);
    if (n == nullptr)
        return;

    // delete related edges first
    for (auto &e: edge) {
        if (e.from == n || e.to == n)
            DeleteEdge(e.from->name, e.to->name);
    }

    // delete node
    for (auto it = node.begin(); it != node.end(); it++) {
        if (it->name == name) {
            node.erase(it);
            break;
        }
    }
}
void Graph::SetNode(const string &name, const string &newName) {
    if (name == newName)
        return;
    Node *n = GetNode(name);
    if (n == nullptr)
        return;

    n->name = newName;
}
Edge *Graph::GetEdge(const string &from, const string &to) {
    Node *f = GetNode(from);
    Node *t = GetNode(to);
    if (f == nullptr || t == nullptr)
        return nullptr;

    for (auto &e: edge) {
        if (e.from == f && e.to == t)
            return &e;
    }
    return nullptr;
}
void Graph::AddEdge(const string &from, const string &to, int distance) {
    Node *f = GetNode(from);
    Node *t = GetNode(to);
    if (f == nullptr || t == nullptr)
        return;

    Edge e(edge.size(), *f, *t, distance);
    edge.push_back(e);

    SetMatrix(f->id, t->id, distance);
}
void Graph::AddBiEdge(const string &from, const string &to, int distance) {
    AddEdge(from, to, distance);
    AddEdge(to, from, distance);
}
void Graph::DeleteEdge(const string &from, const string &to) {
    Edge *e = GetEdge(from, to);
    if (e == nullptr)
        return;

    // delete edge
    for (auto it = edge.begin(); it != edge.end(); it++) {
        if (it->from == e->from && it->to == e->to) {
            SetMatrix(e->from->id, e->to->id, 0);
            edge.erase(it);
            break;
        }
    }
}
void Graph::SetEdge(const string &from, const string &to, int distance) {
    Edge *e = GetEdge(from, to);
    if (e == nullptr)
        return;

    if (distance > 0) {
        e->distance = distance;
        SetMatrix(e->from->id, e->to->id, distance);
    } else {
        DeleteEdge(from, to);
    }
}
vector<Edge> Graph::SearchPathDFS(const string &from, const string &to) {
    Node *f = GetNode(from);
    Node *t = GetNode(to);
    if (f == nullptr || t == nullptr)
        return {};

    // dfs
    vector<Edge> path;
    vector<bool> visited(node.size(), false);
    stack<Node *> s;
    s.push(f);
    visited[f->id] = true;
    while (!s.empty()) {
        Node *n = s.top();
        s.pop();
        if (n == t)
            break;
        for (auto &e: edge) {
            if (e.from == n && !visited[e.to->id]) {
                s.push(e.to);
                visited[e.to->id] = true;
                path.push_back(e);
            }
        }
    }
    // clear path
    vector<Edge> path2;
    auto dst = t;
    while (dst != f) {
        for (auto &e: path) {
            if (e.to == dst) {
                path2.push_back(e);
                dst = e.from;
                break;
            }
        }
    }
    std::reverse(path2.begin(), path2.end());
    return path2;
}
vector<Edge> Graph::SearchPathBFS(const string &from, const string &to) {
    Node *f = GetNode(from);
    Node *t = GetNode(to);
    if (f == nullptr || t == nullptr)
        return {};

    // bfs
    vector<Edge> path;
    vector<bool> visited(node.size(), false);
    queue<Node *> q;
    q.push(f);
    while (!q.empty()) {
        Node *n = q.front();
        q.pop();
        if (n == t)
            break;
        for (auto &e: edge) {
            if (e.from == n && !visited[e.to->id]) {
                q.push(e.to);
                visited[e.to->id] = true;
                path.push_back(e);
            }
        }
    }
    // clear path
    vector<Edge> path2;
    auto dst = t;
    while (dst != f) {
        for (auto &e: path) {
            if (e.to == dst) {
                path2.push_back(e);
                dst = e.from;
                break;
            }
        }
    }
    std::reverse(path2.begin(), path2.end());
    return path2;
}
vector<Edge> Graph::SearchPathDijkstra(const string &from, const string &to) {
    Node *f = GetNode(from);
    Node *t = GetNode(to);
    if (f == nullptr || t == nullptr)
        return {};

    // dijkstra
    vector<int> dist(node.size(), INT_MAX);
    vector<bool> visited(node.size(), false);
    vector<Edge> path;
    priority_queue<pair<int, Node *>, vector<pair<int, Node *>>, greater<>> pq;

    dist[f->id] = 0;
    pq.emplace(0, f);

    while (!pq.empty()) {
        Node *n = pq.top().second;
        pq.pop();

        if (visited[n->id])
            continue;

        visited[n->id] = true;

        for (auto &e: edge) {
            if (e.from == n) {
                int newDist = dist[n->id] + e.distance;
                if (newDist < dist[e.to->id]) {
                    dist[e.to->id] = newDist;
                    pq.emplace(newDist, e.to);
                    path.push_back(e);
                }
            }
        }
    }

    // clear path
    vector<Edge> path2;
    auto dst = t;
    while (dst != f) {
        for (auto &e: path) {
            if (e.to == dst) {
                path2.push_back(e);
                dst = e.from;
                break;
            }
        }
    }
    std::reverse(path2.begin(), path2.end());
    return path2;
}
vector<Edge> Graph::SearchPathFloyd(const string &from, const string &to) {
    Node *f = GetNode(from);
    Node *t = GetNode(to);
    if (f == nullptr || t == nullptr)
        return {};

    // floyd
    vector<vector<unsigned long long>> dist(node.size(), vector<unsigned long long>(node.size(), INT_MAX));
    vector<vector<unsigned long long>> path(node.size(), vector<unsigned long long>(node.size(), 0));
    for (auto &e: edge) {
        dist[e.from->id][e.to->id] = e.distance;
        path[e.from->id][e.to->id] = e.from->id;
    }
    for (unsigned long long i = 0; i < node.size(); ++i) {
        dist[i][i] = 0;
        path[i][i] = i;
    }

    for (unsigned long long k = 0; k < node.size(); ++k) {
        for (unsigned long long i = 0; i < node.size(); ++i) {
            for (unsigned long long j = 0; j < node.size(); ++j) {
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX && dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    path[i][j] = path[k][j];
                }
            }
        }
    }
    // clear path
    vector<Edge> path2;
    auto dst = t->id;
    while (dst != f->id) {
        if (path[f->id][dst] == 0)
            return {};
        path2.push_back(*GetEdge(GetNode(path[f->id][dst])->name, GetNode(dst)->name));
        dst = path[f->id][dst];
    }
    std::reverse(path2.begin(), path2.end());
    return path2;
}
vector<Edge> Graph::SearchPathAStar(const string &from, const string &to) {
    Node *f = GetNode(from);
    Node *t = GetNode(to);
    if (f == nullptr || t == nullptr)
        return {};

    // AStar
    vector<int> dist(node.size(), INT_MAX);
    vector<bool> visited(node.size(), false);
    vector<Edge> path;
    priority_queue<pair<int, Node *>, vector<pair<int, Node *>>, greater<>> pq;

    dist[f->id] = 0;
    pq.emplace(0, f);

    while (!pq.empty()) {
        Node *n = pq.top().second;
        pq.pop();

        if (visited[n->id])
            continue;

        visited[n->id] = true;

        for (auto &e: edge) {
            if (e.from == n) {
                int newDist = dist[n->id] + e.distance;
                if (newDist < dist[e.to->id]) {
                    dist[e.to->id] = newDist;
                    pq.emplace(newDist + GetMatrix(e.to->name, t->name), e.to);
                    path.push_back(e);
                }
            }
        }
    }

    // clear path
    vector<Edge> path2;
    auto dst = t;
    while (dst != f) {
        for (auto &e: path) {
            if (e.to == dst) {
                path2.push_back(e);
                dst = e.from;
                break;
            }
        }
    }
    std::reverse(path2.begin(), path2.end());
    return path2;
}
vector<Edge> Graph::SearchPathGreedy(const string &from, const string &to) {
    Node *f = GetNode(from);
    Node *t = GetNode(to);
    if (f == nullptr || t == nullptr)
        return {};

    // greedy
    vector<int> dist(node.size(), INT_MAX);
    vector<bool> visited(node.size(), false);
    vector<Edge> path;
    priority_queue<pair<int, Node *>, vector<pair<int, Node *>>, greater<>> pq;

    dist[f->id] = 0;
    pq.emplace(0, f);

    while (!pq.empty()) {
        Node *n = pq.top().second;
        pq.pop();

        if (visited[n->id])
            continue;

        visited[n->id] = true;

        for (auto &e: edge) {
            if (e.from == n) {
                int newDist = dist[n->id] + e.distance;
                if (newDist < dist[e.to->id]) {
                    dist[e.to->id] = newDist;
                    pq.emplace(GetMatrix(e.to->name, t->name), e.to);
                    path.push_back(e);
                }
            }
        }
    }

    // clear path
    vector<Edge> path2;
    auto dst = t;
    while (dst != f) {
        for (auto &e: path) {
            if (e.to == dst) {
                path2.push_back(e);
                dst = e.from;
                break;
            }
        }
    }
    std::reverse(path2.begin(), path2.end());
    return path2;
}
vector<Edge> Graph::SearchPathIDAStar(const string &from, const string &to) {
    Node *f = GetNode(from);
    Node *t = GetNode(to);
    if (f == nullptr || t == nullptr)
        return {};

    // idastar
    vector<int> dist(node.size(), INT_MAX);
    vector<bool> visited(node.size(), false);
    vector<Edge> path;
    priority_queue<pair<int, Node *>, vector<pair<int, Node *>>, greater<>> pq;

    dist[f->id] = 0;
    pq.emplace(0, f);

    while (!pq.empty()) {
        Node *n = pq.top().second;
        pq.pop();

        if (visited[n->id])
            continue;

        visited[n->id] = true;

        for (auto &e: edge) {
            if (e.from == n) {
                int newDist = dist[n->id] + e.distance;
                if (newDist < dist[e.to->id]) {
                    dist[e.to->id] = newDist;
                    pq.emplace(newDist + GetMatrix(e.to->name, t->name), e.to);
                    path.push_back(e);
                }
            }
        }
    }

    // clear path
    vector<Edge> path2;
    auto dst = t;
    while (dst != f) {
        for (auto &e: path) {
            if (e.to == dst) {
                path2.push_back(e);
                dst = e.from;
                break;
            }
        }
    }
    std::reverse(path2.begin(), path2.end());
    return path2;
}
vector<Edge> Graph::SearchPathIDDFS(const string &from, const string &to) {
    Node *f = GetNode(from);
    Node *t = GetNode(to);
    if (f == nullptr || t == nullptr)
        return {};

    // iddfs
    vector<int> dist(node.size(), INT_MAX);
    vector<bool> visited(node.size(), false);
    vector<Edge> path;
    priority_queue<pair<int, Node *>, vector<pair<int, Node *>>, greater<>> pq;

    dist[f->id] = 0;
    pq.emplace(0, f);

    while (!pq.empty()) {
        Node *n = pq.top().second;
        pq.pop();

        if (visited[n->id])
            continue;

        visited[n->id] = true;

        for (auto &e: edge) {
            if (e.from == n) {
                int newDist = dist[n->id] + e.distance;
                if (newDist < dist[e.to->id]) {
                    dist[e.to->id] = newDist;
                    pq.emplace(newDist + GetMatrix(e.to->name, t->name), e.to);
                    path.push_back(e);
                }
            }
        }
    }

    // clear path
    vector<Edge> path2;
    auto dst = t;
    while (dst != f) {
        for (auto &e: path) {
            if (e.to == dst) {
                path2.push_back(e);
                dst = e.from;
                break;
            }
        }
    }
    std::reverse(path2.begin(), path2.end());
    return path2;
}
vector<Edge> Graph::SearchPathBellmanFord(const string &from, const string &to) {
    Node *f = GetNode(from);
    Node *t = GetNode(to);
    if (f == nullptr || t == nullptr)
        return {};

    // bellman ford
    vector<int> dist(node.size(), INT_MAX);
    vector<bool> visited(node.size(), false);
    vector<Edge> path;

    dist[f->id] = 0;

    for (unsigned long long i = 0; i < node.size(); ++i) {
        for (auto &e: edge) {
            if (dist[e.from->id] != INT_MAX && dist[e.from->id] + e.distance < dist[e.to->id]) {
                dist[e.to->id] = dist[e.from->id] + e.distance;
                path.push_back(e);
            }
        }
    }

    // clear path
    vector<Edge> path2;
    auto dst = t;
    while (dst != f) {
        for (auto &e: path) {
            if (e.to == dst) {
                path2.push_back(e);
                dst = e.from;
                break;
            }
        }
    }
    std::reverse(path2.begin(), path2.end());
    return path2;
}
vector<Edge> Graph::SearchPathSPFA(const string &from, const string &to) {
    Node *f = GetNode(from);
    Node *t = GetNode(to);
    if (f == nullptr || t == nullptr)
        return {};

    // SPFA
    vector<unsigned long long> dist(node.size(), INT_MAX);
    vector<bool> visited(node.size(), false);
    vector<Edge> path;
    queue<Node *> q;

    dist[f->id] = 0;
    q.push(f);
    visited[f->id] = true;

    while (!q.empty()) {
        Node *n = q.front();
        q.pop();
        visited[n->id] = false;

        for (auto &e: edge) {
            if (e.from == n) {
                unsigned long long newDist = dist[n->id] + e.distance;
                if (newDist < dist[e.to->id]) {
                    dist[e.to->id] = newDist;
                    path.push_back(e);
                    if (!visited[e.to->id]) {
                        q.push(e.to);
                        visited[e.to->id] = true;
                    }
                }
            }
        }
    }

    // clear path
    vector<Edge> path2;
    auto dst = t;
    while (dst != f) {
        for (auto &e: path) {
            if (e.to == dst) {
                path2.push_back(e);
                dst = e.from;
                break;
            }
        }
    }
    std::reverse(path2.begin(), path2.end());
    return path2;
}
vector<Edge> Graph::SearchPathJohnson(const string &from, const string &to) {
    Node *f = GetNode(from);
    Node *t = GetNode(to);
    if (f == nullptr || t == nullptr)
        return {};

    // johnson
    vector<unsigned long long> dist(node.size(), INT_MAX);
    vector<bool> visited(node.size(), false);
    vector<Edge> path;
    priority_queue<pair<int, Node *>, vector<pair<int, Node *>>, greater<>> pq;

    dist[f->id] = 0;
    pq.emplace(0, f);

    while (!pq.empty()) {
        Node *n = pq.top().second;
        pq.pop();

        if (visited[n->id])
            continue;

        visited[n->id] = true;

        for (auto &e: edge) {
            if (e.from == n) {
                unsigned long long newDist = dist[n->id] + e.distance;
                if (newDist < dist[e.to->id]) {
                    dist[e.to->id] = newDist;
                    pq.emplace(newDist + GetMatrix(e.to->name, t->name), e.to);
                    path.push_back(e);
                }
            }
        }
    }

    // clear path
    vector<Edge> path2;
    auto dst = t;
    while (dst != f) {
        for (auto &e: path) {
            if (e.to == dst) {
                path2.push_back(e);
                dst = e.from;
                break;
            }
        }
    }
    std::reverse(path2.begin(), path2.end());
    return path2;
}
void Graph::GetPath(Node &from, Node &to, vector<Edge> &path, vector<vector<Edge>> &result) {
    // get all path from one point to another point
    if (from.id == to.id) {
        result.push_back(path);
        return;
    }

    for (auto &e: edge) {
        if (e.from == &from) {
            bool flag = true;
            for (auto &p: path) {
                if (p.from == e.from && p.to == e.to) {
                    flag = false;
                    break;
                }
            }
            if (flag) {
                path.push_back(e);
                GetPath(*e.to, to, path, result);
                path.pop_back();
            }
        }
    }
}
vector<vector<Edge>> Graph::GetAllPath(const string &from, const string &to) {
    Node *f = GetNode(from);
    Node *t = GetNode(to);
    if (f == nullptr || t == nullptr)
        return {};

    vector<vector<Edge>> result;
    vector<Edge> path;
    GetPath(*f, *t, path, result);
    return result;
}
