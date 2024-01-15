#pragma once

#include <stdio.h>
#include <cstdint>
#include <corecrt_malloc.h>

namespace CW {

	void ConvertToRawData(const char *path, const char *variable_name, const char *output_file);
	char* ConvertWideCharToChar(const wchar_t *str);
	int StrToInt(char *str);
	double Random();
}