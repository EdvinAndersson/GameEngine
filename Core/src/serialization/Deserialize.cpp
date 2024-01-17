#include "Deserialize.h"

namespace CW {

    Deserialize::Deserialize(char *data, char seperator) {
        data_buffer = data;
        this->seperator = seperator;
    }

    int Deserialize::GetInt() {
        char *sep = strchr(data_buffer + buffer_pos, seperator);
        size_t size = sep - (data_buffer + buffer_pos);

        int sign = 1;
        unsigned int start_index = 0;
        if (*(data_buffer + buffer_pos) == '-') {
            sign = -1;
            start_index = 1;
        }

        int result = 0;
        for (unsigned int i = start_index; i < size; i++) {
            int v = *(data_buffer + buffer_pos + i) - '0';
            result = result * 10 + v;
        }
        buffer_pos += size + 1;

        return result * sign;
    }

    char *Deserialize::GetString() {
        char *sep = strchr(data_buffer + buffer_pos, seperator);
        size_t size = sep - (data_buffer + buffer_pos);
        char *result = (char*) calloc(1, size + 1);
        memcpy(result, data_buffer + buffer_pos, size);
        buffer_pos += size + 1;
        return result;
    }
}