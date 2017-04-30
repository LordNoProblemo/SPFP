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
#include <ctype.h>
#define errMes "File: %s \nLine: %d \nMessage: %s\n"
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
	char spImagesDirectory[1025];
	char spImagesPrefix[1025];
	char spImagesSuffix[1025];
	int spNumOfImages;
	int spPCADimension ;
	char spPCAFilename[1025];
	int spNumOfFeatures ;
	bool spExtractionMode ;
	int spNumOfSimilarImages;
	SPLIT_METHOD spKDTreeSplitMethod;
	int spKNN ;
	bool spMinimalGUI;
	int spLoggerLevel;
	char spLoggerFilename[1025];
	bool extractSet;
	bool guiSet;
};

void SetDefaults(SPConfig conf)
{
	assert(conf != NULL);
	if(conf->extractSet == false)
		conf->spExtractionMode = true;
	if(conf->spKDTreeSplitMethod < 0)
		conf->spKDTreeSplitMethod = MAX_SPREAD;
	if(conf->spKNN == 0)
		conf->spKNN = 1;
	if(conf->guiSet== false)
		conf->spMinimalGUI = false;
	if(conf->spPCADimension==0)
		conf->spPCADimension = 20;
	if(conf->spPCAFilename[0] == '\0')
	{
		sprintf(conf->spPCAFilename ,"%s", "pca.yml");
	}
	if(conf->spNumOfFeatures == 0)
		conf->spNumOfFeatures = 100;
	if(conf->spNumOfSimilarImages == 0)
		conf->spNumOfSimilarImages = 1;
	if(conf->spLoggerLevel == 0)
		conf->spLoggerLevel = 3;
	if(conf->spLoggerFilename[0] == '\0')
	{
		sprintf(conf->spLoggerFilename,"%s", "stdout");
	}
}
void setZeros(SPConfig con)
{
	memset(con->spImagesDirectory,'\0',1025);
	memset(con->spImagesPrefix,'\0',1025);
	memset(con->spImagesSuffix,'\0',1025);
	memset(con->spPCAFilename,'\0',1025);
	memset(con->spLoggerFilename,'\0',1025);
	con->extractSet = false;
	con->guiSet = false;
	con->spLoggerLevel = 0;
	con->spKNN = 0;
	con->spNumOfSimilarImages =0;
	con->spNumOfFeatures = 0;
	con->spPCADimension = 0;
	con ->spNumOfImages = 0;
}
void trim(char* str)
{
	unsigned int len =strlen(str);
	char ret[1025];
	unsigned int i = 0;
	for(;i<1025;i++)
		ret[i] = '\0';
	i = 0;
	while(i < len && str[i]==' ')
		i++;
	if(i==strlen(str))
		return;
	int j = strlen(str);
	while((str[j-1] == ' ' || str[j-1] =='\n' || str[j-1] =='\0')&&j>0)
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
	int len = 1025;
	while((str[len-1]==' '||str[len-1]=='\n' || str[len-1]=='\0') && len>0)
		len--;
	if(len == -1)
		return false;
	int i = 0;
	for(;i<len;i++)
		if(str[i] == ' ' || str[i] == '=' || str[i]=='\0')
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
	unsigned int len = strlen(orig), i = 0;
	for(;orig[i] != '=' && i < len; i++);
	if(i == len)
		return -1;
	char buffer[1025];
	if(sscanf(&orig[i+1],"%s %s",suf,buffer) == 2)
		return -2;
	if(snprintf(pre,i+1,"%s",orig) < 0 || sscanf(&orig[i+1],"%s",suf) == EOF)
		return -1;

	return i+1;
}
SP_CONFIG_MSG SetVariable(SPConfig conf, char* varName, char* val)
{
	if(strcmp(varName, "spImagesDirectory")==0)
	{
		if(!noWhiteSpace(val))
			return SP_CONFIG_INVALID_STRING;
		else
		{
			if(conf->spImagesDirectory[0]!='\0')
				return SP_CONFIG_VARIABLE_ALREADY_SET;
			sprintf(conf->spImagesDirectory,"%s",val);
			return SP_CONFIG_SUCCESS;
		}
	}

	if(strcmp(varName , "spImagesPrefix")==0)
	{
		if(!noWhiteSpace(val))
			return SP_CONFIG_INVALID_STRING;
		else
		{
			if(conf->spImagesPrefix[0] != '\0')
				return SP_CONFIG_VARIABLE_ALREADY_SET;
			sprintf(conf->spImagesPrefix,"%s",val);
			return SP_CONFIG_SUCCESS;
		}
	}
	if(strcmp(varName, "spImagesSuffix")==0)
	{
		if(!noWhiteSpace(val))
			return SP_CONFIG_INVALID_STRING;
		else
		{
			if(conf->spImagesSuffix[0] != '\0')
				return SP_CONFIG_VARIABLE_ALREADY_SET;
			sprintf(conf->spImagesSuffix,"%s",val);
			return SP_CONFIG_SUCCESS;
		}
	}
	if(strcmp(varName, "spImagesSuffix")==0)
	{
		if(!noWhiteSpace(val) || !(strcmp(val,JPG)==0||strcmp(val,PNG)==0||strcmp(val,GIF)==0||strcmp(val,BMP)==0))
			return SP_CONFIG_INVALID_STRING;
		else
		{
			if(conf->spImagesSuffix[0] != '\0')
				return SP_CONFIG_VARIABLE_ALREADY_SET;
			sprintf(conf->spImagesSuffix,"%s",val);
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
			if(conf->spPCAFilename[0] != '\0')
				return SP_CONFIG_VARIABLE_ALREADY_SET;
			sprintf(conf->spPCAFilename,"%s",val);
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
				return SP_CONFIG_INVALID_BOOL;
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
		if(conf->spKDTreeSplitMethod >= 0)
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
				return SP_CONFIG_INVALID_BOOL;
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
			if(conf->spLoggerFilename[0] != '\0')
				return SP_CONFIG_VARIABLE_ALREADY_SET;
			sprintf(conf->spLoggerFilename,"%s",val);
			return SP_CONFIG_SUCCESS;
		}
	}
	return SP_CONFIG_NO_SUCH_VAR;
}

