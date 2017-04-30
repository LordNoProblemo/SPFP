#include "SPLogger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//File open mode
#define SP_LOGGER_OPEN_MODE "w"

typedef enum {false=0, true=1} bool;
// Global variable holding the logger
SPLogger logger = NULL;

struct sp_logger_t {
	FILE* outputChannel; //The logger file
	bool isStdOut; //Indicates if the logger is stdout
	SP_LOGGER_LEVEL level; //Indicates the level
};

SP_LOGGER_MSG spLoggerCreate(const char* filename, SP_LOGGER_LEVEL level) {
	if (logger != NULL) { //Already defined
		return SP_LOGGER_DEFINED;
	}
	logger = (SPLogger) malloc(sizeof(*logger));
	if (logger == NULL) { //Allocation failure
		return SP_LOGGER_OUT_OF_MEMORY;
	}
	logger->level = level; //Set the level of the logger
	if (filename == NULL || strcmp("stdout", filename)==0) { //In case the filename is not set use stdout
		logger->outputChannel = stdout;
		logger->isStdOut = true;
	} else { //Otherwise open the file in write mode
		logger->outputChannel = fopen(filename, SP_LOGGER_OPEN_MODE);
		if (logger->outputChannel == NULL) { //Open failed
			free(logger);
			logger = NULL;
			return SP_LOGGER_CANNOT_OPEN_FILE;
		}
		logger->isStdOut = false;
	}
	return SP_LOGGER_SUCCESS;
}

void spLoggerDestroy() {
	if (!logger) {
		return;
	}
	if (!logger->isStdOut) {//Close file only if not stdout
		fclose(logger->outputChannel);
	}
	free(logger);//free allocation
	logger = NULL;
}
/*
 * Used to print errors, warnings, and debug messages
 */

SP_LOGGER_MSG spLoggerPrint(const char* msg, const char* file,
		const char* function, const int line, const char * type)
{
	if(logger == NULL)
		return SP_LOGGER_UNDIFINED;
	char * whole_msg= (char*) malloc(strlen(msg)+strlen(file)+strlen(function)+1000); //allocate enough for output
	if (whole_msg==NULL)
		return SP_LOGGER_OUT_OF_MEMORY;
	sprintf(whole_msg,"---%s---\n- file: %s\n- function: %s\n- line: %d\n- message: %s", type, file, function, line, msg);
	SP_LOGGER_MSG m = spLoggerPrintMsg(whole_msg);
	free(whole_msg);
	return m;
}

SP_LOGGER_MSG spLoggerPrintMsg(const char * msg)
{

	if(logger == NULL)
		return SP_LOGGER_UNDIFINED;
	unsigned int out_size = fwrite(msg, 1,strlen(msg), logger->outputChannel);
	if(out_size < strlen(msg))
		return SP_LOGGER_WRITE_FAIL;
	out_size = fwrite("\n", 1, 1, logger->outputChannel);
	if(out_size < 1)
			return SP_LOGGER_WRITE_FAIL;
	return SP_LOGGER_SUCCESS;
}

SP_LOGGER_MSG spLoggerPrintError(const char* msg, const char* file,
		const char* function, const int line)
{
	if(logger == NULL)
		return SP_LOGGER_UNDIFINED;
	if(msg==NULL || file == NULL || function == NULL || line < 0)
		return SP_LOGGER_INVALID_ARGUMENT;
	return spLoggerPrint(msg, file, function, line, "ERROR");
}

SP_LOGGER_MSG spLoggerPrintWarning(const char* msg, const char* file,
		const char* function, const int line)
{
	if(logger == NULL)
		return SP_LOGGER_UNDIFINED;
	if(msg==NULL || file == NULL || function == NULL || line < 0)
		return SP_LOGGER_INVALID_ARGUMENT;
	if(logger->level == SP_LOGGER_ERROR_LEVEL)
		return SP_LOGGER_SUCCESS;
	return spLoggerPrint(msg, file, function, line, "WARNING");
}


SP_LOGGER_MSG spLoggerPrintDebug(const char* msg, const char* file,
		const char* function, const int line)
{
	if(logger == NULL)
		return SP_LOGGER_UNDIFINED;
	if(msg==NULL || file == NULL || function == NULL || line < 0)
		return SP_LOGGER_INVALID_ARGUMENT;
	if(logger->level !=SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL)
		return SP_LOGGER_SUCCESS;
	return spLoggerPrint(msg, file, function, line, "DEBUG");
}

SP_LOGGER_MSG spLoggerPrintInfo(const char* msg)
{
	if(logger == NULL)
		return SP_LOGGER_UNDIFINED;
	if(logger->level <2)
		return SP_LOGGER_SUCCESS;
	char * whole_msg= (char*) malloc(strlen(msg)+100); //allocate enough for output
	if (whole_msg==NULL)
		return SP_LOGGER_OUT_OF_MEMORY;
	sprintf(whole_msg,"---INFO---\n- message: %s", msg);
	SP_LOGGER_MSG m = spLoggerPrintMsg(whole_msg);
	free(whole_msg);
	return m;
}
