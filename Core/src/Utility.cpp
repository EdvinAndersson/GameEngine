#include "Utility.h"

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
}