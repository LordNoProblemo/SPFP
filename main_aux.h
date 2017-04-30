/*
 * main_aux.h
 *
 *  Created on: Apr 29, 2017
 *      Author: asaf
 */

#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_
#include "SPImageProc.h"
extern "C"{
#include "SPConfig.h"
}

void freeFreatures(SPConfig config, SPPoint*** features, int* nFeatures);

bool extractFeatures(SPConfig config, sp::ImageProc* imageProc, SPPoint**** features, int** nFeatures);


bool fetchFeatures(SPConfig config, sp::ImageProc* imageProc, SPPoint**** features, int** nFeatures);

#endif /* MAIN_AUX_H_ */
