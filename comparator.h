/**
 * @author: Alexander Mark Thompson
 * @title: CS 201 -- Assignment 3 -- Dijkstra's Algorithm
 */

#include <stdio.h>
#include "integer.h"
#include "real.h"
#include "string.h"

#ifndef __COMPARATOR__INCLUDED__
#define __COMPARATOR__INCLUDED__

typedef int (*Comparator)(void *, void *);
extern int intComparator(void *x, void *y);
extern int realComparator(void *x, void *y);

#endif