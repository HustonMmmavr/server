/*!
\file ifile.h "server\desktop\src\cross\ifile.h"
\authors Alexandr Barulev, Dmitry Zaitsev
\copyright � MSiYB 2017
\license GPL license
\version 0.1
\date 28 February 2017
*/

#pragma once

#define MIN_BUFFER_SIZE 128
#define FILE_BUFFER_SIZE 1024
#define MIN_STRING_SIZE 128
#define MIN_STRING_ARRAY_SIZE 2

#include "fileexception.h"

/// File meta data structure
typedef struct
{
	char* filename;			///< File name with is's local path
	char* filepath;			///< Path to dir where is our file
	char* filetype;			///< File extention
	time_t creationdate;	///< File creation date
} FileMeta;

/// Opening file rule
typedef enum
{
	READONLY,		///< Open in READ ONLY mode
	WRITEONEXISTS,	///< Write in end of the existing file or create new file
	WRITE,			///< Create new file to write (or truncate existing file to write after)
	READWRITE		///< Open file for reading and writing (will truncate existing file)
} FileOpenMode;

/// Position used as reference for the offset.
typedef enum 
{ 
	START,			/// Beginning of file.
	CURRENT,		/// Current position of the file pointer.
	END				/// End of file.
} SeekReference;

/*!
\class IFile ifile.h "server\desktop\src\cross\ifile.h"
\brief  Class-interface for OS depended classes
Defined methods should be realized in OS depended classes
*/
class IFile
{
public:
	/*!
	Dealocates memory.
	*/
	virtual ~IFile() {};

	/*!
	Opens file in predetermined mode.
	\param[in] mode Mode to open file (list of possibles modes defined in ifile.h).
	*/
	virtual void Open(FileOpenMode mode) = 0;

	virtual void Open(const char *fileName, FileOpenMode mode) = 0;

	/*!
	Closes file descriptor.
	*/
	virtual void Close() = 0;

	/*!
	Set new file name.
	\param[in] newFileName Name to be associated to a current file.
	*/
	virtual void Rename(const char *newFileName) = 0;

	/*!
	Checks if file exists.
	\return TRUE if file exists, FALSE in other case.
	*/
	virtual bool Exist() = 0;

	/*!
	Deletes file.
	*/
	virtual void Delete() = 0;

	/*!
	Return size of file.
	\return Size of file.
	*/
	virtual size_lt FileSize() = 0;

	/*!
	Moves the file pointer of the specified file.
	\param[in] offset Offset of new pointer position
	\param[in] move Position used as reference for the offset.
	\return New pointer position
	*/
	virtual size_lt Seek(size_lt offset, SeekReference move) = 0;

	/*!
	Reads one byte from file.
	\return Value of readed byte or -1 if can't read (ex. EOF).
	*/																	  
	virtual int ReadByte() = 0;

	/*!
	Reads block from file.
	\param[out] block Array of bytes from file.
	\param[in] sizeBlock Amount of bytes to be readed.
	\return Amount of bytes were readed from file.
	*/
	virtual size_lt ReadBlock(byte *block, size_lt blockSize) = 0;
			
	/*!
	Writes byte in file.
	\param[in] b Byte to save.
	*/
	virtual void WriteByte(byte b) = 0;
	
	/*!
	Writes block into file.
	Clears cache of readed bytes.
	\param[in] block Array of bytes to be writed.
	\param[in] blockSize Amount of bytes to be writed.
	*/
	virtual void WriteBlock(byte *block, size_lt sizeBlock) = 0;

};
