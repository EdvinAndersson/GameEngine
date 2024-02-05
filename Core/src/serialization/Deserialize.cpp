#include "Deserialize.h"

namespace CW {

    Deserialize::Deserialize(char *data, char seperator) {
        data_buffer = data;
        this->seperator = seperator;
    }

    int Deserialize::GetInt() {
        size_t size = GetLength();

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

    size_t Deserialize::GetSize_t() {
        size_t size = GetLength();

        size_t result = 0;
        for (unsigned int i = 0; i < size; i++) {
            int v = *(data_buffer + buffer_pos + i) - '0';
            result = result * 10 + v;
        }
        buffer_pos += size + 1;

        return result;
    }

    float Deserialize::GetFloat() {
        size_t length = GetLength();
        size_t length_to_dot = GetLength('.');

        int sign = 1;
        unsigned int start_index = 0;
        if (*(data_buffer + buffer_pos) == '-') {
            sign = -1;
            start_index = 1;
        }
        //integer numbers
        float integers = 0.0f;
        for (unsigned int i = start_index; i < length_to_dot; i++) {
            int v = *(data_buffer + buffer_pos + i) - '0';
            integers = integers * 10 + v;
        }
        //decimal numbers
        float decimals = 0.0f;
        for (unsigned int i = length - 1; i > length_to_dot; i--) {
            int v = *(data_buffer + buffer_pos + i) - '0';
            decimals = decimals * 0.1 + v;
        }
        decimals *= 0.1f;

        buffer_pos += length + 1;

        return sign * (integers + decimals);
    }

    char *Deserialize::GetString() {
        size_t size = GetLength();

        char *result = (char*) calloc(1, size + 1);
        memcpy(result, data_buffer + buffer_pos, size);
        buffer_pos += size + 1;
        return result;
    }
    vec3s Deserialize::GetVec3() {
        return vec3s { GetFloat(), GetFloat(), GetFloat() };
    }
    vec2s Deserialize::GetVec2() {
        return vec2s { GetFloat(), GetFloat() };
    }

    size_t Deserialize::GetLength() {
        return GetLength(seperator);
    }
    size_t Deserialize::GetLength(char seperator) {
        char *sep = strchr(data_buffer + buffer_pos, seperator);
        size_t size = sep - (data_buffer + buffer_pos);

        return size;
    }
}