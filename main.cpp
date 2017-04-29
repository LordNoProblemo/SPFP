#include <cstring>
#include <cstdio>
#include "SPImageProc.h"
#include "main_aux.h"
extern "C"
{
#include "SPConfig.h"
}

int main(int argc, char** argv)
{
	if((argc != 1&&argc != 3) || (argc == 3 &&strcmp(argv[1], "-c")!=0))
	{
		printf("Invalid command line : use -c <config_filename>\n");
		exit(1);
	}
	char * configfile;
	if(argc == 1)
		configfile = (char*) "spcbir.config";
	else
		configfile = argv[2];
	SP_CONFIG_MSG msg;
	SPConfig config =  spConfigCreate(configfile, &msg);
	if(config == NULL)
	{
		printf("The configuration file %s couldn’t be open\n", configfile);
		exit(1);
	}
	char logger_path[1025];
	msg = spConfigGetLoggerPath(logger_path, config);
	SP_LOGGER_MSG lmsg = spLoggerCreate(logger_path, (SP_LOGGER_LEVEL)spConfigGetLoggerLevel(config, &msg));
	if(lmsg != SP_LOGGER_SUCCESS)
	{
		printf("Logger init failed!");
		exit(1);
	}
	lmsg = spLoggerPrintDebug("Logger initialized", __FILE__, __func__, __LINE__);
	sp::ImageProc* imageProc =new sp::ImageProc(config);
	if(spConfigIsExtractionMode(config, &msg))
	{
		bool extraction = extractFeatures(config, imageProc);
		if(!extraction)
		{
			spLoggerPrintError("Extraction Failed!", __FILE__, __func__, __LINE__);
			spLoggerDestroy();
			delete imageProc;
			exit(1);
		}
	}
	spLoggerDestroy();
	delete imageProc;
}
