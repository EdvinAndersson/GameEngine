#include "Serialize.h"

namespace CW {

    Serialize::Serialize(unsigned int size, char seperator) {
        this->seperator = seperator;

        data_buffer = (char *) calloc(1, size);
    }
    Serialize::~Serialize() {
        delete data_buffer;
    }

    void Serialize::SubmitInt(int data) {
        int digits = CW::NumDigits(data);
        if (data < 0) digits++;
        sprintf(data_buffer + buffer_pos, "%ld", data);
        buffer_pos += digits;

        SubmitSeperator();
    }

    void Serialize::SubmitSize_t(size_t data) {
        int digits = CW::NumDigits(data);
        sprintf(data_buffer + buffer_pos, "%zu", data);
        buffer_pos += digits;

        SubmitSeperator();
    }

    void Serialize::SubmitFloat(float data) {
        int digits = CW::NumDigits((int) data) + 8;
        if (data < 0) digits++;
        sprintf(data_buffer + buffer_pos, "%.7f", data);
        buffer_pos += digits;

        SubmitSeperator();
    }

    void Serialize::SubmitString(char *data) {
        size_t size = strlen(data);
        memcpy(data_buffer + buffer_pos, data, sizeof(char) * size);
        buffer_pos += sizeof(char) * size;
        
        SubmitSeperator();
    }
    void Serialize::SubmitVec3(vec3s data) {
        SubmitFloat(data.x);
        SubmitFloat(data.y);
        SubmitFloat(data.z);
    }
    void Serialize::SubmitVec2(vec2s data) {
        SubmitFloat(data.x);
        SubmitFloat(data.y);
    }

    void Serialize::SubmitSeperator() {
        *(data_buffer + buffer_pos) = seperator;
        buffer_pos++;
    }
}