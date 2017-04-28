/*
 * sp_config_unit_test.c
 *
 *  Created on: Apr 27, 2017
 *      Author: nimro
 */

#include "../SPConfig.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "unit_test_util.h"

static bool checkValidConfig()
{
	SP_CONFIG_MSG msg;
	SPConfig con = spConfigCreate("ValidConfig.txt",&msg);
	if(con == NULL)
		return false;
	if(msg != SP_CONFIG_SUCCESS)
	{
		spConfigDestroy(con);
		return false;
	}
	char* Path = (char*)malloc(1025);
	if(Path == NULL)
	{
		spConfigDestroy(con);
		return false;
	}
	msg = spConfigGetPCAPath(Path,con);
	if(msg != SP_CONFIG_SUCCESS || strcmp(Path,"./images/pca.yml")!= 0)
	{
		spConfigDestroy(con);
		return false;
	}
	int level = spConfigGetLoggerLevel(con,&msg);
	if(msg != SP_CONFIG_SUCCESS || level != 3)
	{
		spConfigDestroy(con);
		return false;
	}
	msg = spConfigGetLoggerPath(Path,con);
	if(msg != SP_CONFIG_SUCCESS || strcmp(Path,"stdout")!=0)
	{
		spConfigDestroy(con);
		return false;
	}
	spConfigDestroy(con);
	return true;
}
static bool testInvalidConfigs()
{
	SPConfig con;
	SP_CONFIG_MSG msg;
	con = spConfigCreate("invalidIntConfig.txt",&msg);
	if(con != NULL)
	{
		spConfigDestroy(con);
		return false;
	}
	if(msg != SP_CONFIG_INVALID_INTEGER)
	{
		return false;
	}
	con = spConfigCreate("InValidSpaceConfig.txt",&msg);
	if(con != NULL)
	{
		spConfigDestroy(con);
		return false;
	}
	if(msg != SP_CONFIG_INVALID_STRING)
	{
		return false;
	}
	con = spConfigCreate("MissingConfig.txt",&msg);
	if(con != NULL)
	{
		spConfigDestroy(con);
		return false;
	}
	if(msg != SP_CONFIG_MISSING_DIR)
	{
		return false;
	}
	return true;
}
int main() {
	RUN_TEST(checkValidConfig);
	RUN_TEST(testInvalidConfigs);
	return 0;
}
