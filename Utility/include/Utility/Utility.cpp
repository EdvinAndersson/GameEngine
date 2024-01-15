#include "Utility.h"
#include <stdlib.h>
#include <stdio.h>
#include <string>

namespace CW {

	void ConvertToRawData(const char *path, const char *variable_name, const char *output_file) {
        char *data;
        int size;

        FILE *file = fopen(path, "rb");
        fseek(file, 0L, SEEK_END);
        size = ftell(file);
        fseek(file, 0L, SEEK_SET);

        data = (char *)malloc(size);

        fread(data, size, 1, file);


        FILE *file_output = fopen(output_file, "w");
        fprintf(file_output, "#pragma once\n");
        fprintf(file_output, "char %s[] = {", variable_name);

        for (int i = 0; i < size; i++) {
            fprintf(file_output, "0x%hhx,", data[i]);
        }
        fprintf(file_output, "};\n");
	}
    char* ConvertWideCharToChar(const wchar_t *str) {
        size_t origsize = wcslen(str) + 1;
        size_t convertedChars = 0;
        const size_t newsize = origsize * 2;
        char* nstring = new char[newsize];
        wcstombs_s(&convertedChars, nstring, newsize, str, _TRUNCATE);

        return nstring;
    }

    int StrToInt(char *str) {
        int len = strlen(str);
        int result = 0;

        for (int i = 0; i < len; i++) {
            result += pow(10, len - i - 1) * (str[i] - 0x30);    
        }
        return result;
    }
    double Random() {
        return rand() / (double) RAND_MAX;
    }
}