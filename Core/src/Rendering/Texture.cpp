#include "texture.h"
#include "../vendor/stb_image/stb_image.h"

namespace CW {
    void Texture::Use(int textureSlot) {
        glActiveTexture(GL_TEXTURE0 + textureSlot);
        glBindTexture(GL_TEXTURE_2D, id);
    }

    TextureData* CreateTexture(const char *path) {
        Texture_Format texture_format;
        texture_format.color_format = GL_RGBA;
        texture_format.texture_filtering = TEXTURE_NEAREST_NEIGHBOR;

        return CreateTexture(path, texture_format);
    }
    TextureData* CreateTexture(const char *path, Texture_Format texture_format) {
        TextureData *texture_data = GetTextureData(path);
        texture_data->texture_format = texture_format;
        texture_data->texture = CreateTextureFromData(texture_data->data, texture_data->width, texture_data->height, texture_format);

        return texture_data;
    }
    void FreeTextureData(TextureData *texture_data) {
        stbi_image_free(texture_data->data);
    }
    TextureData* CreateBlankTexture(Texture_Format texture_format) {
        TextureData *texture_data = new TextureData();
        texture_data->width = 1;
        texture_data->height = 1;
        unsigned int data = 0xFFFFFFFF;
        texture_data->data = (unsigned char*) &data;

        //texture_data->texture = CreateTextureFromData(texture_data->data, 1, 1, texture_format);
        texture_data->texture = CreateTextureFromData((unsigned char*) &data, 1, 1, texture_format);

        return texture_data;
    }

    Texture CreateTextureFromData(unsigned char *data, int width, int height, Texture_Format texture_format) {
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        switch (texture_format.texture_filtering) {
            case TEXTURE_NEAREST_NEIGHBOR:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                break;
            default:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                break;
        }

        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, texture_format.color_format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            printf("Failed to load texture!\n");
        }

        return Texture { texture };
    }
    TextureData* GetTextureData(const char *path) {
        TextureData *texture_data = new TextureData();
        int nr_channels;
        texture_data->data = stbi_load(path, &texture_data->width, &texture_data->height, &nr_channels, 0);

        return texture_data;
    }
    Texture CreateCubemapTexture(const char *right, const char *left, const char *top, const char *bottom, const char *front, const char *back) {
        unsigned int texture_id;

        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

        stbi_set_flip_vertically_on_load(false);
        _LoadFace(right, 0);
        _LoadFace(left, 1);
        _LoadFace(top, 2);
        _LoadFace(bottom, 3);
        _LoadFace(front, 4);
        _LoadFace(back, 5);
        stbi_set_flip_vertically_on_load(true);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        Texture texture;
        texture.id = texture_id;

        return texture;
    }

    void _LoadFace(const char *face, unsigned int i) {
        int width, height, nrChannels;

        unsigned char *data = stbi_load(face, &width, &height, &nrChannels, 0);

        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            printf("Cubemap tex failed to load at path: %s\n", face);
            stbi_image_free(data);
        }
    }
}