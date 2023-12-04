#include "Renderer2D.h"
#include "../vendor/stb_image/stb_image.h"

namespace GL {

    struct Vertex {
        Vec2 position;
        Vec2 tex_coords;
        Vec4 color;
        float tex_id, use_bilinear;
    };

    struct R2D_Data {
        Shader default_shader, active_shader;
        Texture defualt_texture;

        Vertex *vertices, *vertices_begin;
        unsigned int MAX_QUADS = 1000, MAX_INDICES, MAX_VERTICES;
        unsigned int batch_vertex_array = 0, batch_vertex_buffer = 0, current_quad_count = 0;

        unsigned int texture_3d = 0;
        unsigned int max_texture_count = 256;
        unsigned int next_texture_handle = 1;
    };

    static R2D_Data *g_r2d_data;

    void R2D_Init(Window *window) {
        g_r2d_data = new R2D_Data();

        if (g_r2d_data == NULL) {
            printf("ERROR: Could not allocate \"g_render_data\"");
            return;
        }

        stbi_set_flip_vertically_on_load(true);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        window->_SetOnResizeCallbakRenderer2D(R2D__ResizeCallback);

        std::string vertex_shader = R"(

            #version 330 core
            layout (location = 0) in vec2 a_pos;
            layout (location = 1) in vec2 a_tex_coords;
            layout (location = 2) in vec4 a_color;
            layout (location = 3) in float a_tex_index;
            layout (location = 4) in float a_use_bilinear;

            out vec2 f_tex_coords;
            out vec4 f_color;  
            flat out float f_tex_index;
            flat out float f_use_bilinear;

            uniform mat4 view;
            uniform mat4 projection;

            void main()
            {
                f_color = a_color;
                f_tex_coords = a_tex_coords;
                f_tex_index = a_tex_index;
                f_use_bilinear = a_use_bilinear;
                gl_Position = projection * view * vec4(a_pos, 0.0, 1.0);
            }

            )";
        std::string fragment_shader = R"(

            #version 330 core

            in vec4 f_color;
            in vec2 f_tex_coords;
            flat in float f_tex_index;
            flat in float f_use_bilinear;
            out vec4 FragColor;

            uniform sampler2DArray texture_array;

