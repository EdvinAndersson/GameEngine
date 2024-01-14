#include "ProjectManager.h"
#include "Utility/Utility.h"
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "../event/EventManager.h"

namespace CW {

    #define PROJECT_FILE_SIZE 1024*1024

    void ProjectManager::CreateProject(ProjectSpecification spec) {
        Project project;
        project.specification = spec;

        LoadProject(project);
    }
    char *GetStrBetween(char *data, char b, char e, int *offset){
        char *begin = strchr(data, b) + 1;
        char *end = strchr(data, e);

        Assert(end > begin);

        char *str = (char *) calloc(end - begin + 1, 1);
        strncpy(str, begin, end - begin);

        *offset = (int) (end - data) + 1;

        return str;
    }
    
    void ProjectManager::LoadProject(char *path) {
        FILE *file = fopen(path, "r");

        if (!file) {
            printf("Falied to load project: %s\n", path);
            return;
        }
        Project project;

        printf("Loading project... %s\n", path);

        //Get project file size
        fseek(file, 0, SEEK_END);
        long project_file_size = ftell(file);
        fseek(file, 0, 0);

        char *data = (char *) calloc(project_file_size, 1);
        fread(data, 1, project_file_size, file);
        
        int offset = 0, temp_offset = 0;

        //Get project name
        char *project_name_end = strchr(data, '\n');
        int project_name_size = (int) (project_name_end - data);
        project.specification.project_name = (char*) calloc(project_name_size + 1, 1);
        strncpy(project.specification.project_name, data, project_name_size);
        offset += project_name_size;

        //Get resolution mode
        char *resolution_mode = GetStrBetween(data + offset, '{', '}', &temp_offset);
        offset += temp_offset;
        project.specification.resolution_mode = (ResolutionMode) (resolution_mode[0] - 0x30);
        delete resolution_mode;

        //Get windowed size
        char *windowed_size = GetStrBetween(data + offset, '{', '}', &temp_offset);
        offset += temp_offset;
        char *windowed_size_seperator = strchr(windowed_size, ',');
        char width[10] = {0}, height[10] = {0};
        strncpy(width, windowed_size, windowed_size_seperator - windowed_size);
        strncpy(height, windowed_size_seperator + 1, sizeof(windowed_size) - (windowed_size_seperator - windowed_size));
        project.specification.windowed_size = Vec2 { (float) StrToInt(width), (float) StrToInt(height) };
        delete windowed_size;

        //Get vsync
        char *vsync = GetStrBetween(data + offset, '{', '}', &temp_offset);
        offset += temp_offset;
        project.specification.vsync = (int) (vsync[0] - 0x30);
        delete vsync;

        //Get scene count
        char *scene_count_raw = GetStrBetween(data + offset, '{', '}', &temp_offset);
        offset += temp_offset;
        int scene_count = StrToInt(scene_count_raw);
        delete scene_count_raw;

        printf("Name: %s\n", project.specification.project_name);
        printf("Resolution mode: %i\n", project.specification.resolution_mode);
        printf("Windowed size: (%i, %i)\n", (int) project.specification.windowed_size.width, (int) project.specification.windowed_size.height);
        printf("Vsync: %i\n", (int) project.specification.vsync);
        printf("Scene count: %i\n", scene_count);

        //printf("\nData: %s\n", data);
        delete data;

        LoadProject(project);
    }
    void ProjectManager::LoadProject(Project project) {
        if (current_project.loaded)
            CloseProject(current_project);

        current_project = project;
        current_project.loaded = true;

        EventData_PROJECT_LOAD e = { current_project };
        EventManager::InvokeEvent(PROJECT_LOAD, &e);
    }
    void ProjectManager::SaveProject(char *path_to_project) {
        char full_path[1024] = {0};
        strcpy(full_path, path_to_project);

        strcat(full_path, current_project.specification.project_name);
        strcat(full_path, ".proj");

        printf("path %s\n", full_path);

        FILE *file = fopen(full_path, "w");

        if (!file) {
            printf("Falied to save project: %s\n", full_path);
            return;
        }
    
        char *project_data = (char *) calloc(PROJECT_FILE_SIZE, 1);

        //Project name
        char project_name[1024];
        sprintf(project_name, "%s\n", current_project.specification.project_name);
        strcat(project_data, project_name);

        //Resoultion mode
        char resolution_mode[50] = {0};
        sprintf(resolution_mode, "Resolution mode {%i}\n", current_project.specification.resolution_mode);
        strcat(project_data, resolution_mode);

        //Windowed size
        char windowed_size[50] = {0};
        sprintf(windowed_size, "Windowed size {%i,%i}\n", (int)current_project.specification.windowed_size.width, (int)current_project.specification.windowed_size.height);
        strcat(project_data, windowed_size);

        //Vsync
        char vsync[50] = {0};
        sprintf(vsync, "Vsync {%i}\n", current_project.specification.vsync);
        strcat(project_data, vsync);

        //Scene count
        char scene_count[50] = {0};
        sprintf(scene_count, "Scene count {%i}\n", scene_manager->GetSceneCount());
        strcat(project_data, scene_count);

        //Scenes
        char *scenes = (char *) calloc((size_t) (PROJECT_FILE_SIZE * 0.8), 1);
        sprintf(scenes, "Scenes {\n");
        for (unsigned int i = 0; i < scene_manager->GetSceneCount(); i++) {
            strcat(scenes, "\t{\n");
            sprintf(scenes + strlen(scenes), "\tScene name {%s}\n", scene_manager->GetScenes()[i].name);
            sprintf(scenes + strlen(scenes), "\tGame objects {%i}\n", scene_manager->GetScenes()[i].game_objects.size());
            strcat(scenes, "\t}\n");
        }
        strcat(scenes, "}\n");
        strcat(project_data, scenes);

        //Write to file
        fwrite(project_data, 1, strlen(project_data), file);

        fclose(file);

        delete project_data;
    }

    void ProjectManager::CloseProject(Project project) {
        delete project.specification.project_name;
    }
}