#include <bits/stdc++.h>
using namespace std;

// =========================================================
//                 BUILDING DATA ADT
// =========================================================
struct Building {
    int id;
    string name;
    string location;

    Building() {}
    Building(int id, string name, string location) {
        this->id = id;
        this->name = name;
        this->location = location;
    }
};

// =========================================================
//                 BST NODE + BST CLASS
// =========================================================
struct BSTNode {
    Building b;
    BSTNode *left, *right;

    BSTNode(Building b) : b(b), left(NULL), right(NULL) {}
};

class BST {
public:
    BSTNode* root = NULL;

    BSTNode* insert(BSTNode* node, Building b) {
        if (!node) return new BSTNode(b);
        if (b.id < node->b.id)
            node->left = insert(node->left, b);
        else
            node->right = insert(node->right, b);
        return node;
    }

    void insertBuilding(Building b) {
        root = insert(root, b);
    }

    void inorder(BSTNode* node) {
        if (!node) return;
        inorder(node->left);
        cout << node->b.id << " - " << node->b.name << endl;
        inorder(node->right);
    }

    void preorder(BSTNode* node) {
        if (!node) return;
        cout << node->b.id << " - " << node->b.name << endl;
        preorder(node->left);
        preorder(node->right);
    }

    void postorder(BSTNode* node) {
        if (!node) return;
        postorder(node->left);
        postorder(node->right);
        cout << node->b.id << " - " << node->b.name << endl;
    }
};

// =========================================================
//                 AVL TREE NODE + CLASS
// =========================================================
struct AVLNode {
    Building b;
    AVLNode *left, *right;
    int height;
    AVLNode(Building b) : b(b), left(NULL), right(NULL), height(1) {}
};

class AVL {
public:
    AVLNode* root = NULL;

    int getHeight(AVLNode* n) { return n ? n->height : 0; }

    int getBalance(AVLNode* n) {
        return n ? getHeight(n->left) - getHeight(n->right) : 0;
    }

    AVLNode* rightRotate(AVLNode* y) {
        cout << "Performing Right Rotation (RR)\n";
        AVLNode* x = y->left;
        y->left = x->right;
        x->right = y;

        y->height = 1 + max(getHeight(y->left), getHeight(y->right));
        x->height = 1 + max(getHeight(x->left), getHeight(x->right));

        return x;
    }

    AVLNode* leftRotate(AVLNode* x) {
        cout << "Performing Left Rotation (LL)\n";
        AVLNode* y = x->right;
        x->right = y->left;
        y->left = x;

        x->height = 1 + max(getHeight(x->left), getHeight(x->right));
        y->height = 1 + max(getHeight(y->left), getHeight(y->right));

        return y;
    }

