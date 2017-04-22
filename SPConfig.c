/*
 * SPConfig.c
 *
 *  Created on: Apr 6, 2017
 *      Author: nimro
 */
#include "SPConfig.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define errMes "File: %s \nLine: %d \nMessage: %s"
#define linErr "Invalid configuration line"
#define valErr "Invalid value - constraint not met"
#define parNotSetErr "Parameter %s is not set"
#define JPG ".jpg"
#define PNG ".png"
#define BMP ".bmp"
#define GIF ".gif"
#define isPCADim(x) ((10<=(x)) && ((x)<=28))
#define isPositive(x) ((x)>0)
#define isLegitLevel(x) (((x)>0)&&((x)<5))


struct sp_config_t
{
	char* spImagesDirectory;
	char* spImagesPrefix;
	char* spImagesSuffix;
	int spNumOfImages;
	int spPCADimension ;
	char* spPCAFilename ;
	int spNumOfFeatures ;
	bool spExtractionMode ;
	int spNumOfSimilarImages;
	SPLIT_METHOD spKDTreeSplitMethod;
	int spKNN ;
	bool spMinimalGUI;
	int spLoggerLevel;
	char* spLoggerFilename;
	bool extractSet;
	bool guiSet;
};

void SetDefaults(SPConfig conf)
{
	assert(conf != NULL);
	if(conf->extractSet == false)
		conf->spExtractionMode = true;
	if(conf->spKDTreeSplitMethod == -1)
		conf->spKDTreeSplitMethod = MAX_SPREAD;
	if(conf->spKNN == 0)
		conf->spKNN = 1;
	if(conf->guiSet== false)
		conf->spMinimalGUI = false;
	if(conf->spPCADimension==0)
		conf->spPCADimension = 20;
	if(conf->spPCAFilename == NULL)
		conf->spPCAFilename = "pca.yml";
	if(conf->spNumOfFeatures == 0)
		conf->spNumOfFeatures = 100;
	if(conf->spNumOfSimilarImages == 0)
		conf->spNumOfSimilarImages = 1;
	if(conf->spLoggerLevel == 0)
		conf->spLoggerLevel = 3;
	if(conf->spLoggerFilename == NULL)
		conf->spLoggerFilename = "stdout";
}

void trim(char* str)
{
	int len =1025;
	char ret[1025];
	int i = 0;
	while(i < len && str[i]==' ')
		i++;
	if(i==strlen(str))
		return;
	int j = strlen(str);
	while(str[j-1] == ' ' || str[j-1] =='\n')
		j--;
	int k = i;
	for(;k<j;k++)
		ret[k-i] = str[k];
	i=0;
	for(;i<1025;i++)
			str[i] = ret[i];

}

bool noWhiteSpace(const char* str)
{
	int len = strlen(str);
	if(len == 0)
		return false;
	int i = 0;
	for(;i<len;i++)
		if(str[i] == ' ')
			return false;
	return true;
}

