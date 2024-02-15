#include "ProjectManager.h"
#include "CWAssert.h"
#include "Utility.h"
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "../event/EventManager.h"

#include "Rendering/Renderer3D.h"

namespace CW {

    #define PROJECT_FILE_SIZE 1024*1024

    Project* ProjectManager::CreateProject(char *project_folder_path, ProjectSpecification spec) {
        Project *project = new Project();
        project->specification = spec;
        strcpy(project->project_folder_path, project_folder_path);

        if (!project_folder_path && !FolderExists(project_folder_path)) {
            int result = FolderCreate(project_folder_path);

            CW_ASSERT(result == 0, "Project folder could not be created.");
        }

        EventData_PROJECT_CREATE e = { project };
        EventManager::InvokeEvent(PROJECT_CREATE, &e);
        
        return project;
    }
    char *GetStrBetween(char *data, char b, char e, int *offset){
        char *begin = strchr(data, b) + 1;
        char *end = strchr(data, e);

        CW_ASSERT(end > begin, "");

        char *str = (char *) calloc(end - begin + 1, 1);
        strncpy(str, begin, end - begin);

        *offset = (int) (end - data) + 1;

        return str;
    }
    
    void ProjectManager::LoadProject(char *project_file_path) {
        FILE *file = fopen(project_file_path, "r");

        if (!file) {
            printf("Falied to load project: %s\n", project_file_path);
            return;
        }

        if (current_project != 0)
            CloseProject(current_project);

        ProjectSpecification project_specification;

        printf("Loading project... %s\n", project_file_path);

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
        project_specification.windowed_size = vec2s { (float) deserialized.GetInt(), (float) deserialized.GetInt() };
        int scene_count = deserialized.GetInt();

        // Get the project folder path
        char *last_occurrence_of_slash = strrchr(project_file_path, '/');
        char project_folder_path[1024] = {};
        if (last_occurrence_of_slash != 0) {
            int project_folder_path_lenght = last_occurrence_of_slash - project_file_path;
            strncpy(project_folder_path, project_file_path, project_folder_path_lenght);
        }

        current_project = CreateProject(project_folder_path, project_specification); 
        
        EventData_PROJECT_LOAD e = { current_project };
        EventManager::InvokeEvent(PROJECT_LOAD, &e);

        EventData_PROJECT_LOAD_LATE e2 = { current_project };
        EventManager::InvokeEvent(PROJECT_LOAD_LATE, &e2);

        for (unsigned int i = 0; i < scene_count; i++) {
            scene_manager->CreateNewScene(current_project, deserialized.GetString());

            int game_object_count = deserialized.GetInt();

            for (unsigned int j = 0; j < game_object_count; j++) {
                int entity_id = deserialized.GetInt();
                Signature signature;
                char *str_signature = deserialized.GetString();
                for (int i = 0; i < MAX_COMPONENTS; i++)
                    signature.set(i, (int) (str_signature[i] - '0'));

                vec3s pos = deserialized.GetVec3();
                vec3s rotation = deserialized.GetVec3();
                vec3s scale = deserialized.GetVec3();

                GameObject obj = GameObject::Instantiate(pos);
                CW::Transform& transform = obj.GetComponent<CW::Transform>();
                transform.rotation = rotation;
                transform.scale = scale;

                if (signature.test(1)) {
                    CW::MeshRenderer& mesh_renderer = obj.AddComponent<CW::MeshRenderer>();
                    mesh_renderer.mesh = deserialized.GetSize_t();
                    mesh_renderer.material_count = deserialized.GetInt();
                    for (int m = 0; m < mesh_renderer.material_count; m++)
                        mesh_renderer.materials[m] = deserialized.GetSize_t();
                }
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
    void ProjectManager::SaveProject() {
        char full_path[1024] = {0};
        strcpy(full_path, current_project->project_folder_path);
        strcat(full_path, "/");
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

                char str_signature[MAX_COMPONENTS+1];
                for (int c = 0; c < MAX_COMPONENTS; c++)
                {
                    str_signature[c] = signature.test(c) ? '1' : '0';
                }           
                str_signature[MAX_COMPONENTS] = 0;      
                serialized.SubmitString(str_signature);
                
                CW::Transform& transform = obj.GetComponent<CW::Transform>();
                serialized.SubmitVec3(transform.position);
                serialized.SubmitVec3(transform.rotation);
                serialized.SubmitVec3(transform.scale);

                if (obj.HasComponent<CW::MeshRenderer>()) {
                    CW::MeshRenderer& mesh_renderer = obj.GetComponent<CW::MeshRenderer>();
                    serialized.SubmitSize_t(mesh_renderer.mesh);
                    serialized.SubmitSize_t(mesh_renderer.material_count);
                    for (int m = 0; m < mesh_renderer.material_count; m++)
                        serialized.SubmitSize_t(mesh_renderer.materials[m]);
                }
            }
        }
        fwrite(serialized.GetData(), 1, strlen(serialized.GetData()), file);

        fclose(file);
    }

    void ProjectManager::CloseProject(Project *project) {
        delete project;
    }
}