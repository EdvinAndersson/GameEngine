#include "ECS.h"

#include "EntityManager.h"
#include "ComponentManager.h"
#include "Components.h"

//temp
#include "GameObject.h"

namespace CW {

    EntityManager *entity_manager = new EntityManager();
    ComponentManager *component_manager = new ComponentManager();

    ECS::ECS() {
        std::string f = R"(
#pragma once

#include "ecs/GameObject.h"
#include "scene/Scene.h"
#include "ecs/Components.h"
#include "Rendering/Renderer3D.h"

#include "Editor/res/projects/Project1/Assets/scripts/TestComponent.h"

namespace CW {

    inline void UpdateTransforms(std::shared_ptr<ComponentArray<Transform>> components) {
        size_t size = components->GetComponentArraySize();
        auto comp_datas = components->GetComponentArrayData();
        for (int i = 0; i < size; i++) {
        }
    }
    inline void UpdateMeshRenderers(std::shared_ptr<ComponentArray<MeshRenderer>> components) {
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
    inline void UpdateCameras(std::shared_ptr<ComponentArray<Camera>> components) {
        size_t size = components->GetComponentArraySize();
        auto comp_datas = components->GetComponentArrayData();
        for (int i = 0; i < size; i++) {
        }
    }
    inline void UpdateTestComponents(std::shared_ptr<ComponentArray<TestComponent>> components) {
        size_t size = components->GetComponentArraySize();
        auto comp_datas = components->GetComponentArrayData();
        for (int i = 0; i < size; i++) {
            GameObject game_object = GameObject { components->GetEntity(i) };
            TestComponent_OnUpdate(game_object, comp_datas[i]);
        }
    }

    inline void GeneratedUpdateComponenets(Scene &scene) {
        {
            std::shared_ptr<ComponentArray<Transform>> comp = component_manager->GetComponentArray<Transform>();
            UpdateTransforms(comp);
        }
        {
            std::shared_ptr<ComponentArray<MeshRenderer>> comp = component_manager->GetComponentArray<MeshRenderer>();
            UpdateMeshRenderers(comp);
        }
        {
            std::shared_ptr<ComponentArray<Camera>> comp = component_manager->GetComponentArray<Camera>();
            UpdateCameras(comp);
        }
        {
            std::shared_ptr<ComponentArray<TestComponent>> comp = component_manager->GetComponentArray<TestComponent>();
            UpdateTestComponents(comp);
        }
    }
}
        )";

        FILE *file_output = fopen("Core/src/ecs/GeneratedComponents.h", "w");
        fprintf(file_output, f.c_str());
        #if 0
        fprintf(file_output, "#pragma once\n");
        fprintf(file_output, "#include \"GameObject.h\"\n");
        fprintf(file_output, "namespace CW {\n");
        fprintf(file_output, "  void AddC(GameObject obj) {\n");
        fprintf(file_output, "      obj.AddComponent<%s>();\n", "TestComponent");
        fprintf(file_output, "  }\n");
        fprintf(file_output, "}\n");
        fclose(file_output);
        #endif

        //GeneratedRegisterComponents(component_manager);
        EventListen(PROJECT_LOAD);

#if 0
        ComponentType t = component_manager->GetComponentType<MeshRenderer>();
        printf("%i\n", t);

        auto a = component_manager->GetComponentValue(t);
        printf("%s\n", a);
        const char *name_begin = strchr(a, ':') + 2;
        printf("%s\n", name_begin);

        //GameObject obj = GameObject::Instantiate();
        
        FILE *file_output = fopen("Core/src/ecs/AddComponents.h", "w");
        fprintf(file_output, "#pragma once\n");
        fprintf(file_output, "#include \"GameObject.h\"\n");
        fprintf(file_output, "namespace CW {\n");
        fprintf(file_output, "  void AddC(GameObject obj) {\n");
        fprintf(file_output, "      obj.AddComponent<%s>();\n", name_begin);
        fprintf(file_output, "  }\n");
        fprintf(file_output, "}\n");
        fclose(file_output);
#endif
        //
        //AddC(obj);

        //printf("%i\n", obj.GetSignature());
    }
    void ECS::UpdateComponenets(Scene &scene) {
        //GeneratedUpdateComponenets(scene);
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
                component_manager->RegisterComponent<Script>();
                component_manager->RegisterComponent<Light>();
                component_manager->RegisterComponent<Camera>();

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

                fprintf(file_output, "void InitGeneretedComponentsUtility(CW::ComponentManager *component_manager, CW::EntityManager *entity_manager) {\n");
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

                fprintf(file_output, "void AddGenereatedComponent(size_t type, CW::GameObject& obj) {\n");
                for (auto& it : *loaded_scripts) {
                    ScriptData *script_data = it.second;
                    fprintf(file_output, "    if (_%s == type) {\n", script_data->name);
                    fprintf(file_output, "        obj.AddComponent<%s>(_component_manager, _entity_manager);\n", script_data->name);
                    fprintf(file_output, "        return;\n");
                    fprintf(file_output, "    }\n");
                }
                fprintf(file_output, "}\n");

                fprintf(file_output, "bool HasGenereatedComponent(size_t type, CW::GameObject& obj) {\n");
                for (auto& it : *loaded_scripts) {
                    ScriptData *script_data = it.second;
                    fprintf(file_output, "    if (_%s == type) {\n", script_data->name);
                    fprintf(file_output, "        return obj.HasComponent<%s>(_component_manager, _entity_manager);\n", script_data->name);
                    fprintf(file_output, "    }\n");
                }
                fprintf(file_output, "    return false;\n");
                fprintf(file_output, "}\n");

                fclose(file_output);

                FreeDLL();
                BuildDLL();

                LoadDLLFunctions("ScriptsDLL.dll");
                
                CW::InitGeneretedComponentsUtility();
                CW::RegisterGeneratedComponents();
            } break;
        }
    }
}