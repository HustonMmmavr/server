// stdafx.h: включаемый файл дл€ стандартных системных включаемых файлов
// или включаемых файлов дл€ конкретного проекта, которые часто используютс€, но
// не часто измен€ютс€
//

#pragma once

typedef unsigned char byte;
typedef unsigned long long size_lt;

#include <stdio.h>
#include <memory.h>
#include <time.h>

#include <vector>
#include <string>
#include <memory>
#include "string.h"

#include <stdlib.h>
#include <string.h>
#include <iostream>


#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <process.h>
#define PATH_MAX _MAX_PATH

#endif
// TODO: ”становите здесь ссылки на дополнительные заголовки, требующиес€ дл€ программы
