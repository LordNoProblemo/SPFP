/*
 * SPPoint.c
 *
 *  Created on: Dec 16, 2016
 *      Author: Asaf
 */

#include "SPPoint.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct sp_point_t
{
	int dim; // Dimension of point
	double * data; // Data of point
	int index; // index of point
};

SPPoint* spPointCreate(double* data, int dim, int index)
{
	if(index < 0 || dim <= 0) return NULL; // Checking edge cases
	SPPoint * p = (SPPoint *) malloc(sizeof(SPPoint));
	p->dim = dim; // initialize dim
	p->data = (double *) malloc(sizeof(double) * dim);
	memcpy(p->data, data, dim * sizeof(double)); // initialize data
	p->index = index; // initialize index
	return p;
}

SPPoint* spPointCopy(SPPoint* source)
{
	assert (source != NULL);
	return spPointCreate(source->data, source->dim, source->index); // create new point with same data, index and dim
}

void spPointDestroy(SPPoint* point)
{
	if (point == NULL) return; //edge cases
	free(point->data); //free pointer of data
	free(point);
}

int spPointGetDimension(SPPoint* point)
{
	assert (point != NULL);
	return point->dim;
}

int spPointGetIndex(SPPoint* point)
{
	assert (point != NULL);
	return point->index;
}

double spPointGetAxisCoor(SPPoint* point, int axis)
{
	assert (point!=NULL && axis < point->dim && axis >= 0);
	return point->data[axis];
}

double spPointL2SquaredDistance(SPPoint* p, SPPoint* q)
{
	assert (p!=NULL && q!=NULL && p->dim == p->dim);
	double l2 = 0;
	for(int i = 0; i < p->dim; i++)
	{
		double dist = p->data[i] - q->data[i]; //distance between p and q in axis i
		l2 += dist * dist;
	}
	return l2;
}
