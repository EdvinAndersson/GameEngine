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
        char *b = (char*) calloc(1, digits);
        sprintf(b, "%ld", data);

        memcpy(data_buffer + buffer_pos, b, digits);
        buffer_pos += digits;
        delete b;

        SubmitSeperator();
    }

    void Serialize::SubmitString(char *data) {
        size_t size = strlen(data);
        memcpy(data_buffer + buffer_pos, data, sizeof(char) * size);
        buffer_pos += sizeof(char) * size;
        
        SubmitSeperator();
    }
    
    void Serialize::SubmitSeperator() {
        *(data_buffer + buffer_pos) = seperator;
        buffer_pos++;
    }
}