#pragma once

#include <cstring>
#include "string"
#include <stdlib.h>
#include <math.h>

#include "GLMath.h"

namespace CW {

    class Deserialize {
        public:
            Deserialize(char *data, char seperator);

            int GetInt();
            size_t GetSize_t();
            float GetFloat();
            char *GetString();
            Vec3 GetVec3();
            Vec2 GetVec2();
        private:
            size_t GetLength();
            size_t GetLength(char seperator);

            char *data_buffer;
            char seperator;
            unsigned int buffer_pos = 0;
    };
}