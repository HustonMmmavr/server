#ifndef __FILE__H_
#define __FILE__H_
#include "Ifile.h"
#include "winfile.h"
#define FILE_BUFFER_SIZE 1024

void ResizeStringArray(char ***arrayStrings, int *oldSize, int rCoeff)
{
	int newSize = *oldSize * rCoeff;
	char **oldPtr = *arrayStrings;
	char **newPtr = new char*[newSize];//(char**)realloc(oldPtr, newSize * sizeof(*oldPtr));
	if (!newPtr) ThrowException("Cant resize string array");
	memcpy(newPtr, oldPtr, *oldSize * sizeof(*newPtr));
	delete[] oldPtr;
	*oldSize = newSize;
	*arrayStrings = newPtr;
}

void ResizeString(char **string, int *oldSize, int rCoeff)
{
	int newSize = *oldSize * rCoeff;
	char *oldPtr = *string;
	char *newPtr = new char[newSize];// (char*)realloc(oldPtr, newSize * sizeof(*oldPtr));
	if (!newPtr) ThrowException("Cant resize string");
	memcpy(newPtr, oldPtr, *oldSize * sizeof(*newPtr));
	delete[] oldPtr;
	*oldSize = newSize;
	*string = newPtr;
}

void Resize(char **string, int *oldSize, int newSize)
{
	char *oldPtr = *string;
	char *newPtr = new char[newSize];// (char*)realloc(oldPtr, newSize * sizeof(*oldPtr));
	if (!newPtr) ThrowException("Cant resize string");
	memcpy(newPtr, oldPtr, *oldSize);
	delete[] oldPtr;
	*oldSize = newSize;
	*string = newPtr;
}

#endif