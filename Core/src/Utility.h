#pragma once

#include <stdio.h>
#include <cstdint>
#include <corecrt_malloc.h>
#include <sys/stat.h>
#include <direct.h>

namespace CW {

	void ConvertToRawData(const char *path, const char *variable_name, const char *output_file);
	char* ConvertWideCharToChar(const wchar_t *str);
	int StrToInt(char *str);
	double Random();
	int NumDigits(int x);
	int NumDigits(size_t x);
	size_t HashString(char* p);
	int FolderExists(char *path);
	int FolderCreate(char *path);
}