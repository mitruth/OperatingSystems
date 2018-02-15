#include <stdlib.h>
#include <iostream>
#include <vector>
#include <queue>
#define INF 9001

using namespace std;

queue<int> queueT;
int nrNodes, nrEdges;
typedef enum { white, black, gray } COLOR;

typedef struct NodeT
{
    COLOR color;
    int key;
    int d;
    struct NodeT*parent;
    vector<int> next;
}NodeT;

NodeT node[9001];

void BFS(int source)
{
    int nodeReached, i;
    for (i = 0; i < nrNodes; i++)
    {
        node[i].color = white;
        node[i].parent = NULL;
        node[i].d = INF;
    }

    queueT.push(node[source].key);

    node[source].color = gray;
    node[source].parent = NULL;
    node[source].d = 0;

    while (queueT.size() != 0)
    {
        nodeReached = queueT.front();

        queueT.pop();

        for (i = 0; i < node[nodeReached].next.size(); i++)
        {
            if (node[node[nodeReached].next[i]].color == white) {
                node[node[nodeReached].next[i]].color = gray;
                node[node[nodeReached].next[i]].d = node[nodeReached].d + 1;
                node[node[nodeReached].next[i]].parent = node[nodeReached].parent;

                queueT.push(node[node[nodeReached].next[i]].key);
                cout << "Distance of node " << node[node[nodeReached].next[i]].key << " is: " << node[node[nodeReached].next[i]].d << endl;
            }
        }
        node[nodeReached].color = black;
    }
}

void pretty_print(int nrNodes)
{
    int printedNr = 0;
    int i = 0;
    while (printedNr < nrNodes)
    {
        cout << endl;
        cout << node[i].key;
        printedNr++;
        for (int j = 0; j < node[i].next.size(); j++) {
            cout << endl;
            cout << "    " << node[i].next.at(j);
            printedNr++;
            if (node[node[i].next.at(j)].next.size() != 0)
            {
                for (int k = 0; k < node[node[i].next.at(j)].next.size(); k++)
                {
                    cout << endl;
                    cout << "        " <<  node[node[i].next.at(j)].next.at(k);
                    printedNr++;
                }
            }
        }
    }
}

void demo()
{
    int i, j;
    nrNodes = 8;
    nrEdges = 9;
    for (i = 0; i < nrNodes; i++)
    {
        node[i].key = i;
    }

    node[0].next.push_back(node[1].key);
    node[0].next.push_back(node[2].key);
    node[0].next.push_back(node[3].key);
    node[1].next.push_back(node[4].key);
    node[1].next.push_back(node[5].key);
    node[3].next.push_back(node[4].key);
    node[3].next.push_back(node[6].key);

    for (i = 0; i < nrEdges; i++)
    {
        if (node[i].next.size() > 0)
        {
            cout << "Node having key " << i << " has the following nodes on the adjacency list: ";
            for (j = 0; j < node[i].next.size(); j++)
            {
                cout << node[i].next[j] << " ";
            }
            cout << endl;
        }
    }
    BFS(0);
    pretty_print(nrNodes);
}

int main()
{
    demo();
    return 0;
}