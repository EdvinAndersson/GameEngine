#pragma once

#include "Project.h"
#include "../scene/SceneManager.h"
#include "../serialization/Serialize.h"
#include "../serialization/Deserialize.h"

namespace CW {

    class ProjectManager {
        public:
            Project* CreateProject(ProjectSpecification spec);
            void LoadProject(char *path);
            void LoadProject(Project *project);
            void SaveProject(char *file_path);
            void CloseProject(Project *project);

            inline Project* GetCurrentProject() { return current_project; }
            inline void SetSceneManager(SceneManager *_scene_manager) { scene_manager = _scene_manager; }
        private:
            Project *current_project;
            SceneManager *scene_manager;
    }; 
}