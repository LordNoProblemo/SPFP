/*
 * KDTree.h
 *
 *  Created on: Apr 27, 2017
 *      Author: nimro
 */

#ifndef KDTREE_H_
#define KDTREE_H_
#include "SPBPriorityQueue.h"
#include "SPPoint.h"
#include "SPConfig.h"
typedef struct kdn KDNode;

typedef struct _kdArray
{
	SPPoint** pointsArray;
	int** indexOrdPerDim;
	int n,d, numofPoints;

}KDArray;
struct kdn
{
	int Dim;
	double Val;
	KDNode* Left,*Right;
	SPPoint* Data;
};

/**
 * Destroys a pointer of KDArray
 */
void destroyArray(KDArray* ar);
/**
 * Builds a new KDArray pointer containing points
 * @param points - Points list
 * @param size - num of points
 */
KDArray* init(SPPoint** points, int size);
/**
 * Splits the KDArray KDA according to coor
 */
KDArray** split(KDArray* KDA, int coor);
/**
 * Destroys a pointer of KDNode
 */
void destroyKDN(KDNode* KDN);
/**
 * Return a root of KDTree built from KDArray KDA with Split Method method
 */
KDNode* buildFromKDArray(KDArray* KDA,SPLIT_METHOD method, int lastCoor);
/**
 * Returns a root of KDTree build from list of points with Split Method method
 */
KDNode* buildFromPoints(SPPoint** points, SPLIT_METHOD method, int Size);
#endif /* KDTREE_H_ */
