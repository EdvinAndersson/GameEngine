#include "ProjectManager.h"
#include "Utility/Utility.h"
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "../event/EventManager.h"

#include "Utility/Rendering/Renderer3D.h"

namespace CW {

    #define PROJECT_FILE_SIZE 1024*1024

    Project* ProjectManager::CreateProject(ProjectSpecification spec) {
        Project *project = new Project();
        project->specification = spec;

        return project;
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

        if (current_project != 0)
            CloseProject(current_project);

        ProjectSpecification project_specification;

        printf("Loading project... %s\n", path);

        //Get project file size
        fseek(file, 0, SEEK_END);
        long project_file_size = ftell(file);
        fseek(file, 0, 0);

        char *data = (char *) calloc(project_file_size, 1);
        fread(data, 1, project_file_size, file);
        
        Deserialize deserialized(data, ';');

        project_specification.project_name = deserialized.GetString();
        project_specification.resolution_mode = (CW::ResolutionMode) deserialized.GetInt();
        project_specification.vsync = deserialized.GetInt();
        project_specification.windowed_size = Vec2 { (float) deserialized.GetInt(), (float) deserialized.GetInt() };
        int scene_count = deserialized.GetInt();

        printf("Name: %s\n", project_specification.project_name);
        printf("Resolution mode: %i\n", project_specification.resolution_mode);
        printf("Windowed size: (%i, %i)\n", (int) project_specification.windowed_size.width, (int) project_specification.windowed_size.height);
        printf("Vsync: %i\n", (int) project_specification.vsync);
        printf("Scene count: %i\n", scene_count);

        current_project = CreateProject(project_specification); 
        
        EventData_PROJECT_LOAD e = { current_project };
        EventManager::InvokeEvent(PROJECT_LOAD, &e);

        for (unsigned int i = 0; i < scene_count; i++) {
            scene_manager->CreateNewScene(current_project, deserialized.GetString());

            int game_object_count = deserialized.GetInt();

            for (unsigned int j = 0; j < game_object_count; j++) {
                int entity_id = deserialized.GetInt();
                Signature signature = deserialized.GetInt();

                Vec3 pos = { (int) deserialized.GetInt(), (int) deserialized.GetInt(), (int) deserialized.GetInt() };
                GameObject obj = GameObject::Instantiate(pos);

                CW::ModelRenderer& m = obj.AddComponent<CW::ModelRenderer>();
                m.model = R3D_GetDefaultCubeModel();
            }
        }
        delete data;
    }
    void ProjectManager::LoadProject(Project *project) {
        if (current_project != 0)
            CloseProject(current_project);

        current_project = project;

        EventData_PROJECT_LOAD e = { current_project };
        EventManager::InvokeEvent(PROJECT_LOAD, &e);
    }
    void ProjectManager::SaveProject(char *path_to_project) {
        char full_path[1024] = {0};
        strcpy(full_path, path_to_project);

        strcat(full_path, current_project->specification.project_name);
        strcat(full_path, ".proj");

        printf("path %s\n", full_path);

        FILE *file = fopen(full_path, "w");

        if (!file) {
            printf("Falied to save project: %s\n", full_path);
            return;
        }

        Serialize serialized(PROJECT_FILE_SIZE, ';');

        serialized.SubmitString(current_project->specification.project_name);
        serialized.SubmitInt(current_project->specification.resolution_mode);
        serialized.SubmitInt(current_project->specification.vsync);
        serialized.SubmitInt(current_project->specification.windowed_size.x);
        serialized.SubmitInt(current_project->specification.windowed_size.y);
        serialized.SubmitInt(scene_manager->GetSceneCount());
        for (unsigned int i = 0; i < scene_manager->GetSceneCount(); i++) {
            Scene& scene = scene_manager->GetScenes()[i];
            serialized.SubmitString(scene.name);
            serialized.SubmitInt(scene.game_objects.size());

            for (GameObject obj : scene.game_objects) {
                int component_count = 0;
                Signature signature = obj.GetSignature();
                for (int c = 0; c < MAX_COMPONENTS; c++)
                    if (signature.test(c)) component_count++;

                serialized.SubmitInt(obj.GetEntity());
                serialized.SubmitInt(1);
                //serialized.SubmitInt(component_count);

                if (obj.HasComponent<CW::Transform>()) {
                    CW::Transform& transform = obj.GetComponent<CW::Transform>();
                    serialized.SubmitInt((int)transform.position.x);
                    serialized.SubmitInt((int)transform.position.y);
                    serialized.SubmitInt((int)transform.position.z);
                }

                for (int c = 0; c < MAX_COMPONENTS; c++) {
                    if (signature.test(c)) {
                        //serialized.SubmitInt(c);
                    }
                }
            }
        }
        fwrite(serialized.GetData(), 1, strlen(serialized.GetData()), file);

        fclose(file);
    }

    void ProjectManager::CloseProject(Project *project) {
        delete project->specification.project_name;
        delete project;
    }
}