#include "stdafx.h"
#include "http_gen.h"

http_gen::http_gen()
{
}


http_gen::~http_gen()
{
}

void http_gen::EncodeSpaces(string & str)
{
	unsigned int pos = str.find(" ");
	while (pos < 4294967295)
	{
		str.replace(pos, 1, "%20");
		pos = str.find(" ");
	}
}

void http_gen::DecodeSpaces(string & str)
{
	unsigned int pos = str.find("%20");
	while (pos < 4294967295)
	{
		str.replace(pos, 3, " ");
		pos = str.find("%20");
	}
}

void http_gen::GenIndex()
{
	byte *data;
	FILE *f = fopen("../index.html", "w");
	int size = File::ReadAllBytes("../index_top.html", &data);
	fwrite(data, 1, size, f);

	WindowsDir w_dir;

	char *list = w_dir.ListFiles("../library");
	string s_list(list);
	unsigned int spos = 0;
	unsigned int epos;
	
	int count = 0;
	unsigned int dot_pos;
	epos = s_list.find(';', spos);
	while (epos < 4294967295 && epos != spos)
	{
		string fname = s_list.substr(spos, epos - spos);
		string sp_fname = fname;
		EncodeSpaces(sp_fname);

		dot_pos = fname.rfind('.');
		string dot_str = fname.substr(dot_pos);
		if (dot_str == ".mp4")
		{
			count++;

			fwrite("<div class=\"poster poster_width\"><a href=\"video_player.html?movie=", 1,
				strlen("<div class=\"poster poster_width\"><a href=\"video_player.html?movie="), f);

			fwrite(sp_fname.c_str(), 1, strlen(sp_fname.c_str()), f);

			fwrite("\"><img src=\"pic/ph.jpg\"><div class=\"library_file_name\">", 1,
				strlen("\"><img src=\"pic/ph.jpg\"><div class=\"library_file_name\">"), f);

			fwrite(fname.c_str(), 1, strlen(fname.c_str()), f);

			fwrite("</div></a></div>", 1, strlen("</div></a></div>"), f);
		}
		spos = epos+1;
		epos = s_list.find(';', spos);
	}


	if (count == 0)
		fwrite("<div class=\"empty_text\"> Library is empty. Fill the library folder with mp4 files</div>", 1,
				strlen("<div class=\"empty_text\"> Library is empty. Fill the library folder with mp4 files</div>"), f);

	size = File::ReadAllBytes("../index_bot.html", &data);
	
	fwrite(data, 1, size, f);
	fclose(f);
}
