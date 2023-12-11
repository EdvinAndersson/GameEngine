#pragma once

#include "../Window.h"
#include "../GLFunc.h"
#include "../GLMath.h"
#include "Shader.h"
#include "Texture.h"
#include "Model.h"

namespace CW {

	struct R3D_RenderObject {
		unsigned int vao;
		unsigned int vertex_count, index_count;
	};

	void R3D_Init(Window *window);
	void R3D_Clear(Vec4 color);

	void R3D_SetViewModel(Mat4 view);
	void R3D_SetViewPos(Vec3 position);
	
	void R3D_RenderSkybox(Texture skybox_texture, Mat4 view);

	void R3D_RenderModel(Model *model, Vec3 position, Vec3 scale);
	void R3D_RenderModel(Model *model, Vec3 position, Vec3 scale, Vec3 color);
	void R3D_RenderModel(Model *model, Vec3 position, Vec3 scale, Quaternion quaternion);
	void R3D_RenderModel(Model *model, Vec3 position, Vec3 scale, Vec3 color, Quaternion quaternion);

	void R3D_RenderCube(Vec3 position, Vec3 scale, Vec3 color);
	void R3D_RenderCube(Vec3 position, Vec3 scale, Vec3 color, Texture texture);
	void R3D_RenderCube(Vec3 position, Vec3 scale, Vec3 color, Quaternion quaternion);
	void R3D_RenderCube(Vec3 position, Vec3 scale, Vec3 color, Texture texture, Quaternion quaternion);

	void R3D__CreateInstanceSSBO();
	void R3D_BeginInstanced(Model *model);
	void R3D_RenderInstancedModel(Vec3 position, Vec3 scale);
	void R3D_RenderInstancedModel(Vec3 position, Vec3 scale, Quaternion quaternion);
	void R3D_RenderInstanced();

	void R3D_SetDirectionalLight(Vec3 direction, Vec3 ambient, Vec3 diffuse, Vec3 specular);
	void R3D_SetPointLight(Vec3 position, Vec3 ambient, Vec3 diffuse, Vec3 specular, float constant, float linear, float quadratic);

	CW::Model *R3D_GetDefaultCubeModel();
	void R3D_UseShader(Shader *shader);
	void R3D_UseDefaultShader();
	void R3D__ResizeCallback(int screen_width, int screen_height);
}