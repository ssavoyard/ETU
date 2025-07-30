#include <iostream>
#include <ctime>
#include <cstring>
using namespace std;

class Node {
    char d;
    Node* lft;
    Node* rgt;
public:
    Node() : lft(nullptr), rgt(nullptr) {}
    ~Node() {
        if (lft) delete lft;
        if (rgt) delete rgt;
    }
    friend class Tree;
};

class Tree {
    Node* root;
    char num, maxnum;
    int maxrow, offset;
    char** SCREEN;
    void clrscr();
    Node* MakeNode(int depth, bool random);
    void OutNodes(Node* v, int r, int c);
    void preorderTraversal(Node* node);
    int countDepthTwo(Node* node, int depth);

    Tree(const Tree&) = delete;
    Tree(Tree&&) = delete;
    Tree operator=(const Tree&) const = delete;
    Tree operator=(Tree&&) const = delete;

public:
    Tree() = delete;
    Tree(char num, char maxnum, int maxrow);
    ~Tree();
    void MakeTree(bool random) { root = MakeNode(0, random); }
    bool exist() { return root != nullptr; }
    void OutTree();
    void OutPreorderTraversal();
    int countDepthTwo();
};

// Destructor
Tree::~Tree() {
    for (int i = 0; i < maxrow; ++i) delete[] SCREEN[i];
    delete[] SCREEN;
    delete root;
}

// Constructor
Tree::Tree(char nm, char mnm, int mxr) : num(nm), maxnum(mnm), maxrow(mxr), offset(40), root(nullptr), SCREEN(new char*[maxrow]) {
    for (int i = 0; i < maxrow; ++i)
        SCREEN[i] = new char[80];
}

// Screen clearing
void Tree::clrscr() {
    for (int i = 0; i < maxrow; i++)
        memset(SCREEN[i], '.', 80);
}

// Node creation with symmetric filling
Node* Tree::MakeNode(int depth, bool random) {
    Node* v = nullptr;
    int Y;
    if (random)
        Y = (depth < rand() % 6 + 1) && (num <= 'z');
    else {
        cout << "Node (" << num << ',' << depth << ")1/0: ";
        cin >> Y;
    }

    if (Y) {
        v = new Node;
        v->lft = MakeNode(depth + 1, random);
        v->d = num++;
        v->rgt = MakeNode(depth + 1, random);
    }
    return v;
}

/* ================= Top-down variant ================= */
// void Tree::OutNodes(Node* v, int r, int c) {
//     if (v == nullptr || r >= maxrow) return;

//     if (r && c && (c < 80))
//         SCREEN[r - 1][c - 1] = v->d;
//     if (r < maxrow) {
//         if (v->lft)
//             OutNodes(v->lft, r + 1, c - (offset >> r));
//         if (v->rgt)
//             OutNodes(v->rgt, r + 1, c + (offset >> r));
//     }
// }

// void Tree::OutTree() {
//     clrscr();
//     OutNodes(root, 1, offset);
//     for (int i = 0; i < maxrow; i++) {
//         SCREEN[i][79] = 0;
//         cout << '\n' << SCREEN[i];
//     }
// }

/* ================= Bottom-up variant ================= */
// void Tree::OutNodes(Node* v, int r, int c) {
//     if (v == nullptr || r >= maxrow) return;

//     if (r && c && (c < 80))
//         SCREEN[maxrow - r][c - 1] = v->d;

//     if (r < maxrow) {
//         if (v->lft)
//             OutNodes(v->lft, r + 1, c - (offset >> r));
//         if (v->rgt)
//             OutNodes(v->rgt, r + 1, c + (offset >> r));
//     }
// }

// void Tree::OutTree() {
//     clrscr();
//     OutNodes(root, 1, offset);
//     for (int i = 0; i < maxrow; i++) {
//         SCREEN[i][79] = 0;
//         cout << '\n' << SCREEN[i];
//     }
// }

/* ================= Left-to-right variant ================= */
void Tree::OutNodes(Node* v, int depth, int indent) {
    if (depth >= maxrow) return;

    // Initialize a line with dots
    string line(maxrow, '.');

    if (v != nullptr) {
        line[indent] = v->d;
        cout << line << endl;
    }

    if (v->lft) OutNodes(v->lft, depth + 1, indent + 1);
    if (v->rgt) OutNodes(v->rgt, depth + 1, indent + 1);
}

void Tree::OutTree() {
    clrscr();
    OutNodes(root, 0, 0);
    cout << '\n';
}

// Preorder traversal
void Tree::preorderTraversal(Node* node) {
    if (node == nullptr)
        return;

    cout << node->d << " ";
    preorderTraversal(node->lft);
    preorderTraversal(node->rgt);
}

void Tree::OutPreorderTraversal() {
    cout << "Preorder Traversal: ";
    preorderTraversal(root);
    cout << endl;
}

// Count nodes at depth <= 2
int Tree::countDepthTwo(Node* node, int depth) {
    if (node == nullptr || depth > 2)
        return 0;
    return 1 + countDepthTwo(node->lft, depth + 1) + countDepthTwo(node->rgt, depth + 1);
}

int Tree::countDepthTwo() {
    return countDepthTwo(root, 0);
}

int main() {
    srand(static_cast<unsigned int>(time(0))); // Initialize random number generator
    Tree Tr('a', 'z', 8);

    int userOption;
    cout << "Choose a method to create the tree:\n";
    cout << "1. Random creation\n";
    cout << "2. Manual input\n";
    cout << "Choice: ";
    cin >> userOption;

    if (userOption == 1) Tr.MakeTree(true);
    else if (userOption == 2) Tr.MakeTree(false);
    else {
        cout << "Invalid option.";
        return 1;
    }

    if (Tr.exist()) {
        Tr.OutTree();
        Tr.OutPreorderTraversal();
        int count = Tr.countDepthTwo();
        cout << "\nNumber of nodes at depth 2 or less: " << count << endl;
    }
    else {
        cout << "The tree is a null-tree.";
    }
    cout << "\n=== End ===";
    return 0;
}
