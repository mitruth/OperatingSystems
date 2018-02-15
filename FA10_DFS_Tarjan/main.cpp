#include <iostream>

using namespace std;

typedef struct vertex
{
	struct vertex* predecessor;
	struct vertex* adj[10000];
	int adjLength;
	int val;
	int distance;
	int finish;
	char* color;
	int index;
	int lowLink;
	bool onStack;
};

typedef struct graph
{
	int V;
	vertex* vertexes[5000];
};

typedef struct stack
{
	int top;
	vertex* vertexes[100];
};

typedef struct component {
	vertex* vertexes[100];
	int length;
};

typedef struct list
{
	component* components[100];
	int length;
};

int t;

bool emptyStack(stack* S)
{
	if (S->top == 0)
    {
		return true;
	}
	return false;
}

void push(stack* S, vertex* v)
{
	S->top = S->top + 1;
	S->vertexes[S->top] = (vertex*)malloc(sizeof(vertex));
	S->vertexes[S->top] = v;
}

vertex* pop(stack* S)
{
	if (emptyStack(S))
    {
		return NULL;
	}
	else S->top = S->top - 1;
	return S->vertexes[S->top + 1];
}

void DFS_VISIT(graph* G, vertex* v, stack* S)
{
	t = t + 1;
	v->distance = t;
	v->color = "GRAY";
	for (int i = 0; i < v->adjLength; i++)
    {
		if (v->adj[i]->color == "WHITE")
        {
			v->adj[i]->predecessor = v;
			DFS_VISIT(G, v->adj[i], S);
		}
	}
	v->color = "BLACK";
	t = t + 1;
	v->finish = t;
    push(S, v);
}

stack* DFS(graph* G)
{
	stack* S = (stack*)malloc(sizeof(stack));
	S->top = 0;

	for (int i = 0; i < G->V; i++)
    {
		G->vertexes[i]->color = "WHITE";
		G->vertexes[i]->predecessor = NULL;
	}
	t = 0;
	for (int i = 0; i < G->V; i++) {
		if (G->vertexes[i]->color == "WHITE")
        {
			DFS_VISIT(G, G->vertexes[i], S);
		}
	}
	return S;
}

void TOPOLOGICAL_SORT(graph*G)
{
	stack* S = DFS(G);
	int k = 0;
	while (k < G->V)
    {
        cout << pop(S)->val << " ";
		k++;
	}
}

stack* S;
list* scc;

void strongConnect(vertex* v, list* scc, stack* S,int index)
{
	v->index = index;
	v->lowLink = index;
	index++;
    push(S, v);
	v->onStack = true;

	for (int i = 0; i < v->adjLength; i++)
    {
		if (v->adj[i]->index == -1)
        {
            strongConnect(v->adj[i], scc, S,index);
			v->lowLink = min(v->lowLink, v->adj[i]->lowLink);
		}
		else if (v->onStack)
        {
			v->lowLink = min(v->lowLink, v->adj[i]->index);
		}
	}
	if (v->lowLink == v->index)
    {
		vertex* u;
		component* comp = (component*)malloc(sizeof(component));
		comp->length = 0;
		
		int compIndex = 0;
		do {
			u = pop(S);
			comp->vertexes[compIndex] = u;
			compIndex++;
		} while (u != v);
        comp->length = compIndex;
		scc->components[scc->length] = comp;
        scc->length++;
	}
}

list* Tarjan(graph* G)
{
	S= (stack*)malloc(sizeof(stack));
	S->top = 0;
	scc = (list*)malloc(sizeof(list));
	scc->length = 0;

	for (int i = 0; i < G->V; i++)
    {
		if (G->vertexes[i]->index == -1)
        {
            strongConnect(G->vertexes[i], scc, S,0);
		}
	}
	return scc;
}

int main()
{

	stack* S = (stack*)malloc(sizeof(stack));
	S->top = 0;
	int v = 9;
	
	vertex* a[9];

	for (int i = 0; i < v; i++)
    {
		a[i] = (vertex*)malloc(sizeof(vertex));
		a[i]->val = i;
		a[i]->adjLength = 0;
		a[i]->index = -1;
		a[i]->onStack = false;
	}

	a[0]->adj[0] = a[1];
	a[0]->adj[1] = a[2];
	a[0]->adjLength = 2;

	a[1]->adj[0] = a[3];
	a[1]->adj[1] = a[4];
	a[1]->adjLength = 2;

	a[2]->adj[0] = a[3];
	a[2]->adjLength = 1;

	a[3]->adj[0] = a[0];
	a[3]->adj[1] = a[7];
	a[3]->adjLength = 2;

	a[4]->adj[0] = a[5];
	a[4]->adj[1] = a[7];
	a[4]->adjLength = 2;

	a[5]->adj[0] = a[7];
	a[5]->adj[1] = a[8];
	a[5]->adjLength = 2;

	for (int i = 0; i < v; i++)
    {
        cout << a[i]->val << ": ";
		for (int j = 0; j < a[i]->adjLength; j++)
            cout << a[i]->adj[j]->val << "  ";
		cout << endl;
	}

	graph* G1;
	G1 = (graph*)malloc(sizeof(graph));
	G1->V = v;

	for (int i = 0; i < v; i++)
    {
		G1->vertexes[i] = (vertex*)malloc(sizeof(vertex));
		G1->vertexes[i] = a[i];
	}

    cout << endl;
    cout << "Topological sort: ";
    cout << endl;
	stack* topoStack;
	topoStack = (stack*)malloc(sizeof(stack));
	topoStack->top = 0;
	TOPOLOGICAL_SORT(G1);

	//TARJAN
    cout << endl << endl;
    cout << "Tarjan result: ";
    cout << endl;
	list* result = Tarjan(G1);
	for (int i = 0; i < result->length; i++)
    {
		for (int j = 0; j < result->components[i]->length; j++)
            cout << result->components[i]->vertexes[j]->val << " ";
		printf("\n");
	}
    return 0;
}

