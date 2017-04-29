#include "SPImageProc.h"
#include <string.h>


bool writeFeatures(char* imageName, SPPoint** features, int numOfFeatures)
{
	char extension[] = "feats";
	strcpy(imageName + strlen(imageName) - 3, extension);
	spLoggerPrintDebug(imageName, __FILE__, __FUNCTION__, __LINE__);
	FILE* f = fopen(imageName, "wb");
	if(f == NULL)
		return false;
	int outnum = fwrite(&numOfFeatures, sizeof(int), 1, f);
	if (outnum < 1)
	{
		fclose(f);
		return false;
	}
	for (int i = 0; i < numOfFeatures; i++)
	{
		SPPoint* cur = features[i];
		int head[2];
		head[0] = spPointGetDimension(cur);
		head[1] = spPointGetIndex(cur);
		outnum = fwrite(head, sizeof(int), 2, f);
		if (outnum < 2)
		{
			fclose(f);
			return false;
		}
		double * data = (double*) malloc(sizeof(double) * head[0]);
		if (data == NULL)
		{
			fclose(f);
			return false;
		}
		for (int axis = 0; axis < head[0]; axis++)
			data[axis] = spPointGetAxisCoor(cur, axis);
		outnum = fwrite(data, sizeof(double), head[0], f);
		free(data);
		if (outnum < head[0])
		{
			fclose(f);
			return false;
		}
	}
	fclose(f);
	return true;
}

bool extractFeatures(SPConfig config, sp::ImageProc* imageProc)
{
	if (config == NULL)
	{
		spLoggerPrintError("Config is null!", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}
	if (imageProc == NULL)
	{
		spLoggerPrintError("imageProc is null!", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}
	SP_CONFIG_MSG cmsg;
	int numOfImages = spConfigGetNumOfImages(config, &cmsg);
	if (numOfImages <= 0)
	{
		spLoggerPrintError("numOfImages must be positive!", __FILE__,
				__FUNCTION__, __LINE__);
		return false;
	}
	int numOfFeatrues = spConfigGetNumOfFeatures(config, &cmsg);
	if (numOfFeatrues <= 0)
	{
		spLoggerPrintError("numOfFeatrues must be positive!", __FILE__,
				__FUNCTION__, __LINE__);
		return false;
	}
	char imagePath[1027]; //the extension ".feats" needs 2 more chars
	for (int i = 0; i < numOfImages; i++)
	{
		cmsg = spConfigGetImagePath(imagePath, config, i);
		if (cmsg == SP_CONFIG_INDEX_OUT_OF_RANGE)
		{
			spLoggerPrintError("Too much images requested", __FILE__,
					__FUNCTION__, __LINE__);
			return false;
		}
		int cfeats = numOfFeatrues;
		SPPoint** features = imageProc->getImageFeatures(imagePath, i, &cfeats);
		bool wrote = writeFeatures(imagePath, features, cfeats);
		for(int j = 0; j < cfeats; j++)
			spPointDestroy(features[j]);
		free(features);
		if(!wrote)
		{
			spLoggerPrintError("Error in writing features", __FILE__,__FUNCTION__, __LINE__);
			return false;
		}
	}

	return true;
}

int readFeatures(char* imageName, SPPoint*** features)
{
	char extension[] = "feats";
	strcpy(imageName + strlen(imageName) - 3, extension);
	spLoggerPrintDebug(imageName, __FILE__, __FUNCTION__, __LINE__);
	FILE* f = fopen(imageName, "rb");
	if(f==NULL)
		return -1;
	int numOfFeatures = 0;
	int innum = fread(&numOfFeatures, sizeof(int), 1, f);
	if (innum < 1|| numOfFeatures==0)
	{
		fclose(f);
		return -1;
	}
	*features = (SPPoint**)malloc(sizeof(SPPoint*) * numOfFeatures);
	if(*features == NULL)
	{
		fclose(f);
		return -1;
	}
	for (int i = 0; i < numOfFeatures; i++)
	{

		int head[2];
		innum = fread(head, sizeof(int), 2, f);
		if (innum < 2)
		{
			fclose(f);
			return -1;
		}
		double * data = (double*) malloc(sizeof(double) * head[0]);
		if (data == NULL)
		{
			fclose(f);
			return -1;
		}
		innum = fread(data, sizeof(double), head[0], f);
		free(data);
		if (innum < head[0])
		{
			fclose(f);
			return -1;
		}
		*features[i] = spPointCreate(data, head[0], head[1]);
	}
	fclose(f);
	return numOfFeatures;
}

bool fetchFeatures(SPConfig config, sp::ImageProc* imageProc, SPPoint*** features)
{
	if (config == NULL)
	{
		spLoggerPrintError("Config is null!", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}
	if (imageProc == NULL)
	{
		spLoggerPrintError("imageProc is null!", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}
	if (features == NULL)
	{
		spLoggerPrintError("features is null!", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}
	SP_CONFIG_MSG cmsg;
	int numOfImages = spConfigGetNumOfImages(config, &cmsg);
	if (numOfImages <= 0)
	{
		spLoggerPrintError("numOfImages must be positive!", __FILE__,
				__FUNCTION__, __LINE__);
		return false;
	}
	char imagePath[1027]; //the extension ".feats" needs 2 more chars
	for (int i = 0; i < numOfImages; i++)
	{
		cmsg = spConfigGetImagePath(imagePath, config, i);
		if (cmsg == SP_CONFIG_INDEX_OUT_OF_RANGE)
		{
			spLoggerPrintError("Too much images requested", __FILE__,
					__FUNCTION__, __LINE__);
			return false;
		}
		SPPoint** cfeatures = features[i];
		int numOfFeaures = readFeatures(imagePath, &cfeatures);
		if(numOfFeaures == -1)
		{
			spLoggerPrintError("Error in reading features", __FILE__,__FUNCTION__, __LINE__);
			return false;
		}
	}

	return true;
}
