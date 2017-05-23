#include "stdafx.h"
#include "winfile.h"
#include "functons.h"
#include <windows.h>
//#define TCHAR char;
WinFile::WinFile()
{
	_fileName = new char[MAX_PATH];
	_tFileName = new TCHAR[MAX_PATH];
	_opened = false;
}

void WinFile::ConvertFileName(const char *fileName, TCHAR *tFileName)
{
#ifdef _UNICODE
	ctow(fileName, tFileName);
	return;
#else
	strcpy(tFileName, fileName);
#endif
}

WinFile::WinFile(const char *fileName)
{
	_fileName = new char[MAX_PATH];
	if (!_fileName)
	{
		ThrowException("Can't allocate memory on fileName");
	}

	_tFileName = new TCHAR[MAX_PATH];
	if (!_tFileName)
	{
		ThrowException("Can't allocate memory on tFileName");
	}

	strcpy(_fileName, fileName);
	ConvertFileName(fileName, _tFileName);
	_opened = false;
}

WinFile::~WinFile()
{
	if (_opened)
	{
		Close();
	}
	
	delete[] _fileName;
	delete[] _tFileName;
}


//--------------------------------- Static Methods -------------------------------------------------//
size_lt WinFile::FileSize(HANDLE hFile)
{
	LARGE_INTEGER fileSize;
	if (!GetFileSizeEx(hFile, &fileSize))
	{
		ThrowFileExceptionWithCode("Can't get file size!", GetLastError());
	}
	return fileSize.QuadPart;
}

size_lt WinFile::FileSize(const char *fileName)
{
	TCHAR tFileName[MAX_PATH];
	ConvertFileName(fileName, tFileName);
	HANDLE hFile = Open_TCHAR(tFileName, FileOpenMode::READONLY);
	unsigned long long size = FileSize(hFile);
	Close(hFile);
	return size;
}

void WinFile::Close(HANDLE hFile)
{
	FlushFileBuffers(hFile); //?
	if (hFile != INVALID_HANDLE_VALUE)
	{
		if (!CloseHandle(hFile))
		{
			ThrowFileExceptionWithCode("Error occured while closing file!", GetLastError());
		}
	}
}

void WinFile::ParseMode(PDWORD creationDisposition, PDWORD desiredAccess, FileOpenMode mode)
{
	*creationDisposition = OPEN_EXISTING;
	if (mode == FileOpenMode::READONLY)
	{
		*desiredAccess = GENERIC_READ;
		return;
	}
	if (mode == READWRITE)
	{
		*desiredAccess = GENERIC_READ | GENERIC_WRITE;
		return;
	}
	if (mode == FileOpenMode::WRITE)
	{
		*desiredAccess = GENERIC_WRITE;
		return;
	}
	if (mode == FileOpenMode::WRITENEWFILE)
	{
		*desiredAccess = GENERIC_WRITE | GENERIC_READ;
		*creationDisposition = CREATE_ALWAYS;
		return;
	}
}



HANDLE WinFile::Open_TCHAR(const TCHAR *fileName, FileOpenMode mode)
{
	DWORD creationDisposition;
	DWORD desiredAccess;
	ParseMode(&creationDisposition, &desiredAccess, mode);

	HANDLE hFile = CreateFile(fileName, desiredAccess, FILE_SHARE_READ, NULL,
		creationDisposition,
		FILE_ATTRIBUTE_NORMAL, // normal file
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		ThrowFileExceptionWithCode("Cant open File", GetLastError());
	}
	return hFile;
}


void WinFile::Rename(const char *fileName, const char * newFileName)
{
	TCHAR tFileName[MAX_PATH];
	TCHAR tNewFileName[MAX_PATH];
	ConvertFileName(fileName, tFileName);
	ConvertFileName(newFileName, tNewFileName);

	if (!MoveFileEx(tFileName, tNewFileName, MOVEFILE_COPY_ALLOWED))
	{
		ThrowFileExceptionWithCode("Can't rename file", GetLastError());
	}
}


bool WinFile::Exist(const char *fileName)
{
	TCHAR tFileName[MAX_PATH];
	ConvertFileName(fileName, tFileName);
	return WinFile::Exist_TCHAR(tFileName);
}

bool WinFile::Exist_TCHAR(const TCHAR *tFileName)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE handle = FindFirstFile(tFileName, &FindFileData);
	bool found = (handle != INVALID_HANDLE_VALUE);
	if (found)
	{
		FindClose(handle);
	}
	return found;
}



void WinFile::Delete(const char *fileName)
{
	TCHAR tFileName[MAX_PATH];
	ConvertFileName(fileName, tFileName);
	WinFile::Delete_TCHAR(tFileName);
}

// if char = tcha
void WinFile::Delete_TCHAR(const TCHAR *tFileName)//const wchar_t *wFileName)
{
	if (!DeleteFile(tFileName))
	{
		ThrowFileExceptionWithCode("Can't delete file!", GetLastError());
	}
}



// TODO read how works readfile and modif readblock

size_lt WinFile::ReadBlock(HANDLE hFile, byte *block, size_lt blockSize)
{
	size_lt readedBytes = 1;
	bool res = ReadFile(hFile, (LPVOID)block, blockSize, (LPDWORD)&readedBytes, NULL);
	if (!res)
	{
		ThrowFileExceptionWithCode("Can't read data from file!", GetLastError());
	}
	return readedBytes;
}

void WinFile::WriteBlock(HANDLE hFile, byte * block, size_lt sizeBlock)
{
	size_lt writed = 0;
	bool res = WriteFile(hFile, (LPVOID)block, sizeBlock, (LPDWORD)&writed, NULL);
	if (!res)
	{
		ThrowFileExceptionWithCode("Can't write data into file!", GetLastError());
	}
}


