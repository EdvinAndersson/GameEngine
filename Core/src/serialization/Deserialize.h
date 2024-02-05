#pragma once

#include <cstring>
#include "string"
#include <stdlib.h>
#include <math.h>

#include "cglm/struct.h"

namespace CW {

    class Deserialize {
        public:
            Deserialize(char *data, char seperator);

            int GetInt();
            size_t GetSize_t();
            float GetFloat();
            char *GetString();
            vec3s GetVec3();
            vec2s GetVec2();
        private:
            size_t GetLength();
            size_t GetLength(char seperator);

            char *data_buffer;
            char seperator;
            unsigned int buffer_pos = 0;
    };
}