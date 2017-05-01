#include "main_aux.h"
#include <cstring>
#include "KDTree.h"

int main(int argc, char** argv)
{
	if ((argc != 1 && argc != 3) || (argc == 3 && strcmp(argv[1], "-c") != 0))
	{
		printf("Invalid command line : use -c <config_filename>\n");
		exit(1);
	}
	char * configfile;
	if (argc == 1)
		configfile = (char*) "spcbir.config";
	else
		configfile = argv[2];
	SP_CONFIG_MSG msg;
	SPConfig config = spConfigCreate(configfile, &msg);
	if (config == NULL)
	{
		printf("The configuration file %s couldn’t be open\n", configfile);
		exit(1);
	}
	char logger_path[1025];
	msg = spConfigGetLoggerPath(logger_path, config);
	SP_LOGGER_LEVEL lvl = (SP_LOGGER_LEVEL) spConfigGetLoggerLevel(config,
			&msg);
	SP_LOGGER_MSG lmsg = spLoggerCreate(logger_path, lvl);
	if (lmsg != SP_LOGGER_SUCCESS)
	{
		printf("Logger init failed!");
		spConfigDestroy(config);
		exit(1);
	}
	lmsg = spLoggerPrintDebug("Logger initialized", __FILE__, __func__,
			__LINE__);
	sp::ImageProc* imageProc = new sp::ImageProc(config);
	SPPoint*** features;
	int* nFeatures;
	bool fetch;
	if (spConfigIsExtractionMode(config, &msg))
	{
		fetch = extractFeatures(config, imageProc, &features, &nFeatures);

	}
	else
	{
		fetch = fetchFeatures(config, imageProc, &features, &nFeatures);
	}

	if (!fetch)
	{
		spLoggerPrintError("Extraction Failed!", __FILE__, __func__, __LINE__);
		freeFreatures(config, features, nFeatures);
		spLoggerDestroy();
		spConfigDestroy(config);
		delete imageProc;
		exit(1);
	}

	KDNode* tree = TreeFromData(config, features, nFeatures);
	spLoggerPrintDebug("preprocess finished", __FILE__, __func__, __LINE__);
	if (tree == NULL)
	{
		spLoggerPrintError("Tree creation Failed!", __FILE__, __func__,
				__LINE__);
		freeFreatures(config, features, nFeatures);
		spLoggerDestroy();
		spConfigDestroy(config);
		delete imageProc;
		exit(1);
	}
	while (true)
	{
		char query_image[1025];
		printf("Please enter an image path:\n");
		scanf("%s", query_image);
		if (strcmp("<>", query_image) == 0)
			break;
		closestImages(config, imageProc, tree, query_image);
	}
	printf("Exiting...\n");
	destroyKDN(tree);
	freeFreatures(config, features, nFeatures);
	spConfigDestroy(config);
	spLoggerDestroy();
	delete imageProc;
}
