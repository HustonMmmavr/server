#pragma once
#include "stdafx.h"
#include "exception.h"
#include <string>
using namespace std;

void ResizeStringArray(char ***arrayStrings, size_lt *oldSize, int rCoeff);
void ResizeString(char **string, size_lt *oldSize, int rCoeff);

void Resize(char **string, size_lt *oldSize, size_lt newSize);

size_lt ConvertCharStringArrayToByte(byte** byteArr, char **str, size_lt cnt);

size_lt ConvertStringArrayToByte(byte** byteArr, string *str, size_lt cnt);

void ctow(const char *str, wchar_t *wStr);