    AVLNode* insert(AVLNode* node, Building b) {
        if (!node) return new AVLNode(b);

        if (b.id < node->b.id)
            node->left = insert(node->left, b);
        else
            node->right = insert(node->right, b);

        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        int balance = getBalance(node);

        if (balance > 1 && b.id < node->left->b.id)
            return rightRotate(node);

        if (balance < -1 && b.id > node->right->b.id)
            return leftRotate(node);

        if (balance > 1 && b.id > node->left->b.id) {
            cout << "Performing Left-Right Rotation (LR)\n";
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        if (balance < -1 && b.id < node->right->b.id) {
            cout << "Performing Right-Left Rotation (RL)\n";
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    void insertBuilding(Building b) {
        root = insert(root, b);
    }

    void inorder(AVLNode* node) {
        if (!node) return;
        inorder(node->left);
        cout << node->b.id << " - " << node->b.name << endl;
        inorder(node->right);
    }
};

// =========================================================
//                 GRAPH (MATRIX + LIST)
// =========================================================
class Graph {
public:
    int V;
    vector<vector<int>> adjMatrix;
    vector<vector<pair<int,int>>> adjList;

    Graph(int n) {
        V = n;
        adjMatrix.resize(n, vector<int>(n, 0));
        adjList.resize(n);
    }

    void addEdge(int u, int v, int w) {
        adjMatrix[u][v] = w;
        adjMatrix[v][u] = w;

        adjList[u].push_back({v, w});
        adjList[v].push_back({u, w});
    }

    void BFS(int start) {
        cout << "BFS: ";
        vector<bool> visited(V, false);
        queue<int> q;

        q.push(start);
        visited[start] = true;

        while (!q.empty()) {
            int x = q.front(); q.pop();
            cout << x << " ";

            for (auto i : adjList[x]) {
                if (!visited[i.first]) {
                    visited[i.first] = true;
                    q.push(i.first);
                }
            }
        }
        cout << endl;
    }

    void DFSUtil(int node, vector<bool>& visited) {
        visited[node] = true;
        cout << node << " ";
        for (auto x : adjList[node])
            if (!visited[x.first])
                DFSUtil(x.first, visited);
    }

    void DFS(int start) {
        cout << "DFS: ";
        vector<bool> visited(V, false);
        DFSUtil(start, visited);
        cout << endl;
    }

    vector<int> dijkstra(int src) {
        vector<int> dist(V, INT_MAX);
        dist[src] = 0;

        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
        pq.push({0, src});

        while (!pq.empty()) {
            int d = pq.top().first;
            int node = pq.top().second;
            pq.pop();

            for (auto it : adjList[node]) {
                int v = it.first;
                int w = it.second;

                if (dist[node] + w < dist[v]) {
                    dist[v] = dist[node] + w;
                    pq.push({dist[v], v});
                }
            }
        }
        return dist;
    }

    int findParent(int node, vector<int>& parent) {
        if (parent[node] == node) return node;
        return parent[node] = findParent(parent[node], parent);
    }

    void unionSet(int u, int v, vector<int>& parent, vector<int>& rank) {
        u = findParent(u, parent);
        v = findParent(v, parent);

        if (rank[u] < rank[v]) parent[u] = v;
        else if (rank[v] < rank[u]) parent[v] = u;
        else {
            parent[v] = u;
            rank[u]++;
        }
    }

    void kruskalMST() {
        vector<tuple<int,int,int>> edges;

        for (int i = 0; i < V; i++)
            for (auto x : adjList[i])
                edges.push_back(make_tuple(x.second, i, x.first));

        sort(edges.begin(), edges.end());

        vector<int> parent(V), rank(V, 0);
        for (int i = 0; i < V; i++) parent[i] = i;

        cout << "\nKruskal's MST:\n";

        for (auto e : edges) {
            int w = get<0>(e);
            int u = get<1>(e);
            int v = get<2>(e);

            if (findParent(u, parent) != findParent(v, parent)) {
                cout << u << " -- " << v << " (Weight: " << w << ")\n";
                unionSet(u, v, parent, rank);
            }
        }
    }
};

// =========================================================
//                 EXPRESSION TREE
// =========================================================
struct ExpNode {
    char val;
    ExpNode *left, *right;
    ExpNode(char v) : val(v), left(NULL), right(NULL) {}
};

class ExpressionTree {
public:
    ExpNode* root;

    int evaluate(ExpNode* node) {
        if (!node->left && !node->right)
            return node->val - '0';

        int leftVal = evaluate(node->left);
        int rightVal = evaluate(node->right);

        switch(node->val) {
            case '+': return leftVal + rightVal;
            case '-': return leftVal - rightVal;
            case '*': return leftVal * rightVal;
            case '/': return leftVal / rightVal;
        }
        return 0;
    }
};

// =========================================================
//                         MAIN
// =========================================================
int main() {

    Building b1(1, "Main Block", "Center Campus");
    Building b2(5, "CSE Dept", "North Wing");
    Building b3(3, "Library", "East Side");
    Building b4(2, "Admin", "South Wing");

    BST bst;
    bst.insertBuilding(b1);
    bst.insertBuilding(b2);
    bst.insertBuilding(b3);
    bst.insertBuilding(b4);

    cout << "BST Inorder:\n";
    bst.inorder(bst.root);

    AVL avl;
    avl.insertBuilding(b1);
    avl.insertBuilding(b2);
    avl.insertBuilding(b3);
    avl.insertBuilding(b4);

    cout << "\nAVL Inorder:\n";
    avl.inorder(avl.root);

    Graph g(6);
    g.addEdge(1,2,4);
    g.addEdge(1,3,2);
    g.addEdge(2,4,7);
    g.addEdge(3,5,1);

    cout << "\nGraph BFS:\n";
    g.BFS(1);

    cout << "Graph DFS:\n";
    g.DFS(1);

    cout << "\nShortest distances from Building 1:\n";
    vector<int> dist = g.dijkstra(1);
    for (int i = 0; i < dist.size(); i++)
        cout << "1 -> " << i << " = " << dist[i] << endl;

    g.kruskalMST();

    ExpressionTree et;
    et.root = new ExpNode('*');
    et.root->left = new ExpNode('+');
    et.root->right = new ExpNode('3');
    et.root->left->left = new ExpNode('2');
    et.root->left->right = new ExpNode('4');

    cout << "\nEnergy Bill (Expression Tree) Result: " 
         << et.evaluate(et.root) << endl;

    return 0;
}
