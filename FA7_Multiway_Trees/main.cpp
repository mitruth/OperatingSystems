#include <iostream>

using namespace std;

typedef struct node
{
	int key;
	struct node *child;
	struct node *brother;
} NodeT;

NodeT *createNode(int key) //just creates a node, with a given key, and with its brother and child being NULL
{
    NodeT *p;
	p = (NodeT*)malloc(sizeof(NodeT));
	if (p)
	{
		p->key = key;
		p->child = NULL;
		p->brother = NULL;
	}
	return p;
}

void printTree(NodeT *root, int level)
{
	if (root)
	{
		for (int i = 0; i < level; i++)
			cout << " ";
		cout << root->key << endl;
		printTree(root->child, level + 4);
		printTree(root->brother, level);
	}
}

int main()
{
	int N = 9;
	int T1[10] = {2, 7, 5, 2, 7, 7, -1, 5, 2};
    NodeT **tree = (NodeT**)malloc(sizeof(NodeT*) * N);
    NodeT *root;

	for (int i = 0; i < N; i++)
		tree[i] = createNode(i + 1);
	for (int i = 0; i < N; i++)
	{
		if (T1[i] == -1)
        {
            root = tree[i];
        }
        else if (tree[T1[i] - 1]->child == NULL)
        {
            tree[T1[i] - 1]->child = tree[i];
        }
		else
		{
            NodeT *temp = tree[T1[i] - 1]->child;
			while (temp->brother != NULL)
            {
                temp = temp->brother;
            }
			temp->brother = tree[i];
		}
	}
	printTree(root, 0);
	return 0;
}