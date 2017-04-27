/*
 * KNN.h
 *
 *  Created on: Apr 16, 2017
 *      Author: nimro
 */

#include "SPBPriorityQueue.h"
#include "SPPoint.h"
#include "SPConfig.h"
/**SPBPQueue KNN(SPPoint** points,SPPoint* pnt, int k)
 * @param points = list of points
 * @param pnt = our point
 * @param k = number of neighbors
 * @param method = the spliting method for the KDTree
 * @param size = Amount of points in param points
 * @return A SPBQueue holding the k nearest neigbors to pnt in points if no errors, else NULL
 */
SPBPQueue* KNN(SPPoint** points,SPPoint* pnt, int k, SPLIT_METHOD method, int size);
