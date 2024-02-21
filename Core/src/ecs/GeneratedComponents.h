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
    
    inline void GeneratedRegisterComponents(ComponentManager *component_manager) {
        component_manager->RegisterComponent<TestComponent>();
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