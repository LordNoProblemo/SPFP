/*
 * sp_KD_tree_and_array_test.c
 *
 *  Created on: Apr 27, 2017
 *      Author: nimro
 */

#include "../KDTree.c"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "unit_test_util.h"

void destroyPointsArray(SPPoint** list, int size)
{
	if(list == NULL)
		return;
	int i =0;
	for(;i<size;i++)
		if(list[i]!=NULL)
			spPointDestroy(list[i]);
	free(list);
}
static bool TestKDASplitAndInit()
{
	SPPoint **points = malloc(5*sizeof(SPPoint*));
	if(points == NULL)
		return false;
	double data[3] ={0,1,0};
	points[0] = spPointCreate(data,3,0);
	if(points[0] == NULL)
	{
		destroyPointsArray(points,5);
		return false;
	}
	data[2]=1;
	data[1] = 0;
	points[1] = spPointCreate(data,3,1);
	if(points[1] == NULL)
	{
		destroyPointsArray(points,5);
		return false;
	}
	data[1]=-2;
	points[2] = spPointCreate(data,3,2);
	if(points[2] == NULL)
	{
		destroyPointsArray(points,5);
		return false;
	}
	data[1]=-1;
	data[0]=3;
	points[3] = spPointCreate(data,3,3);
	if(points[3] == NULL)
	{
		destroyPointsArray(points,5);
		return false;
	}
	data[1]=3;
	points[4] = spPointCreate(data,3,4);
	if(points[4] == NULL)
	{
		destroyPointsArray(points,5);
		return false;
	}
	//(0,1,0)-0,(0,0,1)-1,(0,-2,1)-2,(3,-1,1)-3,(3,3,1)-4
	KDArray* KDA = init(points,5);
	if(KDA == NULL)
	{
		destroyPointsArray(points,5);
		return false;
	}
	int r = 0,c=0;
	for(;r<3;r++)
	{
		c = 0;
		for(;c<4;c++)
		{
			if(spPointGetAxisCoor(points[KDA->indexOrdPerDim[r][c]],r)>spPointGetAxisCoor(points[KDA->indexOrdPerDim[r][c+1]],r))
			{
				destroyArray(KDA);
				destroyPointsArray(points,5);
				return false;
			}
		}
	}
	KDArray** Splited = split(KDA,1);
	if(Splited == NULL)
	{
		destroyArray(KDA);
		destroyPointsArray(points,5);
		return false;
	}
	if(KDA->d!=3 || Splited[0]->n != 3 || Splited[0]->numofPoints != 5)
	{
		destroyArray(Splited[0]);
		destroyArray(Splited[1]);
		free(Splited);
		destroyArray(KDA);
		destroyPointsArray(points,5);
		return false;
	}


	int* arrLeft = Splited[0]->indexOrdPerDim[1];
	int* arrRight = Splited[1]->indexOrdPerDim[1];
	if(arrLeft[0] != 2 || arrLeft[1] != 3 || arrLeft[1]== 0 )
	{
		destroyArray(Splited[0]);
		destroyArray(Splited[1]);
		free(Splited);
		destroyArray(KDA);
		destroyPointsArray(points,5);
		return false;
	}
	if(arrRight[0] != 0 || arrRight[1] != 4 )
	{
		destroyArray(Splited[0]);
		destroyArray(Splited[1]);
		free(Splited);
		destroyArray(KDA);
		destroyPointsArray(points,5);
		return false;
	}
	int i =0,j = 0;
	for(;i<3;i++)
	{
		for(;j<3;j++)
		{
			if(Splited[0]->indexOrdPerDim[i][j] != 2 && Splited[0]->indexOrdPerDim[i][j] != 3 && Splited[0]->indexOrdPerDim[i][j] != 0)
			{
				destroyArray(Splited[0]);
				destroyArray(Splited[1]);
				free(Splited);
				destroyArray(KDA);
				destroyPointsArray(points,5);
				return false;
			}
		}
		j = 0;
		for(;j<2;j++)
		{
			if(Splited[1]->indexOrdPerDim[i][j] != 4 && Splited[1]->indexOrdPerDim[i][j] != 4 )
			{
				destroyArray(Splited[0]);
				destroyArray(Splited[1]);
				free(Splited);
				destroyArray(KDA);
				destroyPointsArray(points,5);
				return false;
			}
		}
		j = 0;
	}
	destroyArray(Splited[0]);
	destroyArray(Splited[1]);
	free(Splited);
	destroyArray(KDA);
	destroyPointsArray(points,5);
	return true;
}

static bool TestKNN()
{
	SPPoint **points = malloc(5*sizeof(SPPoint*));
	if(points == NULL)
		return false;
	double data[3] ={0,1,0};
	points[0] = spPointCreate(data,3,0);
	if(points[0] == NULL)
	{
		destroyPointsArray(points,5);
		return false;
	}
	data[2]=1;
	data[1] = 0;
	points[1] = spPointCreate(data,3,1);
	if(points[1] == NULL)
	{
		destroyPointsArray(points,5);
		return false;
	}
	data[1]=-2;
	points[2] = spPointCreate(data,3,2);
	if(points[2] == NULL)
	{
		destroyPointsArray(points,5);
		return false;
	}
	data[1]=-1;
	data[0]=3;
	points[3] = spPointCreate(data,3,3);
	if(points[3] == NULL)
	{
		destroyPointsArray(points,5);
		return false;
	}
	data[1]=3;
	points[4] = spPointCreate(data,3,4);
	if(points[4] == NULL)
	{
		destroyPointsArray(points,5);
		return false;
	}
	//(0,1,0)-0,(0,0,1)-1,(0,-2,1)-2,(3,-1,1)-3,(3,3,1)-4
	data[0]=3;
	data[1]=3;
	data[2] = 3;
	SPPoint* pnt = spPointCreate(data,3,5);
	if(pnt == NULL)
	{
		spPointDestroy(pnt);
		destroyPointsArray(points,5);
		return false;
	}
	SPBPQueue* nearest3 = KNN(points,pnt,3,RANDOM,5);
	if(nearest3 == NULL)
	{
		spPointDestroy(pnt);
		destroyPointsArray(points,5);
		return false;
	}
	BPQueueElement peek;
	spBPQueuePeek(nearest3,&peek);
	if(peek.index != 4)
	{
		spBPQueueDestroy(nearest3);
		spPointDestroy(pnt);
		destroyPointsArray(points,5);
		return false;
	}
	spBPQueueDequeue(nearest3);
	spBPQueuePeek(nearest3,&peek);
	if(peek.index != 3)
	{
		spBPQueueDestroy(nearest3);
		spPointDestroy(pnt);
		destroyPointsArray(points,5);
		return false;
	}
	spBPQueueDequeue(nearest3);
	spBPQueuePeek(nearest3,&peek);
	if(peek.index != 0 && peek.index != 1 )
	{
		spBPQueueDestroy(nearest3);
		spPointDestroy(pnt);
		destroyPointsArray(points,5);
		return false;
	}
	spBPQueueDestroy(nearest3);
	spPointDestroy(pnt);
	destroyPointsArray(points,5);
	return true;
}
int main() {
	RUN_TEST(TestKDASplitAndInit);
	RUN_TEST(TestKNN);
	return 0;
}
