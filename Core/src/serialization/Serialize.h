#pragma once

#include "stdlib.h"
#include "stdio.h"
#include "string"

#include "Utility.h"
#include "GLMath.h"

namespace CW {

    class Serialize {
        public:
            Serialize(unsigned int size, char seperator);
            ~Serialize();

            void SubmitInt(int data);
            void SubmitSize_t(size_t data);
            void SubmitFloat(float data);
            void SubmitString(char *data);
            void SubmitVec3(Vec3 data);
            void SubmitVec2(Vec2 data);

            inline char *GetData() { return data_buffer; }
        private:
            void SubmitSeperator();

            unsigned int buffer_pos = 0;
            char seperator;
            char *data_buffer;
    };
}