size_lt WinFile::ReadAllBytes(const char *fileName, byte **block)
{
	TCHAR tFileName[MAX_PATH];
	ConvertFileName(fileName, tFileName);

	HANDLE hFile = Open_TCHAR(tFileName, FileOpenMode::READONLY);
	size_lt fSize = FileSize(hFile);
	byte *blockT = new byte[fSize];
	if (!blockT)
	{
		ThrowException("Cant Aloc memory for block arr");
	}

	fSize = WinFile::ReadBlock(hFile, blockT, fSize);
	*block = blockT;
	Close(hFile);
	return fSize;
}

void WinFile::WriteAllBytes(const char *fileName, byte * data, size_lt size, FileOpenMode mode)
{
	TCHAR tFileName[MAX_PATH];
	ConvertFileName(fileName, tFileName);
	HANDLE hFile = Open_TCHAR(tFileName, mode);
	WinFile::WriteBlock(hFile, data, size);
	Close(hFile);
}
//-----------------------------------------------------------------------------------------------------//







void WinFile::Open(FileOpenMode mode)
{
	ConvertFileName(_fileName, _tFileName);
	_hFile = Open_TCHAR(_tFileName, mode);
	_opened = true;
}


void WinFile::Open(const char *fileName, FileOpenMode mode)
{
	strcpy(_fileName, fileName);
	ConvertFileName(_fileName, _tFileName);
	_hFile = Open_TCHAR(_tFileName, mode);
	_opened = true;
}

void WinFile::Close()
{
	Close(_hFile);
	_opened = false;
}

void WinFile::Rename(const char *newFileName)
{
	Rename(_fileName, newFileName);

	ctow(newFileName, _wFileName);
	strcpy(_fileName, newFileName);
}

bool WinFile::Exist()
{
	return Exist_TCHAR(_tFileName);
}

void WinFile::Delete()
{
	if (_opened)
	{
		Close(_hFile);
	}
	Delete_TCHAR(_tFileName);
}

size_lt WinFile::FileSize()
{
	return FileSize(_hFile);
}

size_lt WinFile::Seek(size_lt offset, SeekReference move)
{
	LARGE_INTEGER ps, FilePos;
	ps.QuadPart = (size_lt)offset;

	long res = SetFilePointerEx(_hFile, ps, (PLARGE_INTEGER)&FilePos, move);
	if (res == INVALID_SET_FILE_POINTER)
	{
		ThrowFileExceptionWithCode("Can't set pointer in file on new position!", GetLastError());
	}

	return FilePos.QuadPart;
}

int WinFile::ReadByte()
{
	char b;
	int readedByte;
	bool res = ReadFile(_hFile, (LPVOID)&b, 1, (LPDWORD)&readedByte, NULL);
	if (!res)
	{
		ThrowFileExceptionWithCode("Can't read data from file!", GetLastError());
	}

	if (readedByte == 0)
	{
		return -1;
	}
	return (int)b;
}

size_lt WinFile::ReadBlock(byte *block, size_lt blockSize)
{
	return ReadBlock(_hFile, block, blockSize);
}


void WinFile::WriteByte(byte b)
{
	WriteBlock(&b, 1);
}

void WinFile::WriteBlock(byte *block, size_lt sizeBlock)
{
	WriteBlock(_hFile, block, sizeBlock);
}


FileMeta WinFile::LastModified(const char * fileName)
{
	FileMeta meta;
	return meta;
}





//wchar_t wFileName[MAX_PATH];
//ctow(fileName, wFileName);


// TODO Need wc
//
//HANDLE WinFile::Open_TCHAR(const char *fileName, FileOpenMode mode)
//{
//	DWORD creationDisposition;
//	DWORD desiredAccess;
//	ParseMode(&creationDisposition, &desiredAccess, mode);
//
//	wchar_t wFileName[MAX_PATH];
//	ctow(fileName, wFileName);
//
//	HANDLE hFile = CreateFile(wFileName, desiredAccess, FILE_SHARE_READ, NULL,
//		creationDisposition,
//		FILE_ATTRIBUTE_NORMAL, // normal file
//		NULL);
//	if (hFile == INVALID_HANDLE_VALUE)
//	{
//		ThrowFileExceptionWithCode("Cant open File", GetLastError());
//	}
//	return hFile;
//}


//wchar_t wNewFileName[MAX_PATH];
//ctow(newFileName, wNewFileName);

// TODO Need wc
//
//HANDLE WinFile::Open_TCHAR(const char *fileName, FileOpenMode mode)
//{
//	DWORD creationDisposition;
//	DWORD desiredAccess;
//	ParseMode(&creationDisposition, &desiredAccess, mode);
//
//	wchar_t wFileName[MAX_PATH];
//	ctow(fileName, wFileName);
//
//	HANDLE hFile = CreateFile(wFileName, desiredAccess, FILE_SHARE_READ, NULL,
//		creationDisposition,
//		FILE_ATTRIBUTE_NORMAL, // normal file
//		NULL);
//	if (hFile == INVALID_HANDLE_VALUE)
//	{
//		ThrowFileExceptionWithCode("Cant open File", GetLastError());
//	}
//	return hFile;
//}

//_wFileName = new wchar_t[MAX_PATH];
//if (!_wFileName)
//{
//	ThrowException("Can't allocate memory on wFileName");
//}
/*
if (_isUnicodeFormat)
ctow(_fileName, _wFileName);
else
#ifndef _UNICODE
strcpy(_tFileName, _fileName);
#endif*/