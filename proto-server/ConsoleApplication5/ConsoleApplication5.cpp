//
// ser.cpp: ���������� ����� ����� ��� ����������� ����������.

#include "stdafx.h"
#include "httpserver.h"
//#include "header.h"
#include <stdlib.h>
#include <vector>
#include "file.h"
#include "logger.h"
#include "windir.h"
int main()
{
	try
	{
		setlocale(LC_CTYPE, ".1251");
		WindowsDir::ListFiles("../");
		char **strings;// { "afnwklef", "kfkewf","akef" };
		//File::WriteAllCharStrings("newFile", strings, 3);

		size_lt num = File::ReadAllCharStrings("newFile", &strings);
		Logger &logger = Logger::GetLogger();
		logger.Log("error\r\n");
		logger.Log("\r\n");
		logger.Log("\r\n");
		logger.Log("no");
		Server::GetServer().Commander();
	}
	catch (Exception &e)
	{
		printf("%s %s", e.what(), ParseException(e.GetCode()));
		getchar();
		getchar();
	}
	//File *f = new File("C:\\users\\ho\\Desktop\\g.docx", 10);
	//f->Open(FileOpenMode::READONLY);
	//File out("C:\\Users\\ho\\Desktop\\1.docx", 10);
	//out.Open(FileOpenMode::WRITEONEXISTS);
	//int b = f->ReadByte();
	//while (b != -1)
	//{
	//	out.WriteByte(byte(b));
	//	b = f->ReadByte();
	//}
	//out.Close();


	//std::vector<int> a1;
	//Array<int> *arr =new Array<int>(10);// arr(10);s
	//for (int i = 0; i < 100; i++)
	//{
	//	int rnd = rand() % 1000;
	//	a1.push_back(rnd);
	//	arr->PushBack(rnd);// rand() % 1000);

	//}
	//Array<int> a;
	//a = *arr;
	//delete arr;
	//for (int i = 0; i < 100; i++)
	//{
	//	if (a[i] != a1[i])
	//	{
	//		printf("ffff");
	//		break;
	//	}
	//}
	//Server::GetServer().Commander();
	return 0;
}

