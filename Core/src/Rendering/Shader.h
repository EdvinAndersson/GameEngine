#pragma once

#include "../GLFunc.h"
#include "cglm/struct.h"
#include <stdio.h>
#include <fstream>
#include <string>

namespace CW {
	
	struct Shader {
		unsigned int id;

		void Use();
		void SetV4(const char *name, float x, float y, float z, float w);
		void SetV4(const char* name, vec4s vec4);
		void SetV3(const char *name, float x, float y, float z);
		void SetV3(const char *name, vec3s vec3);
		void SetV2(const char *name, float x, float y);
		void SetMat4f(const char *name, mat4s *mat);
		void SetFloat(const char *name, float a);
		void SetInt(const char *name, int a);
	};

	Shader CreateShader(std::string vertex_shader, std::string fragment_shader);
	Shader CreateShaderFromFiles(const char* vSource, const char* fSource);
}