bool isSizeStr(const char* str)
{
	int len = strlen(str);
		int i = 0;
		for(;i<len;i++)
			if(str[i] < '0' || str[i]>'9')
				return false;
	return true;
}
int splitWithEq(char* orig, char* pre, char* suf)
{
	int len = strlen(orig), i = 0;
	for(;orig[i] != '=' && i < len; i++);
	if(i == len)
		return -1;
	if(snprintf(pre,i+1,"%s",orig) < 0 || sscanf(&orig[i+1],"%s",suf) == EOF)
		return -1;
	return 0;
}
SP_CONFIG_MSG SetVariable(SPConfig conf, char* varName, char* val)
{
	if(strcmp(varName, "spImagesDirectory")==0)
	{
		if(!noWhiteSpace(val))
			return SP_CONFIG_INVALID_STRING;
		else
		{
			if(conf->spImagesDirectory != NULL)
				return SP_CONFIG_VARIABLE_ALREADY_SET;
			conf->spImagesDirectory = (char*)malloc(1025);
			if(conf->spImagesDirectory == NULL)
				return SP_CONFIG_ALLOC_FAIL;
			sscanf(conf->spImagesDirectory,"%s",val);
			return SP_CONFIG_SUCCESS;
		}
	}

	if(strcmp(varName , "spImagesPrefix")==0)
	{
		if(!noWhiteSpace(val))
			return SP_CONFIG_INVALID_STRING;
		else
		{
			if(conf->spImagesPrefix != NULL)
				return SP_CONFIG_VARIABLE_ALREADY_SET;
			conf->spImagesPrefix = (char*)malloc(1025);
			if(conf->spImagesPrefix == NULL)
				return SP_CONFIG_ALLOC_FAIL;
			sscanf(conf->spImagesPrefix,"%s",val);
			return SP_CONFIG_SUCCESS;
		}
	}
	if(strcmp(varName, "spImagesSuffix")==0)
	{
		if(!noWhiteSpace(val))
			return SP_CONFIG_INVALID_STRING;
		else
		{
			if(conf->spImagesSuffix != NULL)
				return SP_CONFIG_VARIABLE_ALREADY_SET;
			conf->spImagesSuffix = (char*)malloc(1025);
			if(conf->spImagesSuffix == NULL)
				return SP_CONFIG_ALLOC_FAIL;
			sscanf(conf->spImagesSuffix,"%s",val);
			return SP_CONFIG_SUCCESS;
		}
	}
	if(strcmp(varName, "spImagesSuffix")==0)
	{
		if(!noWhiteSpace(val) || !(strcmp(val,JPG)==0||strcmp(val,PNG)==0||strcmp(val,GIF)==0||strcmp(val,BMP)==0))
			return SP_CONFIG_INVALID_STRING;
		else
		{
			if(conf->spImagesSuffix != NULL)
				return SP_CONFIG_VARIABLE_ALREADY_SET;
			conf->spImagesSuffix = (char*)malloc(1025);
			if(conf->spImagesSuffix == NULL)
				return SP_CONFIG_ALLOC_FAIL;
			sscanf(conf->spImagesSuffix,"%s",val);
			return SP_CONFIG_SUCCESS;
		}
	}
	if(strcmp(varName, "spNumOfImages")==0)
	{
		if(!isSizeStr(val))
			return SP_CONFIG_INVALID_INTEGER;
		else
		{
			if(conf->spNumOfImages > 0)
				return SP_CONFIG_VARIABLE_ALREADY_SET;
			int Val = atoi(val);
			if(!isPositive(Val))
				return SP_CONFIG_INVALID_INTEGER;
			conf->spNumOfImages = Val;
			return SP_CONFIG_SUCCESS;
		}
	}
	if(strcmp(varName, "spPCADimension")==0)
	{
		if(!isSizeStr(val))
			return SP_CONFIG_INVALID_INTEGER;
		else
		{
			if(conf->spPCADimension > 0)
				return SP_CONFIG_VARIABLE_ALREADY_SET;
			int Val = atoi(val);
			if(!isPCADim(Val))
				return SP_CONFIG_INVALID_INTEGER;
			conf->spPCADimension = Val;
			return SP_CONFIG_SUCCESS;
		}
	}
	if(strcmp(varName, "spPCAFilename")==0)
	{
		if(!noWhiteSpace(val))
			return SP_CONFIG_INVALID_STRING;
		else
		{
			if(conf->spPCAFilename != NULL)
				return SP_CONFIG_VARIABLE_ALREADY_SET;
			conf->spPCAFilename = (char*)malloc(1025);
			if(conf->spPCAFilename == NULL)
				return SP_CONFIG_ALLOC_FAIL;
			sscanf(conf->spPCAFilename,"%s",val);
			return SP_CONFIG_SUCCESS;
		}
	}
	if(strcmp(varName, "spNumOfFeatures")==0)
	{
		if(!isSizeStr(val))
			return SP_CONFIG_INVALID_INTEGER;
		else
		{
			if(conf->spNumOfFeatures > 0)
				return SP_CONFIG_VARIABLE_ALREADY_SET;
			int Val = atoi(val);
			if(!isPositive(Val))
				return SP_CONFIG_INVALID_INTEGER;
			conf->spNumOfFeatures = Val;
			return SP_CONFIG_SUCCESS;
		}
	}
	if(strcmp(varName, "spExtractionMode")==0)
	{
		if(!noWhiteSpace(val))
				return SP_CONFIG_INVALID_STRING;
		if(conf->extractSet)
			return SP_CONFIG_VARIABLE_ALREADY_SET;
		if(!(strcmp(val,"false")==0||strcmp(val,"true")==0))
			return SP_CONFIG_INVALID_BOOL;
		conf->extractSet = true;
		conf->spExtractionMode = (strcmp(val,"true")==0);
		return SP_CONFIG_SUCCESS;
	}
	if(strcmp(varName, "spNumOfSimilarImages")==0)
	{
		if(!isSizeStr(val))
			return SP_CONFIG_INVALID_INTEGER;
		else
		{
			if(conf->spNumOfSimilarImages > 0)
				return SP_CONFIG_VARIABLE_ALREADY_SET;
			int Val = atoi(val);
			if(!isPositive(Val))
				return SP_CONFIG_INVALID_INTEGER;
			conf->spNumOfSimilarImages = Val;
			return SP_CONFIG_SUCCESS;
		}
	}
	if(strcmp(varName, "spKDTreeSplitMethod")==0)
	{
		if(!noWhiteSpace(val)||!(strcmp(val,"RANDOM") ==0|| strcmp(val,"MAX_SPREAD")==0 || strcmp(val,"INCREMENTAL") ==0))
			return SP_CONFIG_INVALID_STRING;
		if(conf->spKDTreeSplitMethod != -1)
			return SP_CONFIG_VARIABLE_ALREADY_SET;

			if(strcmp(val,"RANDOM")==0)
				conf->spKDTreeSplitMethod = RANDOM;

			if(strcmp(val, "MAX_SPREAD")==0)
				conf->spKDTreeSplitMethod = MAX_SPREAD;

			if(strcmp(val, "INCREMENTAL")==0)
				conf->spKDTreeSplitMethod = INCREMENTAL;
		return SP_CONFIG_SUCCESS;

	}
	if(strcmp(varName, "spKNN")==0)
	{
		if(!isSizeStr(val))
			return SP_CONFIG_INVALID_INTEGER;
		else
		{
			if(conf->spKNN > 0)
				return SP_CONFIG_VARIABLE_ALREADY_SET;
			int Val = atoi(val);
			if(!isPositive(Val))
				return SP_CONFIG_INVALID_INTEGER;
			conf->spKNN = Val;
			return SP_CONFIG_SUCCESS;
		}
	}
	if(strcmp(varName, "spMinimalGUI")==0)
	{
		if(!noWhiteSpace(val))
				return SP_CONFIG_INVALID_STRING;
		if(conf->guiSet)
			return SP_CONFIG_VARIABLE_ALREADY_SET;
		if(!(strcmp(val,"false")==0||strcmp(val,"true")==0))
			return SP_CONFIG_INVALID_BOOL;
		conf->guiSet = true;
		conf->spMinimalGUI = (strcmp(val,"true")==0);
		return SP_CONFIG_SUCCESS;
	}
	if(strcmp(varName, "spLoggerLevel")==0)
	{
		if(!isSizeStr(val))
			return SP_CONFIG_INVALID_INTEGER;
		else
		{
			if(conf->spLoggerLevel > 0)
				return SP_CONFIG_VARIABLE_ALREADY_SET;
			int Val = atoi(val);
			if(!isLegitLevel(Val))
				return SP_CONFIG_INVALID_INTEGER;
			conf->spLoggerLevel = Val;
			return SP_CONFIG_SUCCESS;
		}
	}
	if(strcmp(varName, "spLoggerFilename")==0)
	{
		if(!noWhiteSpace(val))
			return SP_CONFIG_INVALID_STRING;
		else
		{
			if(conf->spLoggerFilename != NULL)
				return SP_CONFIG_VARIABLE_ALREADY_SET;
			conf->spLoggerFilename = (char*)malloc(1025);
			if(conf->spLoggerFilename == NULL)
				return SP_CONFIG_ALLOC_FAIL;
			sscanf(conf->spLoggerFilename,"%s",val);
			return SP_CONFIG_SUCCESS;
		}
	}
	return SP_CONFIG_NO_SUCH_VAR;
}

