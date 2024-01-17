#pragma once

#include "stdlib.h"
#include "stdio.h"
#include "string"

#include "Utility/Utility.h"

namespace CW {

    class Serialize {
        public:
            Serialize(unsigned int size, char seperator);
            ~Serialize();

            void SubmitInt(int data);
            void SubmitString(char *data);

            inline char *GetData() { return data_buffer; }
        private:
            void SubmitSeperator();

            unsigned int buffer_pos = 0;
            char seperator;
            char *data_buffer;
    };
}