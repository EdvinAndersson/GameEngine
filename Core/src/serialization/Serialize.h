#pragma once

#include "stdlib.h"
#include "stdio.h"
#include "string"

#include "Utility.h"
#include "cglm/struct.h"

namespace CW {

    class Serialize {
        public:
            Serialize(unsigned int size, char seperator);
            ~Serialize();

            void SubmitInt(int data);
            void SubmitSize_t(size_t data);
            void SubmitFloat(float data);
            void SubmitString(char *data);
            void SubmitVec3(vec3s data);
            void SubmitVec2(vec2s data);

            inline char *GetData() { return data_buffer; }
        private:
            void SubmitSeperator();

            unsigned int buffer_pos = 0;
            char seperator;
            char *data_buffer;
    };
}