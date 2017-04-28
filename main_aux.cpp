#include "SPImageProc.h"
#include <string.h>

bool writeFeatures(char* imageName, SPPoint** features, int NumOfFeatures)
{
	char extension[] = "feats";
	strcpy(imageName+strlen(imageName)-3, extension);
	spLoggerPrintDebug(imageName, __FILE__, __FUNCTION__, __LINE__);

}


bool extractFeatures(SPConfig config, sp::ImageProc* imageProc)
{
	if(config==NULL)
	{
		spLoggerPrintError("Config is null!", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}
	SP_CONFIG_MSG cmsg;
	int numOfImages = spConfigGetNumOfImages(config, &cmsg);
	if(numOfImages <=0)
	{
		spLoggerPrintError("numOfImages must be positive!", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}
	int numOfFeatrues = spConfigGetNumOfFeatures(config, &cmsg);
	if(numOfFeatrues <=0)
	{
		spLoggerPrintError("numOfFeatrues must be positive!", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}
	char imagePath[1027]; //the extension ".feats" needs 2 more chars
	for(int i = 0; i < numOfImages; i++)
	{
		cmsg = spConfigGetImagePath(imagePath, config, i);
		if(cmsg == SP_CONFIG_INDEX_OUT_OF_RANGE)
		{
			spLoggerPrintError("Too much images requested", __FILE__, __FUNCTION__, __LINE__);
			return false;
		}
		int cfeats = numOfFeatrues;
		SPPoint** features = imageProc->getImageFeatures(imagePath, i, &cfeats);
	}

	return true;
}

