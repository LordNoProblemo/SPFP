/*
 * main_aux.h
 *
 *  Created on: Apr 29, 2017
 *      Author: asaf
 */

#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_
#include "SPImageProc.h"
extern "C"
{
#include "SPConfig.h"
}

bool extractFeatures(SPConfig config, sp::ImageProc* imageProc);


bool fetchFeatures(SPConfig config, sp::ImageProc* imageProc, SPPoint*** features);

#endif /* MAIN_AUX_H_ */
