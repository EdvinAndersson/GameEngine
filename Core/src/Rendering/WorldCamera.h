#pragma once

#include "cglm/struct.h"

namespace CW {

    class WorldCamera {
        public:
            inline void Update() {
                view = GLMS_MAT4_IDENTITY_INIT;
                view = glms_translate(view, pos);
                view = glms_mat4_mul(glms_quat_mat4(glms_euler_xyz_quat(rotation)), view);
                view = glms_scale(view, vec3s { 1.0f, flip_y * 1.0f, 1.0f });
                CW::R3D_SetViewModel(view);
                CW::R3D_SetViewPos(pos);
            }

            vec3s pos, rotation;
            float fov;
            mat4s view;
            int flip_y = 1;
    };

    extern WorldCamera *g_main_camera;
}