/**
 * @author: Alexander Mark Thompson
 * @title: CS 201 -- Assignment 3 -- Dijkstra's Algorithm
 */

#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "darray.h"
#include "binomial.h"
#include "integer.h"
#include "vertex.h"

static void displayForest(FILE *, queue *);
static void dijkstra(FILE *, Binomial *, DArray *);

int main(int argc, char **argv)
{
	if(argc < 2 || argc >= 3)
	{
		fprintf(stderr, "Incorrect amount of arguments\n");
		exit(-1);
	}

	FILE *fp = fopen(argv[1], "r");

	DArray *list = newDArray(displayVertex);

	Edge *e = readData(fp);
	while(e != NULL)
	{
		insertVertex(list, e->from, e->to, e->weight);
		insertVertex(list, e->to, e->from, e->weight);
		free(e);
		e = readData(fp);
	}

	Binomial *heap = fillHeap(list);

	dijkstra(stdout, heap, list);

	fclose(fp);
}

static void dijkstra(FILE *fp, Binomial *h, DArray *list)
{
	Vertex *minVert = startVertex(list);

	minVert->prev = minVert;

	queue *nodesVisited = newQueue(displayVertex);

	while(sizeBinomial(h) != 0)
	{
		Vertex *u = (Vertex *) extractBinomial(h);

		if(u->prev == NULL)
		{
			u->ID = 0;
			displayForest(fp, nodesVisited);
			nodesVisited = newQueue(displayVertex);
		}

		enqueue(nodesVisited, u);
		u->visited = 1;

		for(int i = 0; i < sizeDArray(u->adjacentV);++i)
		{
			Adjacent *a = (Adjacent *)getDArray(u->adjacentV, i);
			Vertex *v = findVertex(list, a->vertex->value);

			if(v->visited != 1)
			{
				minVert->ID = 0;
				if((u->ID + a->weight) < v->ID || v->prev == NULL)
				{
					v->prev = u;
					v->ID = (u->ID + a->weight);
					v->touched = v->prev->touched + 1;
					decreaseKeyBinomial(h, v->node, v);
				}
			}
		}
	}

	displayForest(fp, nodesVisited);
}

static void displayForest(FILE *fp, queue *branch)
{
	if(sizeQueue(branch) == 0)
	{
		return;
	}

	Binomial *priority = newBinomial(displayVertex, compareVertex, updateVertex);
	Vertex *v = NULL;

	int i = 0, stepSize = 0, size = sizeQueue(branch);

	while(i < size)
	{
		void *q = dequeue(branch);
		v = q;
		if(v->touched > stepSize)
		{
			stepSize = v->touched;
		}

		insertBinomial(priority, v);
		i++;
	}

	queue *levelOrder[stepSize];
	i = 0;

	while(i <= stepSize)
	{
		levelOrder[i] = newQueue(displayVertex);
		i++;
	}

	i = 0;
	while(i < size)
	{
		v = (Vertex *) extractBinomial(priority);
		enqueue(levelOrder[v->touched], v);
		i++;
	}

	i = 0;
	while(i <= stepSize)
	{
		fprintf(fp, "%d : ", i);
		while(sizeQueue(levelOrder[i]) != 0)
		{
			v = (Vertex *) dequeue(levelOrder[i]);
			displayVertex(stdout, v);
			if(sizeQueue(levelOrder[i]) > 0)
			{
				fprintf(fp, " ");
			}
		}
		fprintf(fp, "\n");
		i++;
	}
	fprintf(fp, "----\n");
}