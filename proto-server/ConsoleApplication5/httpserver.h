#include "stdafx.h"
#include "socket.h"
#include "file.h"
#include "exception.h"
#include "http_gen.h"

#define MIN_SIZE 32678
class IServer
{

};


enum Command
{
};

class Server: public IServer
{
	Socket *serverSocket;
public:
	Server()
	{
		serverSocket = NULL;
	}

	static Server &GetServer()
	{	
		static Server server = Server();
		return server;
	}

	string CreateResponse(const char* type, int len, int statusCode, char *stat)
	{
		char codebuf[10];
	    string str = "HTTP/1.1 " + string(itoa(statusCode, codebuf, 10)) + " " + string(stat);
		str += "\r\n";
		str += "Server: MSIYB\r\n";
		//str += "Content-Type: " + string("image/webp"/*type*/) + string("\r\n");
		str += "Content-Length: " + string(itoa(len-1, codebuf, 10));
		str += string("\r\n") + string("Connection: close\r\n\r\n");

		printf("Resp:\r\n%s\r\n\r\n", str.c_str());
		return str;
	}

	void SendResponse(Socket *dest, const char *data, const char *type, int len, int code, char *stat)
	{
		string str = CreateResponse(type, len, code, stat);
		int oldLen = len;
		int resPonseLen = str.length();
		char *buf = new char[len + resPonseLen];

		strcpy(buf, str.c_str());
		memcpy(buf + resPonseLen , data, len);
		//strcpy(buf, "\r\n\r\n");
		dest->Send(buf, len + resPonseLen);
		delete[] buf;
	}

	void Commander()
	{
		Socket s(2345, "127.0.0.1");
		s.Bind();
		s.Listen(12);

		char buf[1024];

		byte *data;
		byte *buf1;
		size_lt fsize = File::ReadAllBytes("e:\\out.txt", &buf1);
		File::WriteAllBytes("e:\\11.is", buf1, fsize);
		delete[] buf1;

		http_gen generator;
		for (;;)
		{
			Socket *n = s.AcceptSocket();
			try
			{
				int size1 = n->Recv(buf, 1024);
				string str(buf);
				int c = 1;
				if (size1)
				{
					size_t begin = str.find('/', 0) + 1;
					size_t end = str.find(" ", begin);
					if (str.find("?", begin) < end)
						end = str.find("?", begin);

					size_t len = end - begin;



					string path = string("../");
					if (len == 0)
						path += string("index.html");
					else
						path += string(str, begin, len);

					if (path == string("../index.html"))
						generator.GenIndex();

					generator.DecodeSpaces(path);

					begin = str.find("Accept:", 0) + 8;
					end = str.find(',', begin);
					len = end - begin;

					string ty = string("*/*");

					len = File::FileSize(path.c_str());
					byte *buf;
					int size = File::ReadAllBytes(path.c_str(), &data) + 1;
					SendResponse(n, (char*)data, ty.c_str(), size, 200, "OK");


				}
			}
			catch (Exception &e)
			{
				// if (e.GetErrcode() == ENOENT)
				//{
				SendResponse(n, "File no found", "", strlen("File no found"), 404, "Error");
				//sprintf(response, "%s %s", error, "404\r\n Not Found");
				//n->Send(response, strlen(response));
				memset(buf, 2048, 0);

			}
			n->ShutDown(How::SHUTBOTH);
			n->Close();
			delete n;
		}

		serverSocket = new Socket();
		char *requestBuffer = new char[MIN_SIZE];
		if (!serverSocket) ThrowException("null server");
		//Socket * newSocket;
		//while (newSocket = serverSocket.AcceptSocket())
		//{
		//	/*	int size;
		//		char sbuffer[1024];
		//		CreateResponse(NULL")
		//		sprintf(sbuffer, "200 Welcome \r\n\r\n");
		//		char path[MAX_PATH] = DEFAULT_PATH;
		//		TypeFile tF = ASCII;
		//		size = newSocket->Send(sbuffer, strlen(sbuffer));*/
		//	int len = newSocket->Recv(requestBuffer, MIN_SIZE);
		//	//ParseRequest();
		//}


	}

	Command ParseArgument(char *arg)
	{

	}


};

