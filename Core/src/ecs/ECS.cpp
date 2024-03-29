#include "ECS.h"

#include "EntityManager.h"
#include "ComponentManager.h"
#include "Components.h"

namespace CW {

    EntityManager *entity_manager = new EntityManager();
    ComponentManager *component_manager = new ComponentManager();

    ECS::ECS(bool build_scripts_dll) {
        this->build_scripts_dll = build_scripts_dll;

        EventListen(PROJECT_LOAD);
    }

    inline void UpdateMeshRenderers(ComponentArray<MeshRenderer> *components) {
        size_t size = components->GetComponentArraySize();
        auto comp_datas = components->GetComponentArrayData();

        for (int i = 0; i < size; i++) {
            GameObject game_object = GameObject { components->GetEntity(i) };
            Transform& transform = game_object.GetComponent<Transform>();
            
            mat4s mat = glms_euler_xyz(transform.rotation);
            versors quat = glms_mat4_quat(mat);
            R3D_RenderMesh(comp_datas[i].mesh, comp_datas[i].materials, comp_datas[i].material_count, transform.position, transform.scale, quat);
        }
    }
    inline void UpdateCameras(ComponentArray<Camera> *components) {
        size_t size = components->GetComponentArraySize();
        auto comp_datas = components->GetComponentArrayData();
        for (int i = 0; i < size; i++) {
            GameObject game_object = GameObject { components->GetEntity(i) };
            Transform& transform = game_object.GetComponent<Transform>();
            Camera& camera = game_object.GetComponent<Camera>();
            if (camera.is_main) {
                CW::g_main_camera->pos = transform.position;
                CW::g_main_camera->pos.x = -CW::g_main_camera->pos.x;
                CW::g_main_camera->rotation = transform.rotation;
                CW::g_main_camera->fov = camera.fov;
            }
        }
    }

    void ECS::UpdateBaseComponents() {
        {
            ComponentArray<MeshRenderer> *comp = component_manager->GetComponentArray<MeshRenderer>();
            UpdateMeshRenderers(comp);
        }
        {
            ComponentArray<Camera> *comp = component_manager->GetComponentArray<Camera>();
            UpdateCameras(comp);
        }
    }
    void ECS::UpdateComponenets() {
        UpdateBaseComponents();
        CW::UpdateGeneratedComponents();
    }
    void ECS::OnEvent(Event event) {
        switch (event.event_type)
        {
            case EventType::PROJECT_LOAD:
            {
                component_manager->ResetComponentArrays();
                entity_manager->ResetEntities();

                component_manager->RegisterComponent<Transform>();
                component_manager->RegisterComponent<MeshRenderer>();
                component_manager->RegisterComponent<Light>();
                component_manager->RegisterComponent<Camera>();

                if (build_scripts_dll) {
                    auto loaded_scripts = CW::AssetManager::Get()->GetLoadedScripts();

                    FILE *file_output = fopen("tools/GeneratedComponentsUtility.cpp", "w");
                    fprintf(file_output, "#define EXPORTING_DLL\n");
                    fprintf(file_output, "#include \"GeneratedComponentsUtility.h\"\n");
                    fprintf(file_output, "#undef EXPORTING_DLL\n\n");

                    for (auto& it : *loaded_scripts) {
                        ScriptData *script_data = it.second;
                        fprintf(file_output, "#include \"Editor/res/projects/Project1/Assets/%s\"\n", script_data->asset_path);
                    }
                    for (auto& it : *loaded_scripts) {
                        ScriptData *script_data = it.second;
                        fprintf(file_output, "const size_t _%s = CW::HashString(\"%s\");\n", script_data->name, script_data->name);
                    }

                    fprintf(file_output, "CW::ComponentManager *_component_manager;\n");
                    fprintf(file_output, "CW::EntityManager *_entity_manager;\n\n");

                    fprintf(file_output, "void InitGeneratedComponentsUtility(CW::ComponentManager *component_manager, CW::EntityManager *entity_manager) {\n");
                    fprintf(file_output, "    _component_manager = component_manager;\n");
                    fprintf(file_output, "    _entity_manager = entity_manager;\n");
                    fprintf(file_output, "}\n");

                    fprintf(file_output, "void RegisterGeneratedComponents() {\n");
                    for (auto& it : *loaded_scripts) {
                        ScriptData *script_data = it.second;
                        fprintf(file_output, "    _component_manager->RegisterComponent<%s>();\n", script_data->name);
                        fprintf(file_output, "    printf(\"Registered Component: %s\\n\");\n", script_data->name);
                    }
                    fprintf(file_output, "}\n");

                    fprintf(file_output, "void UpdateGeneratedComponents() {\n");
                    for (auto& it : *loaded_scripts) {
                        ScriptData *script_data = it.second;
                        fprintf(file_output, "    {\n");
                        fprintf(file_output, "        auto components = _component_manager->GetComponentArray<%s>();\n", script_data->name);
                        fprintf(file_output, "        %s *c = components->GetComponentArrayData();\n", script_data->name);
                        fprintf(file_output, "        int count = _component_manager->GetComponentArray<%s>()->GetComponentArraySize();\n", script_data->name);
                        fprintf(file_output, "        for (int i = 0; i < count; i++) {\n");
                        fprintf(file_output, "            %s_OnUpdate(CW::GameObject { components->GetEntity(i) }, c[i]);\n", script_data->name);
                        fprintf(file_output, "        }\n");
                        fprintf(file_output, "    }\n");
                    }
                    fprintf(file_output, "}\n");

                    fprintf(file_output, "void AddGeneratedComponent(size_t type, CW::GameObject& obj) {\n");
                    for (auto& it : *loaded_scripts) {
                        ScriptData *script_data = it.second;
                        fprintf(file_output, "    if (_%s == type || typeid(%s).hash_code() == type) {\n", script_data->name, script_data->name);
                        fprintf(file_output, "        obj.AddComponent<%s>(_component_manager, _entity_manager);\n", script_data->name);
                        fprintf(file_output, "        return;\n");
                        fprintf(file_output, "    }\n");
                    }
                    fprintf(file_output, "}\n");

                    fprintf(file_output, "bool HasGeneratedComponent(size_t type, CW::GameObject& obj) {\n");
                    for (auto& it : *loaded_scripts) {
                        ScriptData *script_data = it.second;
                        fprintf(file_output, "    if (_%s == type) {\n", script_data->name);
                        fprintf(file_output, "        return obj.HasComponent<%s>(_component_manager, _entity_manager);\n", script_data->name);
                        fprintf(file_output, "    }\n");
                    }
                    fprintf(file_output, "    return false;\n");
                    fprintf(file_output, "}\n");

                    fprintf(file_output, "void RemoveGeneratedComponent(size_t type, CW::GameObject& obj) {\n");
                    for (auto& it : *loaded_scripts) {
                        ScriptData *script_data = it.second;
                        fprintf(file_output, "    if (_%s == type || typeid(%s).hash_code() == type) {\n", script_data->name, script_data->name);
                        fprintf(file_output, "        obj.RemoveComponent<%s>(_component_manager, _entity_manager);\n", script_data->name);
                        fprintf(file_output, "        return;\n");
                        fprintf(file_output, "    }\n");
                    }
                    fprintf(file_output, "}\n");

                    fclose(file_output);

                    FreeDLL();
                    BuildDLL();
                }
                LoadDLLFunctions();
                
                CW::InitGeneratedComponentsUtility();
                CW::RegisterGeneratedComponents();
            } break;
        }
    }
}