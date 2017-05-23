#pragma once
#include "file.h"
class Logger
{
public:
	Logger()
	{
		loggerFile = NULL;
	}

	Logger(const char *fileName)
	{
		loggerFile = new File(fileName);
	}

	static Logger &GetLogger()
	{
		static char fileName[] = "logger.log";
		static Logger logger = Logger(fileName);
		if (!File::Exist(fileName))
		{
			File f;
			f.Open(fileName, FileOpenMode::WRITENEWFILE);
			f.Close();
		}
		return logger;
	}

	void Log(const char *logData)
	{
		loggerFile->Open(FileOpenMode::WRRITEATTEHEND);
		loggerFile->WriteBlock((byte*)logData, strlen(logData));
		loggerFile->Close();
	}

private:
	File *loggerFile;
};