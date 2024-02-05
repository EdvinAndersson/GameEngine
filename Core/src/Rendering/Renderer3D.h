#pragma once

#include "../Window.h"
#include "../GLFunc.h"
#include "cglm/struct.h"
#include "Shader.h"
#include "Texture.h"
#include "Model.h"
#include "assets/AssetManager.h"

namespace CW {

	struct R3D_RenderObject {
		unsigned int vao;
		unsigned int vertex_count, index_count;
	};

	void R3D_Init(Window *window);
	void R3D_Clear(vec4s color);

	void R3D_SetViewModel(mat4s view);
	void R3D_SetViewPos(vec3s position);

	void R3D_RenderSkybox(Texture skybox_texture, mat4s view);
	void R3D_RenderMesh(MeshIndex mesh_index, MaterialIndex material_index, vec3s position, vec3s scale, versors quaternion);

	void R3D__CreateInstanceSSBO();
	void R3D_BeginInstanced(Model *model);
	void R3D_RenderInstancedModel(vec3s position, vec3s scale);
	void R3D_RenderInstancedModel(vec3s position, vec3s scale, versors quaternion);
	void R3D_RenderInstanced();

	void R3D_SetDirectionalLight(vec3s direction, vec3s ambient, vec3s diffuse, vec3s specular);
	void R3D_SetPointLight(vec3s position, vec3s ambient, vec3s diffuse, vec3s specular, float constant, float linear, float quadratic);

	void R3D_UseShader(Shader *shader);
	void R3D_UseDefaultShader();
	Shader R3D_GetDefaultShader();
	void R3D_Resize(int screen_width, int screen_height);

}