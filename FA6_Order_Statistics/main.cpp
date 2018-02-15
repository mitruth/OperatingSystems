#include <iostream>
#include "Profiler_linux.h";
using namespace std;

Profiler profiler ("FA6");

//rank of an element is the position at which it would be printed in an inorder walk of the tree

int n, osSelectTotal, osDeleteTotal;

struct NodeT
{
    int key;
    int size; //size of the subtree
    struct NodeT* left;
    struct NodeT* right;
    struct NodeT* parent;
};

struct NodeT* createNode(int value, NodeT* parent)
{
    NodeT* node = (struct NodeT*) malloc(sizeof(struct NodeT));
    node->key = value;
    node->size = NULL;
    node->left = NULL;
    node->right = NULL;
    node->parent = parent;

    return node;
}

struct NodeT* createBST(int array[], int s, int e, NodeT* parent)
{
    if (s > e)
        return NULL;

    int m = (s + e) / 2;
    struct NodeT *root = createNode(array[m], parent);

    root->left =  createBST(array, s, m-1, root);

    root->right = createBST(array, m+1, e, root);

    if (root->left != NULL && root->right != NULL) {
        root->size = root->left->size + root->right->size + 1;
    }
    else if (root->left != NULL && root->right == NULL){
        root->size = root->left->size + 1;
    }
    else if (root->left == NULL && root->right != NULL){
        root->size = root->right->size + 1;
    }
    else {
        root->size = 1;
    }

    return root;
}

struct NodeT* OSselect(NodeT* root, int i) { //returns a pointer to the node containing the ith smallest key in the subtree rooted at root

    int r;
    osSelectTotal++;
    if (root->left == NULL) {
        r = 1;
    }
    else {
        r = root->left->size +1; //rank of node root within the subtree rooted at root
    }

    if (i == r) { //means the node is the ith smallest element
        osSelectTotal++;
        return root;
    }
    else if (i < r) {
        osSelectTotal++;
        return OSselect(root->left, i); // => ith smallest element is in the left subtree
    }
    else {
        osSelectTotal++;
        return OSselect(root->right, i-r); // => ith smallest element is in the right subtree
        //right subtree of root contains r elements that come before x’s right subtree in an inorder tree walk
    }
}

struct NodeT * minValueNode(NodeT* node) //returns the left most element of the left subtree
{
    NodeT* curr = node;

    while (curr->left != NULL)
    {
        osDeleteTotal++;
        curr = curr->left;
    }

    return curr;
}

struct NodeT* deleteNode(NodeT* root, int key)
{
    if (root == NULL) return root;

    osDeleteTotal++;
    if (key < root->key)
        root->left = deleteNode(root->left, key);

    else if (key > root->key)
        root->right = deleteNode(root->right, key);

    else
    {
        if(root->left == NULL && root->right == NULL)
        {
            osDeleteTotal++;
            NodeT* p = root->parent;
            while (p != NULL)
            {
                p->size--;
                p = p->parent;
                osDeleteTotal++;
            }
            free(root);
            root = NULL;
        }

        else if (root->left == NULL)
        {
            osDeleteTotal++;
            NodeT* p = root->parent;
            while (p != NULL)
            {
                p->size--;
                p = p->parent;
                osDeleteTotal++;
            }
            NodeT* temp = root;
            NodeT* par = root->parent;
            root = root->right;
            root->parent = par;
            free(temp);
            return root;
        }
        else if (root->right == NULL)
        {
            osDeleteTotal++;
            NodeT* p = root->parent;
            while (p != NULL)
            {
                p->size--;
                p = p->parent;
                osDeleteTotal++;
            }
            NodeT* temp = root;
            NodeT* par = root->parent;
            root = root->left;
            root->parent = par;
            free(temp);
            return root;
        }
        else
        {
            NodeT* temp = minValueNode(root->right);
            osDeleteTotal++;
            root->key = temp->key;
            root->right = deleteNode(root->right, temp->key);
        }
    }
    return root;
}


void preOrder(struct NodeT* node) //
{
    if (node == NULL)
        return;
    if (node->left != NULL && node->right != NULL)
    {
        cout << node->key << " ( size = " << node->size << " )" << " and has the left child: " << node->left->key << " , and the right child: " << node->right->key;
    }
    else if (node->left != NULL && node->right == NULL)
    {
        cout << node->key << " ( size = " << node->size << " )" << " and has the left child: " << node->left->key << " , and the right child: " << 0;
    }
    else if (node->left == NULL && node->right != NULL)
    {
        cout << node->key << " ( size = " << node->size << " )" << " and has the left child: " << 0 << " , and the right child: " << node->right->key;
    }
    else
    {
        cout << node->key << " ( size = " << node->size << " )" << " and has the left child: " << 0 << " , and the right child: " << 0;
    }
    cout<< "\n";
    preOrder(node->left);
    preOrder(node->right);
}

int main()
{

//    int a[1000];
//
//    n = 11;
//    for (int i = 1; i <= n; i++)
//    {
//        a[i] = i;
//    }
//    struct NodeT *root = createBST(a, 1, n, NULL);
//
//    preOrder(root);
//
//    cout << endl << endl;
//
//    int r = rand() % n + 1;
//    NodeT* node1 = OSselect(root, r);
//    cout << "OSselect in the tree after the " << r << "th smallest element in the subtree of the node: " << root->key << " is: " << node1->key << endl;
//    cout << "Tree after deleting: " << node1->key << endl;
//    deleteNode(root, node1->key);
//    preOrder(root);
//    cout << endl;
//    r = rand() % n + 1;
//    NodeT* node2 = OSselect(root, r);
//    cout << "OSselect in the tree after the " << r << "th smallest element in the subtree of the node: " << root->key << " is: " << node2->key << endl;
//    cout << "Tree after deleting: " << node2->key << endl;
//    deleteNode(root, node2->key);
//    preOrder(root);
//    cout << endl;
//    r = rand() % n + 1;
//    NodeT* node3 = OSselect(root, r);
//    cout << "OSselect in the tree after the " << r << "th smallest element in the subtree of the node: " << root->key << " is: " << node3->key << endl;
//    cout << "Tree after deleting: " << node3->key << endl;
//    deleteNode(root, node3->key);
//    preOrder(root);
//    cout << endl;
//
    int a[10001];

    for(n = 100; n < 10000; n+=100)
    {
        for (int i = 1; i <= n; i++)
        {
            a[i] = i;
        }
        for (int j = 0; j < 5; j++)
        {
            int tree_size = n;
            struct NodeT *root = createBST(a, 1, tree_size, NULL);

            while (tree_size > 0)
            {
                int r = rand() % tree_size + 1;
                NodeT* toBeDeleted = OSselect(root, r);
                deleteNode(root, toBeDeleted->key);
                tree_size--;
                osDeleteTotal = 0;
                osSelectTotal = 0;
                profiler.countOperation("OS-Select:", tree_size, osSelectTotal);
                profiler.countOperation("OS-Delete:", tree_size, osDeleteTotal);
            }
        }
    }

    profiler.showReport();

    return 0;
}