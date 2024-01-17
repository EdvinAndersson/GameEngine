#pragma once

#include <cstring>
#include <stdlib.h>
#include <math.h>

namespace CW {

    class Deserialize {
        public:
            Deserialize(char *data, char seperator);

            int GetInt();
            char *GetString();
        
        private:
            char *data_buffer;
            char seperator;
            unsigned int buffer_pos = 0;
    };
}