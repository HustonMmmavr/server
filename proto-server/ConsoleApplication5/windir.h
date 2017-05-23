#ifndef __WINDIR__H_
#define __WINDIR__H_
#ifdef _WIN32
#include <stdio.h>
#include <windows.h>
#include <wchar.h>
#include <tchar.h>
#include "accctrl.h"
#include "aclapi.h"
#pragma comment(lib, "advapi32.lib")
#include "file.h"

enum InfoLevels { Standart, Basic, Max };
//
//bool FindFirst(char *name, void **data, InfoLevels level = Basic)///, int SearchOP = 0, )
//{
//	LPVOID *info;
//	FINDEX_INFO_LEVELS lev;
//	wchar_t *wName = new wchar_t[MAX_PATH];
//	mbstowcs(wName, name, strlen(name) + 1);
//	
//	if (level = InfoLevels::Basic)
//	{
//		info = (LPVOID*)(new WIN32_FIND_DATA);
//		lev = FindExInfoBasic;
//	}
//
//}
#include <string>

#define ACCT_NAME_SIZE 1024
#define ACL_SIZE 1024
#define INIT_EXCEPTION 0x3
#define CHANGE_EXCEPTION 0x4
#define SID_SIZE 1024  /* See support.h */
#define DOM_SIZE 1024

#define	S_IRWXU	0000700			/* RWX mask for owner */
#define	S_IRUSR	0000400			/* R for owner */
#define	S_IWUSR	0000200			/* W for owner */
#define	S_IXUSR	0000100			/* X for owner */

#define	S_IRWXG	0000070			/* RWX mask for group */
#define	S_IRGRP	0000040			/* R for group */
#define	S_IWGRP	0000020			/* W for group */
#define	S_IXGRP	0000010			/* X for group */

#define	S_IRWXO	0000007			/* RWX mask for other */
#define	S_IROTH	0000004			/* R for other */
#define	S_IWOTH	0000002			/* W for other */
#define	S_IXOTH	0000001			/* X for other */


class WindowsDir
{
private:

	static void ResizeString(char **string, int *oldSize, int rCoeff)
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


	static void SetPermision(char *charPermission, char value, long permissionsFlag, long flag)
	{
		if (permissionsFlag & flag)
			*charPermission = value;
		else
			*charPermission = '-';
	}

	static void CharPermissions(char* permissionBuffer, long permissionsFlag)
	{
		SetPermision(&permissionBuffer[0], 'r', permissionsFlag, S_IRUSR);
		SetPermision(&permissionBuffer[1], 'w', permissionsFlag, S_IWUSR);
		SetPermision(&permissionBuffer[2], 'x', permissionsFlag, S_IXUSR);
		SetPermision(&permissionBuffer[3], 'r', permissionsFlag, S_IRGRP);
		SetPermision(&permissionBuffer[4], 'w', permissionsFlag, S_IWGRP);
		SetPermision(&permissionBuffer[5], 'x', permissionsFlag, S_IXGRP);
		SetPermision(&permissionBuffer[6], 'r', permissionsFlag, S_IROTH);
		SetPermision(&permissionBuffer[7], 'w', permissionsFlag, S_IWOTH);
		SetPermision(&permissionBuffer[8], 'x', permissionsFlag, S_IXOTH);
	}

	static void ConvertCharToTCHAR(char *source, TCHAR *out)
	{
#ifdef _UNICODE
		mbstowcs((wchar_t*)out, source, strlen(source) + 1);
#else
		strcpy(source, out);
#endif
	}

	/*static int TCHARStringLength(TCHAR *string)
	{
#ifdef _UNICODE
		return wcslen(string);
#else 
		return strlen(string);
#endif
	}

	static void StringNTCHARCopy(TCHAR *source, TCHAR *dest, size_t size)
	{
#ifdef _UNICODE
		wcsncpy(dest, source, size);
#else
		strncpy(dest, source, size);
#endif
	}

	static void StringTCHARCopy(TCHAR *source, TCHAR *dest)
	{
#ifdef _UNICODE
		wcscpy(dest, source);
#else
		strcpy(dest, source);
#endif
	}*/