SP_CONFIG_MSG checkConfig(SPConfig conf,int lines,const char* filename)
{
	if(conf->spImagesDirectory == NULL)
	{
		char format[1025];
		sprintf(format,parNotSetErr,"spImagesDirectory");
		printf(errMes,filename,lines,format);
		return SP_CONFIG_MISSING_DIR;
	}
	if(conf->spImagesPrefix == NULL)
	{
		char format[1025];
		sprintf(format,parNotSetErr,"spImagesPrefix");
		printf(errMes,filename,lines,format);
		return SP_CONFIG_MISSING_PREFIX;
	}
	if(conf->spImagesSuffix == NULL)
	{
		char format[1025];
		sprintf(format,parNotSetErr,"spImagesSuffix");
		printf(errMes,filename,lines,format);
		return SP_CONFIG_MISSING_SUFFIX;
	}
	if(conf->spNumOfImages==0)
	{
		char format[1025];
		sprintf(format,parNotSetErr,"spNumOfImages");
		printf(errMes,filename,lines,format);
		return SP_CONFIG_MISSING_NUM_IMAGES;
	}
	return SP_CONFIG_SUCCESS;
}

SPConfig spConfigCreate(const char* filename, SP_CONFIG_MSG* msg)
{
	if(filename == NULL)
	{
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return NULL;
	}
	FILE* fp =fopen(filename,"r");
	if(fp == NULL)
	{
		*msg = SP_CONFIG_CANNOT_OPEN_FILE;
		return NULL;
	}
	char buffer[1025];
	char varName[1025];
	char val[1025];
	int lines = 0;
	SPConfig config = (SPConfig)malloc(sizeof(SPConfig));
	config->extractSet = false;
	config->guiSet=false;
	SP_CONFIG_MSG tempMsg;
	while(fgets(buffer,1025,fp)!=NULL)
	{
		lines++;
		trim(buffer);
		if(strcmp(buffer,"") ==0|| buffer[0] == '#')
			continue;
		else if(splitWithEq(buffer,varName,val) == -1)
		{
			printf(errMes,filename,lines,linErr);
			spConfigDestroy(config);
			*msg= SP_CONFIG_INVALID_LINE;
			return NULL;
		}
		trim(varName);
		trim(val);
		 tempMsg = SetVariable(config,varName,val);
		if(tempMsg != SP_CONFIG_SUCCESS)
		{
			spConfigDestroy(config);
			*msg = tempMsg;
			if(tempMsg != SP_CONFIG_NO_SUCH_VAR && tempMsg != SP_CONFIG_VARIABLE_ALREADY_SET)
				printf(errMes,filename,lines,valErr);
			return NULL;
		}
	}
	SetDefaults(config);
	tempMsg = checkConfig(config,lines, filename);
	if(tempMsg != SP_CONFIG_SUCCESS)
	{
		spConfigDestroy(config);
		*msg = tempMsg;
		return NULL;
	}
	fclose(fp);
	return config;
}


