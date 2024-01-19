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
    int NumDigits(int x) {  
        x = abs(x);  
        return (x < 10 ? 1 :   
            (x < 100 ? 2 :   
            (x < 1000 ? 3 :   
            (x < 10000 ? 4 :   
            (x < 100000 ? 5 :   
            (x < 1000000 ? 6 :   
            (x < 10000000 ? 7 :  
            (x < 100000000 ? 8 :  
            (x < 1000000000 ? 9 :  
            10)))))))));  
    }
    int NumDigits(size_t x) {  
        return (x < 10 ? 1 :   
            (x < 100 ? 2 :   
            (x < 1000 ? 3 :   
            (x < 10000 ? 4 :   
            (x < 100000 ? 5 :   
            (x < 1000000 ? 6 :   
            (x < 10000000 ? 7 :  
            (x < 100000000 ? 8 :  
            (x < 1000000000 ? 9 :
            (x < 10000000000 ? 10 : 
            (x < 100000000000 ? 11 : 
            (x < 1000000000000 ? 12 : 
            (x < 10000000000000 ? 13 : 
            (x < 100000000000000 ? 14 : 
            (x < 1000000000000000 ? 15 : 
            (x < 10000000000000000 ? 16 : 
            (x < 100000000000000000 ? 17 : 
            (x < 1000000000000000000 ? 18 : 
            (x < 10000000000000000000 ? 19 : 
            20)))))))))))))))))));  
    }
    size_t HashString(char* p) {
        size_t result = 0;
        size_t length = strlen(p);
        const size_t prime = 31;
        for (size_t i = 0; i < length; ++i) {
            result = p[i] + (result * prime);
        }
        return result;
    }
    int FolderExists(char *path) {
        struct stat s;
        if (stat(path, &s) == 0 && (s.st_mode & S_IFDIR)) {
            return 1;
        }
        return 0;
    }
    int FolderCreate(char *path) {
        return _mkdir(path);
    }
}