SP_CONFIG_MSG checkConfig(SPConfig conf,int lines,const char* filename)
{
	if(conf->spImagesDirectory[0] == '\0')
	{
		char format[1025];
		sprintf(format,parNotSetErr,"spImagesDirectory");
		printf(errMes,filename,lines,format);
		return SP_CONFIG_MISSING_DIR;
	}
	if(conf->spImagesPrefix[0] == '\0')
	{
		char format[1025];
		sprintf(format,parNotSetErr,"spImagesPrefix");
		printf(errMes,filename,lines,format);
		return SP_CONFIG_MISSING_PREFIX;
	}
	if(conf->spImagesSuffix[0] == '\0')
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
		printf("Invalid command line : use -c <config_filename>\n");
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return NULL;
	}
	FILE* fp =fopen(filename,"r");
	if(fp == NULL)
	{
		*msg = SP_CONFIG_CANNOT_OPEN_FILE;
		if(strcmp(filename,"spcbir.config") != 0)
		{
			printf("The configuration file %s couldn't be open\n",filename);
		}
		else
			printf("The default configuration file spcbir.config couldn't be open\n");
		return NULL;
	}
	char buffer[1025];
	char varName[1025];
	char val[1025];
	int lines = 0;
	SPConfig config = (SPConfig)malloc(sizeof(struct sp_config_t));
	if(config == NULL)
	{
		*msg = SP_CONFIG_ALLOC_FAIL;
		fclose(fp);
		return NULL;
	}
	setZeros(config);
	config->extractSet = false;
	config->guiSet=false;
	SP_CONFIG_MSG tempMsg;
	while(fgets(buffer,1025,fp)!=NULL)
	{
		lines++;
		trim(buffer);
		if(isspace(buffer[0])!=0|| buffer[0] == '#' || strlen(buffer) == 0)
		{
			memset(buffer,'\0',1025);
			memset(varName,'\0',1025);
			memset(val,'\0',1025);
			continue;
		}
		int split = splitWithEq(buffer,varName,val);
		if(split == -1)
		{
			fclose(fp);
			printf(errMes,filename,lines,linErr);
			spConfigDestroy(config);
			*msg= SP_CONFIG_INVALID_LINE;
			return NULL;
		}
		if(split == -2)
		{
			fclose(fp);
			printf(errMes,filename,lines,valErr);
			spConfigDestroy(config);
			*msg= SP_CONFIG_INVALID_STRING;
			return NULL;
		}
		trim(varName);
		trim(val);
		tempMsg = SetVariable(config,varName,val);
		if(tempMsg != SP_CONFIG_SUCCESS)
		{
			spConfigDestroy(config);
			*msg = tempMsg;
			fclose(fp);
			if(tempMsg != SP_CONFIG_NO_SUCH_VAR && tempMsg != SP_CONFIG_VARIABLE_ALREADY_SET)
				printf(errMes,filename,lines,valErr);
			if(tempMsg == SP_CONFIG_INVALID_INTEGER)
				*msg = SP_CONFIG_INVALID_INTEGER;
			else if(tempMsg == SP_CONFIG_INVALID_STRING)
				*msg =  SP_CONFIG_INVALID_STRING;
			else if(tempMsg == SP_CONFIG_INVALID_BOOL)
				*msg =  SP_CONFIG_INVALID_BOOL;
			else if(tempMsg == SP_CONFIG_NO_SUCH_VAR)
				*msg =  SP_CONFIG_NO_SUCH_VAR;
			else
				*msg = SP_CONFIG_VARIABLE_ALREADY_SET;
			return NULL;
		}
		memset(buffer,'\0',1025);
		memset(varName,'\0',1025);
		memset(val,'\0',1025);
	}

	SetDefaults(config);
	*msg = checkConfig(config,lines, filename);
	if(*msg != SP_CONFIG_SUCCESS)
	{
		spConfigDestroy(config);
		fclose(fp);
		return NULL;
	}
	*msg = SP_CONFIG_SUCCESS;
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
	sprintf(imagePath,"%s%s%d%s",config->spImagesDirectory,config->spImagesPrefix,index,config->spImagesSuffix);
	return SP_CONFIG_SUCCESS;

}
SP_CONFIG_MSG spConfigGetPCAPath(char* pcaPath, const SPConfig config)
{
	if(pcaPath == NULL || config == NULL)
		return SP_CONFIG_INVALID_ARGUMENT;
	sprintf(pcaPath,"%s%s",config->spImagesDirectory,config->spPCAFilename);
	return SP_CONFIG_SUCCESS;
}
void spConfigDestroy(SPConfig config)
{
	if(config == NULL)
		return;
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
		sprintf(loggerPath,"%s","stdout");
	else
		sprintf(loggerPath,"%s/%s",config->spImagesDirectory,config->spLoggerFilename);
	return SP_CONFIG_SUCCESS;
}
SPLIT_METHOD spConfigGetMethod(const SPConfig config)
{
	return config->spKDTreeSplitMethod;
}
