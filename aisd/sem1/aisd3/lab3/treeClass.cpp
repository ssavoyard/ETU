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
	void preorderTraversal(Node* node); // внутренний метод для прямого обхода
	int countDepthTwo(Node* node, int depth);

	Tree(const Tree&) = delete;
	Tree(Tree&&) = delete;
	Tree operator= (const Tree&) const = delete;
	Tree operator= (Tree&&) const = delete;

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

// Деструктор
Tree::~Tree() {
	for (int i = 0; i < maxrow; ++i) delete[]SCREEN[i];
	delete[]SCREEN;
	delete root;
}

// Конструктор
Tree::Tree(char nm, char mnm, int mxr) : num(nm), maxnum(mnm), maxrow(mxr), offset(40), root(nullptr), SCREEN(new char* [maxrow]) {
	for (int i = 0; i < maxrow; ++i)
		SCREEN[i] = new char[80];
}

// Очистка экрана
void Tree::clrscr() {
	for (int i = 0; i < maxrow; i++)
		memset(SCREEN[i], '.', 80);
}

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

void Tree::OutNodes(Node* v, int r, int c) {
	if (r && c && (c < 80))
		SCREEN[r - 1][c - 1] = v->d;
	if (r < maxrow) {
		if (v->lft)
			OutNodes(v->lft, r + 1, c - (offset >> r));
		if (v->rgt)
			OutNodes(v->rgt, r + 1, c + (offset >> r));
	}
}

void Tree::OutTree() {
	clrscr();
	OutNodes(root, 1, offset);
	for (int i = 0; i < maxrow; i++) {
		SCREEN[i][79] = 0;
		cout << '\n' << SCREEN[i];
	}
	cout << '\n';
}

void Tree::preorderTraversal(Node* node) {
	if (node == NULL)
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

int Tree::countDepthTwo(Node* node, int depth) {
	if (node == nullptr || depth > 2)
		return 0;
	return 1 + countDepthTwo(node->lft, depth + 1) + countDepthTwo(node->rgt, depth + 1);
}

int Tree::countDepthTwo() {
	return countDepthTwo(root, 0);
}

int main() {
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


/*---- Examples ----*/
/*

Node (a,0)1/0: 1
Node (a,1)1/0: 1
Node (a,2)1/0: 1
Node (a,3)1/0: 0
Node (b,3)1/0: 1
Node (b,4)1/0: 0
Node (c,4)1/0: 0
Node (d,2)1/0: 1
Node (d,3)1/0: 1
Node (d,4)1/0: 0
Node (e,4)1/0: 0
Node (f,3)1/0: 0
Node (g,1)1/0: 1
Node (g,2)1/0: 0
Node (h,2)1/0: 0

0.......................................f.......................................
1...................c.......................................g...................
2.........a...................e.................................................
3..............b.........d......................................................
4...............................................................................
5...............................................................................
6...............................................................................
7...............................................................................


Node (a,0)1/0: 1
Node (a,1)1/0: 1
Node (a,2)1/0: 1
Node (a,3)1/0: 0
Node (b,3)1/0: 0
Node (c,2)1/0: 1
Node (c,3)1/0: 0
Node (d,3)1/0: 0
Node (e,1)1/0: 1
Node (e,2)1/0: 0
Node (f,2)1/0: 0

0.......................................d.......................................
1...................b.......................................e...................
2.........a...................c.................................................
3...............................................................................
4...............................................................................
5...............................................................................
6...............................................................................
7...............................................................................

*/


/********************************************************

// Tree traversal in C++

#include <iostream>
using namespace std;

struct Node {
	int data;
	struct Node* left, * right;
	Node(int data) {
		this->data = data;
		left = right = NULL;
	}
};

// Preorder traversal
void preorderTraversal(struct Node* node) {
	if (node == NULL)
		return;

	cout << node->data << "->";
	preorderTraversal(node->left);
	preorderTraversal(node->right);
}

// Postorder traversal
void postorderTraversal(struct Node* node) {
	if (node == NULL)
		return;

	postorderTraversal(node->left);
	postorderTraversal(node->right);
	cout << node->data << "->";
}

// Inorder traversal
void inorderTraversal(struct Node* node) {
	if (node == NULL)
		return;

	inorderTraversal(node->left);
	cout << node->data << "->";
	inorderTraversal(node->right);
}

int main() {
	struct Node* root = new Node(1);
	root->left = new Node(12);
	root->right = new Node(9);
	root->left->left = new Node(5);
	root->left->right = new Node(6);

	cout << "Inorder traversal ";
	inorderTraversal(root);

	cout << "\nPreorder traversal ";
	preorderTraversal(root);

	cout << "\nPostorder traversal ";
	postorderTraversal(root);
	return 0;
}
************************************************/