            void main()
            {
                ivec3 texture_size = textureSize(texture_array, 0);
                vec2 uv = f_tex_coords;
                if (f_use_bilinear == 0) {
                    uv = (floor(f_tex_coords * ivec2(texture_size)) + 0.5) / ivec2(texture_size);
                }
                vec3 array_uv = vec3(uv.x, uv.y, f_tex_index);
	            FragColor = f_color * texture(texture_array, array_uv);
            }

            )";

        g_r2d_data->default_shader = GL::CreateShader(vertex_shader, fragment_shader);
        g_r2d_data->active_shader = g_r2d_data->default_shader;

        GL::Texture_Format format = { GL_RGBA, GL::TEXTURE_NEAREST_NEIGHBOR };
        g_r2d_data->defualt_texture = GL::CreateBlankTexture(format);

        R2D__ResizeCallback(window->GetWidth(), window->GetHeight());

        g_r2d_data->active_shader.Use();
        g_r2d_data->active_shader.SetInt("image", 0);
        Mat4 view = Mat4Identity();
        g_r2d_data->active_shader.SetMat4f("view", &view);
        g_r2d_data->active_shader.SetInt("texture_array", GL_TEXTURE0);

        //Batch Data
        {
            g_r2d_data->MAX_VERTICES = g_r2d_data->MAX_QUADS * 4;
            g_r2d_data->MAX_INDICES = g_r2d_data->MAX_QUADS * 6;

            g_r2d_data->vertices = new Vertex[g_r2d_data->MAX_VERTICES]; //delete memoy
            g_r2d_data->vertices_begin = g_r2d_data->vertices;

            unsigned int ebo;

            glGenVertexArrays(1, &g_r2d_data->batch_vertex_array);
            glGenBuffers(1, &g_r2d_data->batch_vertex_buffer);
            glGenBuffers(1, &ebo);

            glBindVertexArray(g_r2d_data->batch_vertex_array);
            glBindBuffer(GL_ARRAY_BUFFER, g_r2d_data->batch_vertex_buffer);
            glBufferData(GL_ARRAY_BUFFER, g_r2d_data->MAX_VERTICES * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
           

            unsigned int* indices = new unsigned int[g_r2d_data->MAX_INDICES];
            int offset = 0;
            for (unsigned int i = 0; i < g_r2d_data->MAX_INDICES; i += 6) {
                indices[i + 0] = 0 + offset;
                indices[i + 1] = 1 + offset;
                indices[i + 2] = 2 + offset;
                indices[i + 3] = 2 + offset;
                indices[i + 4] = 3 + offset;
                indices[i + 5] = 0 + offset;

                offset += 4;
            }
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * g_r2d_data->MAX_INDICES, indices, GL_STATIC_DRAW);
            delete[] indices;

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
            
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tex_coords));
            
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, color));
            
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tex_id));

            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, use_bilinear));

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
        g_r2d_data->active_shader.SetInt("texture_array", GL_TEXTURE0);

        R2D__CreateTexture3D();
        R2D__BeginBatch();
    }
    void R2D_Clear(Vec4 color) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void R2D__ResizeCallback(int screen_width, int screen_height) {
        Mat4 projection = Mat4Ortho(0.0f, (float)screen_width, (float)screen_height, 0.0f, -1.0f, 1.0f);

        g_r2d_data->default_shader.Use();
        g_r2d_data->default_shader.SetMat4f("projection", &projection);

        glViewport(0, 0, screen_width, screen_height);
    }
    
    void R2D_RenderRect(Vec2 position, Vec2 size, Vec4 color) {
        R2D_RenderSprite(position, size, R2D_Texture { 1, 0, 1, 1}, color, 0, {0});
    }
    void R2D_RenderRect(Vec2 position, Vec2 size, Vec4 color, float rotation, Vec2 pivot) {
        R2D_RenderSprite(position, size, R2D_Texture{ 1, 0, 1, 1 }, color, rotation, pivot);
    }
    void R2D_RenderSprite(Vec2 position, Vec2 size, R2D_Texture texture) {
        R2D_RenderSprite(position, size, texture, Vec4 { 1.0f, 1.0f, 1.0f, 1.0f }, 0, {0});
    }
    void R2D_RenderSprite(Vec2 position, Vec2 size, R2D_Texture texture, Vec4 color) {
        R2D_RenderSprite(position, size, texture, color, 0, { 0 });
    }
    void R2D_RenderSprite(Vec2 position, Vec2 size, R2D_Texture texture, Vec4 color, float rotation, Vec2 pivot) {
        
        if (g_r2d_data->current_quad_count >= g_r2d_data->MAX_QUADS) {
            R2D_Flush();
        }

        g_r2d_data->active_shader.Use();
        float texture_id = (float) texture.handle;

        pivot = position + Vec2{ pivot.x * size.x, pivot.y * size.y };
        Vec2 pos = position;
        g_r2d_data->vertices->position = pivot + Vec2Rotate(pos - pivot, ((float)M_PI / 180.0f) * rotation);
        g_r2d_data->vertices->tex_coords = Vec2{ 0.0f, 0.0f } / 256.0f;
        g_r2d_data->vertices->color = color;
        g_r2d_data->vertices->tex_id = texture_id;
        g_r2d_data->vertices->use_bilinear = texture.use_bilinear;
        g_r2d_data->vertices++;

        pos = Vec2{ position.x + size.x, position.y };
        g_r2d_data->vertices->position = pivot + Vec2Rotate(pos - pivot, ((float)M_PI / 180.0f) * rotation);
        g_r2d_data->vertices->tex_coords = Vec2{ (float) texture.width, 0.0f } / 256.0f;
        g_r2d_data->vertices->color = color;
        g_r2d_data->vertices->tex_id = texture_id;
        g_r2d_data->vertices->use_bilinear = texture.use_bilinear;
        g_r2d_data->vertices++;

        pos = position + size;
        g_r2d_data->vertices->position = pivot + Vec2Rotate(pos - pivot, ((float)M_PI / 180.0f) * rotation);
        g_r2d_data->vertices->tex_coords = Vec2{ (float) texture.width, (float)texture.height } / 256.0f;
        g_r2d_data->vertices->color = color;
        g_r2d_data->vertices->tex_id = texture_id;
        g_r2d_data->vertices->use_bilinear = texture.use_bilinear;
        g_r2d_data->vertices++;

        pos = Vec2{ position.x, position.y + size.y };
        g_r2d_data->vertices->position = pivot + Vec2Rotate(pos - pivot, ((float)M_PI / 180.0f) * rotation);
        g_r2d_data->vertices->tex_coords = Vec2{ 0.0f, (float) texture.height } / 256.0f;
        g_r2d_data->vertices->color = color;
        g_r2d_data->vertices->tex_id = texture_id;
        g_r2d_data->vertices->use_bilinear = texture.use_bilinear;
        g_r2d_data->vertices++;

        g_r2d_data->current_quad_count++;
    }

    void R2D_Flush() {
        R2D__EndBatch();

        g_r2d_data->active_shader.Use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, g_r2d_data->texture_3d);

        //Flush data
        glBindVertexArray(g_r2d_data->batch_vertex_array);
        glDrawElements(GL_TRIANGLES, g_r2d_data->current_quad_count * 6, GL_UNSIGNED_INT, 0);

        R2D__BeginBatch();
    }
    void R2D__BeginBatch() {
        g_r2d_data->current_quad_count = 0;
        g_r2d_data->vertices = g_r2d_data->vertices_begin;
    }
    void R2D__EndBatch() {
        GLsizeiptr size = (uint8_t *)g_r2d_data->vertices - (uint8_t *)g_r2d_data->vertices_begin;
        glBindBuffer(GL_ARRAY_BUFFER, g_r2d_data->batch_vertex_buffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, g_r2d_data->vertices_begin);
    }

    void R2D__CreateTexture3D() {
        glGenTextures(1, &g_r2d_data->texture_3d);
        glBindTexture(GL_TEXTURE_2D_ARRAY, g_r2d_data->texture_3d);
        glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, 256, 256, g_r2d_data->max_texture_count);

        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        unsigned int white = 0xffffffff;

        glBindTexture(GL_TEXTURE_2D_ARRAY, g_r2d_data->texture_3d);
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 1, 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char *)&white);
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

        g_r2d_data->next_texture_handle = 2;
    }

    R2D_Texture R2D_AddTexture(const char *path, GL::Texture_Filter texture_filter) {
        GL::TextureData texture_data = GL::GetTextureData(path);
        R2D_Texture texture = {};
        texture.handle = g_r2d_data->next_texture_handle;
        texture.use_bilinear = texture_filter == GL::TEXTURE_LINEAR_MIPMAP;
        texture.width = texture_data.width;
        texture.height = texture_data.height;

        glBindTexture(GL_TEXTURE_2D_ARRAY, g_r2d_data->texture_3d);
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, g_r2d_data->next_texture_handle, texture.width, texture.height, 1, GL_RGBA, GL_UNSIGNED_BYTE, texture_data.data);
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

        GL::FreeTextureData(&texture_data);

        g_r2d_data->next_texture_handle++;

        return texture;
    }
    void R2D_UseShader(Shader *shader) {
        shader->Use();
        g_r2d_data->active_shader = *shader;
    }
}