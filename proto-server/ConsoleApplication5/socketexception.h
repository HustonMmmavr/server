#pragma once

#include "exception.h"

#ifdef DEBUG
#define ThrowSocketException(msg)											\
{																		\
	throw SocketException(msg, __FILE__, __FUNCTION__, __LINE__);			\
}

#define ThrowSocketExceptionWithCode(msg, code)							\
{																		\
	throw SocketException(msg, __FILE__, __FUNCTION__, __LINE__, code);	\
}
#else
#define ThrowSocketException(msg)										\
{																		\
	throw SocketException(msg);											\
}																	
#define void ThrowSocketExceptionWithCode(msg, code)					\
{																		\
	throw SocketException(msg, code);										\
}
#endif

class SocketException : public Exception
{
public:
	SocketException(const char* message, const char *file, const char *function, int line, long errCode)
	{
		sprintf(this->message, "In file %s function %s line %d error occured.\n Error message: %s\n Error code: %d\n", file, function, line, message, errCode);
		this->errCode = errCode;
	}

	SocketException(const char* message, const char *file, const char *function, int line)
	{
		sprintf(this->message, "In file %s function %s line %d error occured.\n Error message: %s\n", file, function, line, message);
		this->errCode = -1;
	}

	const char* what() const
	{
		return message;
	}

	void operator = (const SocketException &fE)
	{
		strcpy(message, fE.what());
		errCode = fE.GetErrorCode();
	}

	long GetErrorCode() const
	{
		return errCode;
	}

	void SetErrorCode(long errCode)
	{
		this->errCode = errCode;
	}
};
