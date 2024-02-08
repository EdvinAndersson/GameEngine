#include "Renderer3D.h"
#include "../Utility.h"
#include "../vendor/stb_image/stb_image.h"
#include "Shaders.h"
#include "Models.h"
#include "Framebuffer.h"
//#include "raw_cube.h"

namespace CW {

    void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char *message, const void *userParam);

    struct R3D_Data {
        Window *window;

        Shader default_shader, active_shader, skybox_shader, instance_shader, depth_shader;
        Texture defualt_texture;

        const unsigned int frame_buffer_depth_width = 1024, frame_buffer_depth_height = 1024;
        Framebuffer framebuffer_depth = Framebuffer(FramebufferType::DEPTH, frame_buffer_depth_width, frame_buffer_depth_height);
        mat4s light_space_matrix;

        int point_lights = 0;

        Model *instance_model;
        mat4s *matrices;
        int max_instance_count = 100000, instance_count = 0;
        unsigned int instance_ssbo;

        unsigned int depth_map_fbo, depth_map;
    };

    static R3D_Data *g_r3d_data;

    void R3D_Init(Window *window) {
        g_r3d_data = new R3D_Data();

        if (g_r3d_data == NULL) {
            printf("ERROR: Could not allocate \"g_render_data\"");
            return;
        }
        
        int flags; 
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(glDebugOutput, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        }

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_FRAMEBUFFER_SRGB);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        stbi_set_flip_vertically_on_load(false);

        g_r3d_data->window = window;

        g_r3d_data->default_shader = CreateShader(vertex_shader, fragment_shader);
        g_r3d_data->skybox_shader = CreateShader(vertex_shader_skybox, fragment_shader_skybox);
        g_r3d_data->instance_shader = CreateShader(vertex_shader_instanced, fragment_shader);
        g_r3d_data->depth_shader = CreateShader(vertex_shader_depth, fragment_shader_depth);

        g_r3d_data->defualt_texture = AssetManager::Get()->GetTexture("default_texture");

        R3D_Resize(window->GetWidth(), window->GetHeight());
        R3D_SetViewModel(GLMS_MAT4_IDENTITY_INIT);

        for (int i = 0; i < 2; i++) {
            Shader shader = i == 0 ? g_r3d_data->default_shader : g_r3d_data->instance_shader;

            shader.Use();
            shader.SetInt("material.texture_diffuse1", 0);
            shader.SetInt("material.texture_specular1", 0);
            shader.SetInt("material.texture_normal1", 0);
            shader.SetInt("number_of_point_lights", 0);

            //shader.SetFloat("material.shininess", 0.078125f * 128.0f);
            shader.SetFloat("material.shininess", 0.078125f * 200.0f);

            shader.SetV3("dirLight.direction", vec3s{ 1, -2, 1 });
            shader.SetV3("dirLight.ambient", vec3s{ 0.4f, 0.4f, 0.4f });
            shader.SetV3("dirLight.diffuse", vec3s{ 0.9f, 0.9f, 0.9f });
            shader.SetV3("dirLight.specular", vec3s { 0.5f, 0.5f, 0.5f });

            mat4s view = GLMS_MAT4_IDENTITY_INIT;
            shader.SetMat4f("view", &view);
        }
        g_r3d_data->active_shader = g_r3d_data->default_shader;
        g_r3d_data->active_shader.Use();

        R3D__CreateInstanceSSBO();
    }
    void R3D_Clear(vec4s color) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(color.r, color.g, color.b, color.a);

        g_r3d_data->point_lights = 0;
        g_r3d_data->default_shader.Use();
        g_r3d_data->default_shader.SetInt("number_of_point_lights", g_r3d_data->point_lights);

        g_r3d_data->instance_shader.Use();
        g_r3d_data->instance_shader.SetInt("number_of_point_lights", g_r3d_data->point_lights);
    }
    void R3D_SetViewModel(mat4s view) {
        g_r3d_data->default_shader.Use();
        g_r3d_data->default_shader.SetMat4f("view", &view);

        g_r3d_data->instance_shader.Use();
        g_r3d_data->instance_shader.SetMat4f("view", &view);
    }
    void R3D_SetViewPos(vec3s position) {
        g_r3d_data->default_shader.Use();
        g_r3d_data->default_shader.SetV3("viewPos", position);

        g_r3d_data->instance_shader.Use();
        g_r3d_data->instance_shader.SetV3("viewPos", position);
    }

    void R3D_Resize(int screen_width, int screen_height) {
        mat4s projection = glms_perspective(45.0f * ((float)M_PI / 180.0f), (float)screen_width / (float)screen_height, 0.1f, 200.0f);

        g_r3d_data->default_shader.Use();
        g_r3d_data->default_shader.SetMat4f("projection", &projection);

        g_r3d_data->skybox_shader.Use();
        g_r3d_data->skybox_shader.SetMat4f("projection", &projection);

        g_r3d_data->instance_shader.Use();
        g_r3d_data->instance_shader.SetMat4f("projection", &projection);

        glViewport(0, 0, screen_width, screen_height);
    }

    void R3D_RenderSkybox(Texture skybox_texture, mat4s view) {
        glDepthFunc(GL_LEQUAL);

        mat4s m = view;
        m.m03 = 0;
        m.m13 = 0;
        m.m23 = 0;
        m.m33 = 1;

        m.m30 = 0;
        m.m31 = 0;
        m.m32 = 0;

        g_r3d_data->skybox_shader.Use();
        g_r3d_data->skybox_shader.SetMat4f("view", &m);
        g_r3d_data->skybox_shader.SetInt("skybox", 5);

        glActiveTexture(GL_TEXTURE0 + 5);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_texture.id);

        Mesh *mesh = AssetManager::Get()->GetMesh(AssetManager::Get()->GetDefaultMeshIndex());
        mesh->DrawMesh(&g_r3d_data->skybox_shader);

        glDepthFunc(GL_LESS);
    }
    void R3D_RenderMesh(MeshIndex mesh_index, MaterialIndex *material_indexes, vec3s position, vec3s scale, versors quaternion) {
        g_r3d_data->active_shader.Use();

        mat4s transform = GLMS_MAT4_IDENTITY_INIT;

        transform = glms_translate(transform, position);
        transform = glms_mat4_mul(glms_quat_mat4(quaternion), transform);
        transform = glms_scale(transform, scale);

        g_r3d_data->active_shader.SetMat4f("model", &transform);

        Mesh *mesh = AssetManager::Get()->GetMesh(mesh_index);
        mesh->DrawMesh(&g_r3d_data->active_shader, material_indexes);
    }

    void R3D__CreateInstanceSSBO() {
        g_r3d_data->matrices = new mat4s[g_r3d_data->max_instance_count];

        glGenBuffers(1, &g_r3d_data->instance_ssbo);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, g_r3d_data->instance_ssbo);
        glBufferData(GL_SHADER_STORAGE_BUFFER, 4 * sizeof(vec4s) * g_r3d_data->max_instance_count, nullptr, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, g_r3d_data->instance_ssbo);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }
    void R3D_BeginInstanced(Model *model) {
        g_r3d_data->instance_model = model;
        g_r3d_data->instance_count = 0;
    }
    void R3D_RenderInstancedModel(vec3s position, vec3s scale) {
        R3D_RenderInstancedModel(position, scale, GLMS_QUAT_IDENTITY_INIT);
    }
    void R3D_RenderInstancedModel(vec3s position, vec3s scale, versors quaternion) {
        #if 0
        Mat4 model = Mat4Identity();

        model = Mat4TranslateV3(model, position);
        model = QuatToMat4(quaternion) * model;
        model = Mat4ScaleV3(model, scale);

        g_r3d_data->matrices[g_r3d_data->instance_count] = model;

        g_r3d_data->instance_count++;
        #endif
    }
    void R3D_RenderInstanced() {
        #if 0
        if (g_r3d_data->instance_count <= 0) return;

        g_r3d_data->instance_shader.Use();
        g_r3d_data->instance_shader.SetV4("objectColor", 1, 1, 1, 1.0f);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, g_r3d_data->instance_ssbo);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, 4 * sizeof(vec4s) * g_r3d_data->instance_count, g_r3d_data->matrices);

        g_r3d_data->instance_model->DrawInstanced(&g_r3d_data->instance_shader, g_r3d_data->instance_count);

        g_r3d_data->instance_count = 0;
        #endif
    }
    void R3D_BeginShadowPass(vec3s light_pos) {
        g_r3d_data->framebuffer_depth.Bind();
        glViewport(0, 0, g_r3d_data->frame_buffer_depth_width, g_r3d_data->frame_buffer_depth_height);
        glClear(GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_FRONT);
        
        float near_plane = 1.0f, far_plane = 60.f;
        mat4s lightProjection = glms_ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        mat4s lightView = glms_lookat(light_pos, vec3s{ 0.0f, 0.0f,  0.0f}, vec3s{ 0.0f, 1.0f,  0.0f});
        g_r3d_data->light_space_matrix = glms_mat4_mul(lightProjection, lightView);

        CW::R3D_UseDefaultShader();
        g_r3d_data->default_shader.SetV3("tempLightPos", light_pos);

        CW::R3D_UseShader(&g_r3d_data->depth_shader);
        g_r3d_data->depth_shader.SetMat4f("lightSpaceMatrix", &g_r3d_data->light_space_matrix);
    }
    void R3D_EndShadowPass() {
        glCullFace(GL_BACK);
        g_r3d_data->framebuffer_depth.UnBind();

        CW::R3D_UseDefaultShader();
        g_r3d_data->default_shader.SetMat4f("lightSpaceMatrix", &g_r3d_data->light_space_matrix);
        g_r3d_data->default_shader.SetInt("shadowMap", 5);
        g_r3d_data->framebuffer_depth.GetTexture().Use(5);
    }
    void R3D_SetDirectionalLight(vec3s direction, vec3s ambient, vec3s diffuse, vec3s specular) {
        g_r3d_data->active_shader.Use();
        g_r3d_data->active_shader.SetV3("dirLight.direction", direction);
        g_r3d_data->active_shader.SetV3("dirLight.ambient", ambient);
        g_r3d_data->active_shader.SetV3("dirLight.diffuse", diffuse);
        g_r3d_data->active_shader.SetV3("dirLight.specular", specular);
    }
    void R3D_SetPointLight(vec3s position, vec3s ambient, vec3s diffuse, vec3s specular, float constant, float linear, float quadratic) {
        g_r3d_data->active_shader.Use();

        std::string pl = std::to_string(g_r3d_data->point_lights);
        std::string var = "pointLights[" + pl + "].position";
        g_r3d_data->active_shader.SetV3(var.c_str(), position);

        var = "pointLights[" + pl + "].ambient";
        g_r3d_data->active_shader.SetV3(var.c_str(), ambient);
        var = "pointLights[" + pl + "].diffuse";
        g_r3d_data->active_shader.SetV3(var.c_str(), diffuse);
        var = "pointLights[" + pl + "].specular";
        g_r3d_data->active_shader.SetV3(var.c_str(), specular);

        var = "pointLights[" + pl + "].constant";
        g_r3d_data->active_shader.SetFloat(var.c_str(), constant);
        var = "pointLights[" + pl + "].linear";
        g_r3d_data->active_shader.SetFloat(var.c_str(), linear);
        var = "pointLights[" + pl + "].quadratic";
        g_r3d_data->active_shader.SetFloat(var.c_str(), quadratic);

        g_r3d_data->point_lights++;
        g_r3d_data->active_shader.SetInt("number_of_point_lights", g_r3d_data->point_lights);
    }
    void R3D_UseShader(Shader *shader) {
        shader->Use();
        g_r3d_data->active_shader = *shader;
    }
    void R3D_UseDefaultShader() {
        R3D_UseShader(&g_r3d_data->default_shader);
    }

    Shader R3D_GetDefaultShader() {
        return g_r3d_data->default_shader;
    }

    void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char *message, const void *userParam) {
        // ignore non-significant error/warning codes
        if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

        printf("---------------");
        printf("Debug message (%i): %s", id, message);

        switch (source)
        {
            case GL_DEBUG_SOURCE_API:             printf("Source: API"); break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   printf("Source: Window System"); break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER: printf("Source: Shader Compiler"); break;
            case GL_DEBUG_SOURCE_THIRD_PARTY:     printf("Source: Third Party"); break;
            case GL_DEBUG_SOURCE_APPLICATION:     printf("Source: Application"); break;
            case GL_DEBUG_SOURCE_OTHER:           printf("Source: Other"); break;
        } printf("\n");

        switch (type)
        {
            case GL_DEBUG_TYPE_ERROR:               printf("Type: Error"); break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: printf("Type: Deprecated Behaviour"); break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  printf("Type: Undefined Behaviour"); break; 
            case GL_DEBUG_TYPE_PORTABILITY:         printf("Type: Portability"); break;
            case GL_DEBUG_TYPE_PERFORMANCE:         printf("Type: Performance"); break;
            case GL_DEBUG_TYPE_MARKER:              printf("Type: Marker"); break;
            case GL_DEBUG_TYPE_PUSH_GROUP:          printf("Type: Push Group"); break;
            case GL_DEBUG_TYPE_POP_GROUP:           printf("Type: Pop Group"); break;
            case GL_DEBUG_TYPE_OTHER:               printf("Type: Other"); break;
        } printf("\n");
        
        switch (severity)
        {
            case GL_DEBUG_SEVERITY_HIGH:         printf("Severity: high"); break;
            case GL_DEBUG_SEVERITY_MEDIUM:       printf("Severity: medium"); break;
            case GL_DEBUG_SEVERITY_LOW:          printf("Severity: low"); break;
            case GL_DEBUG_SEVERITY_NOTIFICATION: printf("Severity: notification"); break;
        } printf("\n");

        printf("\n");
    }
}