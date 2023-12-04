#pragma once
#include <stdio.h>
#include <cstdint>
#include <corecrt_malloc.h>

namespace GL {

	void ConvertToRawData(const char *path, const char *variable_name, const char *output_file);
}