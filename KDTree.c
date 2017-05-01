/*
 * KDTree.c
 *
 *  Created on: Apr 15, 2017
 *      Author: nimro
 */

#include <stdlib.h>
#include "SPLogger.h"
#include "KNN.h"
#include <stdio.h>
#include "KDTree.h"

SPPoint** pointsToCompare;
int coorToCompare;
int comp(const void * a, const void * b)
{
	int iA = *((int*)a);
	int iB = *((int*)b);
	double ret = spPointGetAxisCoor(pointsToCompare[iA],coorToCompare)-spPointGetAxisCoor(pointsToCompare[iB],coorToCompare);
	if(ret>0)
		return 1;
	if(ret< 0 )
		return -1;
	return 0;
}
void destroyArray(KDArray* ar)
{
	if(ar == NULL)
		return;
	if(ar->indexOrdPerDim == NULL)
	{
		free(ar);
		return;
	}
	int i = 0;
	for(;i<ar->d;i++)
		if(ar->indexOrdPerDim[i]!=NULL)
			free(ar->indexOrdPerDim[i]);
	free(ar);
	return;
}
KDArray* init(SPPoint** points, int size)
{
	if(size == 0)
	{
		spLoggerPrintInfo("No points given for KDTree!");
		spLoggerPrintInfo("Returning NULL!");
		return NULL;
	}
	KDArray* ret = (KDArray*)malloc(sizeof(KDArray));
	if(ret == NULL)
	{
		spLoggerPrintError("Error while trying to allocating data for KDArray",__FILE__,__func__,__LINE__);
		spLoggerPrintInfo("Returning NULL!");
		return NULL;
	}
	ret->pointsArray = points;
	ret->n = size;
	ret->numofPoints = size;
	ret->d = spPointGetDimension(points[0]);
	ret->indexOrdPerDim = (int**)malloc(ret->d*sizeof(int*));
	if(ret->indexOrdPerDim == NULL)
	{
		spLoggerPrintError("Error while trying to allocating data for KDArray",__FILE__,__func__,__LINE__);
		destroyArray(ret);
		return NULL;
	}
	int i = 0;
	pointsToCompare = points;
	for(;i<ret->d;i++)
	{
		ret->indexOrdPerDim[i]=(int*)malloc(sizeof(int)*ret->n);
		if(ret->indexOrdPerDim[i] == NULL)
		{
			spLoggerPrintError("Error while trying to allocating data for KDArray",__FILE__,__func__,__LINE__);
			spLoggerPrintInfo("Returning NULL!");
			destroyArray(ret);
			return NULL;
		}

	}
	for(i = 0;i<ret->d;i++)
	{
		int j = 0;
		for(;j<ret->n;j++)
			ret->indexOrdPerDim[i][j] = j;
		coorToCompare = i;
		qsort(ret->indexOrdPerDim[i],ret->n,sizeof(int),comp);
	}
	return ret;
}