	static void ConvertTCHARToChar(TCHAR *source, char *out)
	{
#ifdef _UNICODE
		wcstombs(out, (wchar_t*)source, wcslen((wchar_t*)source) + 1);
#else
		strcpy(source, out);
#endif
	}

	static DWORD ReadFilePermissions(LPTSTR lpFileName, LPTSTR UsrNm, LPTSTR GrpNm)

		/* ���������� ���������� �� ������ � ����� � ����� UNIX. */

	{
		PSECURITY_DESCRIPTOR pSD = (PSECURITY_DESCRIPTOR)GlobalAlloc(GMEM_FIXED, 65536);;
		DWORD LenNeeded, PBits, iAce;
		BOOL DaclF, AclDefF, OwnerDefF, GroupDefF;
		BYTE DAcl[ACL_SIZE];
		PACL pAcl = (PACL)&DAcl;
		ACL_SIZE_INFORMATION ASizeInfo;
		PACCESS_ALLOWED_ACE pAce;
		BYTE AType;
		HANDLE ProcHeap = GetProcessHeap();
		PSID pOwnerSid, pGroupSid;
		TCHAR RefDomain[2][DOM_SIZE];
		DWORD RefDomCnt[] = { DOM_SIZE, DOM_SIZE };
		DWORD AcctSize[] = { ACCT_NAME_SIZE, ACCT_NAME_SIZE };
		SID_NAME_USE sNamUse[] = { SidTypeUser, SidTypeGroup };

		/* �������� ��������� ������ ����������� ������������. */
		if (GetFileSecurity(lpFileName, OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION, pSD, 0, &LenNeeded))
			ThrowExceptionWithCode("GetFile security error, GetLast Error", GetLastError());

		//		if ((pSD = HeapAlloc(ProcHeap, HEAP_GENERATE_EXCEPTIONS, LenNeeded)) == NULL)
		//			ThrowExceptionWithCode("HeapAlloc security error, GetLast Error", GetLastError());

		if (!GetFileSecurity(lpFileName, OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION, pSD, LenNeeded, &LenNeeded))
			ThrowExceptionWithCode("GetFile security error, GetLast Error", GetLastError());


		if (!GetSecurityDescriptorDacl(pSD, &DaclF, &pAcl, &AclDefF))
			ThrowExceptionWithCode("GetSecurityDescriptorDacl  error, GetLast Error", GetLastError());

		if (!GetAclInformation(pAcl, &ASizeInfo, sizeof(ACL_SIZE_INFORMATION), AclSizeInformation))
			ThrowExceptionWithCode("GetAclInformation  error, GetLast Error", GetLastError());

		PBits = 0; /* ��������� ���������� �� ������ �� ������ ACL. */

		for (iAce = 0; iAce < ASizeInfo.AceCount; iAce++)
		{
			if (!GetAce(pAcl, iAce, (LPVOID*)&pAce))
				ThrowExceptionWithCode("GetAce  error, GetLast Error", GetLastError());
			AType = pAce->Header.AceType;
			if (AType == ACCESS_ALLOWED_ACE_TYPE) PBits |= (0x1 << (8 - iAce));
		}

		/* ���������� ��� ��������� � ��������� ������. */
		if (!GetSecurityDescriptorOwner(pSD, &pOwnerSid, &OwnerDefF))
			ThrowExceptionWithCode("GetSecurityDescriptorOwner  error, GetLast Error", GetLastError());

		if (!GetSecurityDescriptorGroup(pSD, &pGroupSid, &GroupDefF))
			ThrowExceptionWithCode("GetSecurityDescriptorOwner  error, GetLast Error", GetLastError());

		if (!LookupAccountSid(NULL, pOwnerSid, UsrNm, &AcctSize[0], GrpNm, &RefDomCnt[0], &sNamUse[0]))
		{
			ConvertCharToTCHAR("No information", UsrNm);
			ConvertCharToTCHAR("No information", GrpNm);
		}
		//else
		//	ThrowExceptionWithCode("LookupAccountSid  error, GetLast Error", GetLastError());

		if (!LookupAccountSid(NULL, pGroupSid, GrpNm, &AcctSize[1], RefDomain[1], &RefDomCnt[1], &sNamUse[1]))
			GrpNm = (TCHAR)'\0';//ThrowExceptionWithCode("LookupAccountSid  error, GetLast Error", GetLastError());
		else
			//StringTCHARCopy(GrpLookupAccountSidNm, RefDomain[1]);
			_tcscpy((TCHAR*)GrpNm, RefDomain[1]);
		GlobalFree(pSD);
		return 0;//PBits;
	}

public:

	static char *ListFiles(const char *path)
	{
		char absoluteName[MAX_PATH];

		char fileName[MAX_PATH];
		sprintf(fileName, "%s%s", path, "\\*");
		//= "C:\\Users\\ho\\Downloads\\*";
		size_t lenDir = strlen(fileName);
		WIN32_FIND_DATA info;
		FILETIME time;
		SYSTEMTIME sysTime;
		char fName[MAX_PATH];	// current file name
		char permission[10];	// file permissions
		char resultString[1024];
		char fileType;
		char cAccName[128] = "";
		char cGrName[128] = "";
		unsigned long long fSize;
		TCHAR tAccName[ACCT_NAME_SIZE];
		TCHAR tGrName[ACCT_NAME_SIZE];
		TCHAR tName[MAX_PATH];
		TCHAR *dirName = new TCHAR[MAX_PATH];
		int sizeListFiles = 1024;
		int currentLen = 0;
		char *listFiles = new char[sizeListFiles];
		listFiles[0] = '\0';
		//if (!ListFiles)
		// init
		ConvertCharToTCHAR(fileName, dirName);
		//mbstowcs(dirName, fileName, lenDir + 1);
		HANDLE hDir = FindFirstFile(dirName, &info);//FindFirstFileEx(dirName, FindExInfoBasic, &info , FindExSearchNameMatch, NULL, 0);
		_tcsncpy(tName, dirName, lenDir - 1);
		//_tcsncpy(tName, dirName, lenDir - 1);	// without *
		permission[9] = 0;

		/*
		Txxxxxxxxx[]uk[]user[]group[]size[]mm[]dd[]yytt[]name CR, LF
		���,
		T � ��� ��������(�d� � �������, � - � � ����, �l� � ������ � �.�.);
		xxxxxxxxx � �������� ������ �����;
		user � ������������, �������� �����;
		group � ������ ���������;
		size � ������ ��������;
		mm � ����� �������� �������� � ��������� ����, �������� �jul�;
		dd � ���� ������ �������� ��������;
		yytt � ����� ����� ���� ��� ��� ����� �������� ��������;
		name � ��� ��������(�����, ��������, ������);
		[] � ���� ��� ����� ��������.
		*/
		do
		{
			ConvertTCHARToChar(info.cFileName, fName);
			if (fName[0] != '.')
			{
				// Get file absolute name
				memcpy(tName + lenDir - 1, info.cFileName, (_tcslen(info.cFileName) + 1) * sizeof(TCHAR));
				ConvertTCHARToChar(tName, absoluteName);
				//wcstombs(absoluteName, tName, wcslen(wName) + 1);
				// Get file type
				if (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					fileType = 'd';
				else if (info.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)
					fileType = 'l';
				else
					fileType = '-';

				// Get File permissions and file group and file owner
				DWORD res = ReadFilePermissions(tName, tAccName, tGrName);

				// Parse byte permissions to human understand rwxrwwxrwx to char
				CharPermissions(permission, res);

				// Get cha acc Name and domin Name
				ConvertTCHARToChar(tAccName, cAccName);
				ConvertTCHARToChar(tGrName, cGrName);

				// Get time creati
				time = info.ftCreationTime;
				FileTimeToSystemTime(&time, &sysTime);

				// fileSize
				if (fileType == 'd')
					fSize = 0;
				else
					fSize = File::FileSize(absoluteName);

				// write data to resultString
				sprintf(resultString, "%s;", fName);
				size_t len = strlen(resultString);

				if (currentLen + len >= sizeListFiles)
					ResizeString(&listFiles, &sizeListFiles, 2);

				currentLen += len;
				strcat(listFiles, resultString);

			}
		} while (FindNextFile(hDir, &info) != NULL);
		return NULL;// listFiles;
	}

	
};

#endif
#endif