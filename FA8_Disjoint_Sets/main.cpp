#include<iostream>
using namespace std;


typedef struct node
{
    int key;
    int rank;
    node *parent;
}NodeT;

NodeT* MakeSet(int x) //creates a new set whose only member (and thus representative) is x
{
    NodeT *n = (NodeT*)malloc(sizeof(NodeT));
    n->key = x;
    n->parent = n;
    n->rank = 0;
    return n;
}

void Link(NodeT *x, NodeT *y)
{
    if (x->rank > y->rank)
    {
        y->parent = x;
    }
    else
    {
        x->parent = y;
        if (x->rank == y->rank)
        {
            y->rank++;
        }
    }
}

NodeT* FindSet(NodeT *x)//returns a pointer to the representative of the (unique) set contain- ing x
{
    if (x != x->parent)
    {
        x->parent = FindSet(x->parent);
    }
    return x->parent;
}

void Union(NodeT *x, NodeT *y) //unites the dynamic sets that contain x and y, say Sx and Sy,
// into a new set that is the union of these two sets
{
    Link(FindSet(x), FindSet(y));
}

int main()
{

    NodeT *nodes[11];
    for (int i = 1; i <= 10; i++)
        nodes[i] = MakeSet(i);

    for (int i = 1; i <= 5; i++)
    {
        int rand1 = rand() % 10 + 1;
        int rand2 = rand() % 10 + 1;
        if (rand1 == rand2)
        {
            rand2++;
        }
        cout << "Before Union: ";
        cout << nodes[rand1]->key << "(" << FindSet(nodes[rand1])->key << ")" << " and " << nodes[rand2]->key << "(" << FindSet(nodes[rand2])->key << ") " << endl;

        Union(nodes[rand1], nodes[rand2]);

        cout << "After Union: ";
        cout << nodes[rand1]->key << "(" << FindSet(nodes[rand1])->key << ") " << " and " << nodes[rand2]->key << "(" << FindSet(nodes[rand2])->key << ") " << endl;

        cout << endl;
    }

    cout << "The final result: " << endl;
    for (int i = 1; i <= 10; i++)
        cout << nodes[i]->key << "(" << FindSet(nodes[i])->key << ") ";
    cout << endl;

    for (int i = 1; i <= 10; i++)
        free(nodes[i]);

    return 0;
}