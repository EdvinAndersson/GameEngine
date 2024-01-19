#pragma once

#include "Project.h"
#include "../scene/SceneManager.h"
#include "../serialization/Serialize.h"
#include "../serialization/Deserialize.h"

namespace CW {

    class ProjectManager {
        public:
            Project* CreateProject(char *projects_folder_path, ProjectSpecification spec);
            void LoadProject(char *project_file_path);
            void LoadProject(Project *project);
            void SaveProject();
            void CloseProject(Project *project);

            inline Project* GetCurrentProject() { return current_project; }
            inline void SetSceneManager(SceneManager *_scene_manager) { scene_manager = _scene_manager; }
        private:
            Project *current_project;
            SceneManager *scene_manager;
    }; 
}