KDArray** split(KDArray* KDA, int coor)
{
	if(KDA == NULL || KDA->n<2)
		return NULL;
	int size1 = (KDA->n+1)/2;
	int size2 = KDA->n - size1;
	KDArray** ret = (KDArray**)malloc(2*sizeof(KDArray*));
	if(ret == NULL)
	{
		spLoggerPrintError("Error while trying to allocating data for KDArray",__FILE__,__func__,__LINE__);
		spLoggerPrintInfo("Returning NULL!");
		return NULL;
	}
	ret[0] = (KDArray*)malloc(sizeof(KDArray));
	if(ret[0] == NULL)
	{
		spLoggerPrintError("Error while trying to allocating data for KDArray",__FILE__,__func__,__LINE__);
		spLoggerPrintInfo("Returning NULL!");
		free(ret);
		return NULL;
	}
	ret[1] = (KDArray*)malloc(sizeof(KDArray));
	if(ret[1] == NULL)
	{
		spLoggerPrintError("Error while trying to allocating data for KDArray",__FILE__,__func__,__LINE__);
		spLoggerPrintInfo("Returning NULL!");
		destroyArray(ret[0]);
		free(ret);
		return NULL;
	}
	int d = KDA->d, n = KDA->n;
	SPPoint** points = KDA->pointsArray;
	int** indexOrd = KDA->indexOrdPerDim;
	ret[0]->pointsArray = points;
	ret[0]->n=size1;
	ret[0]->d = d;
	ret[0]->numofPoints = KDA->numofPoints;
	ret[1]->pointsArray = points;
	ret[1]->n=size2;
	ret[1]->d = d;
	ret[1]->numofPoints = KDA->numofPoints;
	ret[0]->indexOrdPerDim = (int**)malloc(d*sizeof(int*));
	ret[1]->indexOrdPerDim = (int**)malloc(d*sizeof(int*));
	if(ret[1]->indexOrdPerDim == NULL || ret[0]->indexOrdPerDim ==NULL)
	{
		spLoggerPrintError("Error while trying to allocating data for KDArray",__FILE__,__func__,__LINE__);
		spLoggerPrintInfo("Returning NULL!");
		destroyArray(ret[0]);
		destroyArray(ret[1]);
		free(ret);
		return NULL;
	}
	int i = 0 ;
	int leftIndexes[KDA->numofPoints];
	for(;i<n;i++){
		if(i<size1)
			leftIndexes[indexOrd[coor][i]] = 1;
		else
			leftIndexes[indexOrd[coor][i]] = 0;
	}
	//double leftBound = spPointGetAxisCoor(points[indexOrd[coor][size1-1]],coor);
	for(i=0;i<d;i++)
	{
		ret[0]->indexOrdPerDim[i] = (int*)malloc(sizeof(int)*size1);
		ret[1]->indexOrdPerDim[i] = (int*)malloc(sizeof(int)*size2);
	}
	for(i=0;i<d;i++)
	{
		if(ret[0]->indexOrdPerDim[i]==NULL || ret[1]->indexOrdPerDim[i] == NULL)
		{
			spLoggerPrintError("Error while trying to allocating data for KDArray",__FILE__,__func__,__LINE__);
			spLoggerPrintInfo("Returning NULL!");
			destroyArray(ret[0]);
			destroyArray(ret[1]);
			free(ret);
			return NULL;
		}
		int k1 = 0,k2 = 0,j = 0;
		for(;j<n;j++)
		{
			if(leftIndexes[indexOrd[i][j]] == 1)
			{
				ret[0]->indexOrdPerDim[i][k1] = indexOrd[i][j];
				k1++;
			}
			else
			{
				ret[1]->indexOrdPerDim[i][k2] = indexOrd[i][j];
				k2++;
			}
		}
	}
	return ret;
}



