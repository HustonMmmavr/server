#pragma once
#include "stdafx.h"
#include "socket.h"
#include "file.h"

#include "windir.h"
#include "exception.h"

class http_gen
{
public:
	http_gen();
	~http_gen();

	void EncodeSpaces(string &str);
	void DecodeSpaces(string &str);
	
	void GenIndex();
};