bool spConfigIsExtractionMode(const SPConfig config, SP_CONFIG_MSG* msg)
{
	assert(msg!=NULL);
	if(config == NULL)
	{
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return false;
	}
	*msg = SP_CONFIG_SUCCESS;
	return config->spExtractionMode;
}
bool spConfigMinimalGui(const SPConfig config, SP_CONFIG_MSG* msg)
{
	assert(msg!=NULL);
	if(config == NULL)
	{
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return false;
	}
	*msg = SP_CONFIG_SUCCESS;
	return config->spMinimalGUI;
}
int spConfigGetNumOfImages(const SPConfig config, SP_CONFIG_MSG* msg)
{
	assert(msg!=NULL);
	if(config == NULL)
	{
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return false;
	}
	*msg = SP_CONFIG_SUCCESS;
	return config->spNumOfImages;

}
int spConfigGetNumOfFeatures(const SPConfig config, SP_CONFIG_MSG* msg)
{
	assert(msg!=NULL);
	if(config == NULL)
	{
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return false;
	}
	*msg = SP_CONFIG_SUCCESS;
	return config->spNumOfFeatures;
}
int spConfigGetPCADim(const SPConfig config, SP_CONFIG_MSG* msg)
{
	assert(msg!=NULL);
	if(config == NULL)
	{
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return false;
	}
	*msg = SP_CONFIG_SUCCESS;
	return config->spPCADimension;
}
SP_CONFIG_MSG spConfigGetImagePath(char* imagePath, const SPConfig config, int index)
{
	if(imagePath == NULL || config == NULL)
		return SP_CONFIG_INVALID_ARGUMENT;
	if(index >= config->spNumOfImages)
		return SP_CONFIG_INDEX_OUT_OF_RANGE;
	sprintf(imagePath,"%s%d%s",config->spImagesPrefix,index,config->spImagesSuffix);
	return SP_CONFIG_SUCCESS;

}
SP_CONFIG_MSG spConfigGetPCAPath(char* pcaPath, const SPConfig config)
{
	if(pcaPath == NULL || config == NULL)
		return SP_CONFIG_INVALID_ARGUMENT;
	sprintf(pcaPath,"%s/%s",config->spImagesDirectory,config->spPCAFilename);
	return SP_CONFIG_SUCCESS;
}
void spConfigDestroy(SPConfig config)
{
	if(config == NULL)
		return;
	if(config->spImagesDirectory!=NULL)
		free(config->spImagesDirectory);
	if(config->spImagesPrefix!=NULL)
		free(config->spImagesPrefix);
	if(config->spImagesSuffix!=NULL)
		free(config->spImagesSuffix);
	if(config->spLoggerFilename!=NULL)
		free(config->spLoggerFilename);
	if(config->spPCAFilename!=NULL)
		free(config->spPCAFilename);
	free(config);
}
int spConfigGetNumOfSimImages(const SPConfig config, SP_CONFIG_MSG* msg)
{
	assert(msg!=NULL);
	if(config == NULL)
	{
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return false;
	}
	*msg = SP_CONFIG_SUCCESS;
	return config->spNumOfSimilarImages;

}
int spConfigGetKNN(const SPConfig config, SP_CONFIG_MSG* msg)
{
	assert(msg!=NULL);
	if(config == NULL)
	{
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return false;
	}
	*msg = SP_CONFIG_SUCCESS;
	return config->spKNN;

}
int spConfigGetLoggerLevel(const SPConfig config, SP_CONFIG_MSG* msg)
{
	assert(msg!=NULL);
	if(config == NULL)
	{
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return false;
	}
	*msg = SP_CONFIG_SUCCESS;
	return config->spLoggerLevel;

}
SP_CONFIG_MSG spConfigGetLoggerPath(char* loggerPath, const SPConfig config)
{
	if(loggerPath == NULL || config == NULL)
		return SP_CONFIG_INVALID_ARGUMENT;
	if(strcmp(config->spLoggerFilename,  "stdout")==0)
		sprintf(loggerPath,"%s",config->spLoggerFilename);
	else
		sprintf(loggerPath,"%s/%s",config->spImagesDirectory,config->spLoggerFilename);
	return SP_CONFIG_SUCCESS;
}
