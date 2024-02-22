#define EXPORTING_DLL
#include "GeneratedComponentsUtility.h"
#undef EXPORTING_DLL

#include "Editor/res/projects/Project1/Assets/scripts/TestComponent.h"
#include "Editor/res/projects/Project1/Assets/scripts/TestComponent2.h"
const size_t _TestComponent = CW::HashString("TestComponent");
const size_t _TestComponent2 = CW::HashString("TestComponent2");
CW::ComponentManager *_component_manager;
CW::EntityManager *_entity_manager;

void InitGeneretedComponentsUtility(CW::ComponentManager *component_manager, CW::EntityManager *entity_manager) {
    _component_manager = component_manager;
    _entity_manager = entity_manager;
}
void RegisterGeneratedComponents() {
    _component_manager->RegisterComponent<TestComponent>();
    printf("Registered Component: TestComponent\n");
    _component_manager->RegisterComponent<TestComponent2>();
    printf("Registered Component: TestComponent2\n");
}
void AddGenereatedComponent(size_t type, CW::GameObject& obj) {
    if (_TestComponent == type) {
        obj.AddComponent<TestComponent>(_component_manager, _entity_manager);
        return;
    }
    if (_TestComponent2 == type) {
        obj.AddComponent<TestComponent2>(_component_manager, _entity_manager);
        return;
    }
}
bool HasGenereatedComponent(size_t type, CW::GameObject& obj) {
    if (_TestComponent == type) {
        return obj.HasComponent<TestComponent>(_component_manager, _entity_manager);
    }
    if (_TestComponent2 == type) {
        return obj.HasComponent<TestComponent2>(_component_manager, _entity_manager);
    }
    return false;
}