void destroyKDN(KDNode* KDN)
{
	if(KDN == NULL)
		return;
	if(KDN->Left!=NULL)
		destroyKDN(KDN->Left);
	if(KDN->Right!=NULL)
		destroyKDN(KDN->Right);
	if(KDN->Data!=NULL)
		spPointDestroy(KDN->Data);
	free(KDN);
}
KDNode* buildFromKDArray(KDArray* KDA,SPLIT_METHOD method, int lastCoor)
{
	if(KDA == NULL)
		return NULL;
	KDNode* ret = (KDNode*)malloc(sizeof(KDNode));
	if(ret==NULL)
	{
		spLoggerPrintError("Error while trying to allocating data for KDTree",__FILE__,__func__,__LINE__);
		spLoggerPrintInfo("Returning NULL!");
		return NULL;
	}
	if(KDA->n == 1)
	{
		ret->Dim = -1;
		ret->Val = -1;
		ret->Left=NULL;
		ret->Right =NULL;
		ret->Data = spPointCopy(KDA->pointsArray[KDA->indexOrdPerDim[0][0]]);
		if(ret->Data == NULL)
		{
			spLoggerPrintError("Error while trying to allocating data for KDTree",__FILE__,__func__,__LINE__);
			spLoggerPrintInfo("Returning NULL!");
			destroyKDN(ret);
			return NULL;
		}
		return ret;
	}
	ret->Data = NULL;
	SPPoint** points = KDA->pointsArray;
	int d = KDA->d, n = KDA->n;
	int** indexOrd= KDA->indexOrdPerDim;
	if(method == MAX_SPREAD )
	{
		ret->Dim = 0;
		double maxSpread = spPointGetAxisCoor(points[indexOrd[0][n-1]],0)-spPointGetAxisCoor(points[indexOrd[0][0]],0);
		int i =1;
		for(;i<d;i++)
		{
			double newSpread = spPointGetAxisCoor(points[indexOrd[i][n-1]],i)-spPointGetAxisCoor(points[indexOrd[i][0]],i);
			if(newSpread > maxSpread)
			{
				maxSpread = newSpread;
				ret->Dim = i;
			}
		}
	}
	else if(method == INCREMENTAL)
		ret->Dim = (lastCoor+1)%d;
	else
		ret->Dim = rand()%d;

	KDArray** splitArray = split(KDA,ret->Dim);

	if(splitArray == NULL)
	{
		destroyKDN(ret);
		return NULL;
	}
	ret->Val = spPointGetAxisCoor(points[splitArray[0]->indexOrdPerDim[ret->Dim][splitArray[0]->n-1]],ret->Dim);
	ret->Left = buildFromKDArray(splitArray[0],method,ret->Dim);
	ret->Right = buildFromKDArray(splitArray[1],method,ret->Dim);
	destroyArray(splitArray[0]);
	destroyArray(splitArray[1]);
	free(splitArray);

	if(ret->Left == NULL || ret->Right == NULL)
	{
		destroyKDN(ret);
		return NULL;
	}

	return ret;
}

KDNode* buildFromPoints(SPPoint** points, SPLIT_METHOD method, int Size)
{
	KDArray* KDA = init(points, Size);
	KDNode* ret = buildFromKDArray(KDA,method,-1);
	destroyArray(KDA);
	return ret;
}
bool isLeaf(KDNode* KDN)
{
	return KDN->Data!=NULL;
}
void KNNFromTree(KDNode* KDTree, SPPoint* pnt, SPBPQueue* ret)
{
	if(KDTree == NULL)
	{
		spLoggerPrintWarning("KDTree given for KNN is NULL",__FILE__,__func__,__LINE__);
		spLoggerPrintInfo("Finishing KNN!");
		return;
	}
	if(isLeaf(KDTree))
	{
		spBPQueueEnqueue(ret, spPointGetIndex(KDTree->Data),spPointL2SquaredDistance(KDTree->Data,pnt));
		return;
	}
	if(spPointGetAxisCoor(pnt,KDTree->Dim)<=KDTree->Val)
	{
		KNNFromTree(KDTree->Left,pnt,ret);
		double dist = spPointGetAxisCoor(pnt,KDTree->Dim) - KDTree->Val;
		if(!spBPQueueIsFull(ret) || dist*dist <= spBPQueueMaxValue(ret))
			KNNFromTree(KDTree->Right, pnt, ret);
	}
	else
	{
		KNNFromTree(KDTree->Right,pnt,ret);
		double dist = spPointGetAxisCoor(pnt,KDTree->Dim) - KDTree->Val;
		if(!spBPQueueIsFull(ret) || dist*dist <= spBPQueueMaxValue(ret))
			KNNFromTree(KDTree->Left, pnt, ret);
	}
}

SPBPQueue* KNN(SPPoint** points,SPPoint* pnt, int k, SPLIT_METHOD method, int size)
{
	SPBPQueue* ret = spBPQueueCreate(k);
	if(ret == NULL)
	{
	spLoggerPrintError("Error while trying to allocating data for Queue!",__FILE__,__func__,__LINE__);
	spLoggerPrintInfo("Returning NULL!");
	}
	KDNode* KDTree = buildFromPoints(points, method,size);
	if(KDTree == NULL)
	{
		spBPQueueDestroy(ret);
		return NULL;
	}
	KNNFromTree(KDTree, pnt, ret);
	destroyKDN(KDTree);
	return ret;
}
