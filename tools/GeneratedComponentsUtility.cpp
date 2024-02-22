#define EXPORTING_DLL
#include "GeneratedComponentsUtility.h"
#undef EXPORTING_DLL

#include "Editor/res/projects/Project1/Assets/scripts/TestComponent.h"
const size_t _TestComponent = CW::HashString("TestComponent");
CW::ComponentManager *_component_manager;
CW::EntityManager *_entity_manager;

void InitGeneretedComponentsUtility(CW::ComponentManager *component_manager, CW::EntityManager *entity_manager) {
    _component_manager = component_manager;
    _entity_manager = entity_manager;
}
void RegisterGeneratedComponents() {
    _component_manager->RegisterComponent<TestComponent>();
    printf("Registered Component: TestComponent\n");
}
void AddGenereatedComponent(size_t type, CW::GameObject& obj) {
    if (_TestComponent == type) {
        obj.AddComponent<TestComponent>(_component_manager, _entity_manager);
        return;
    }
}
bool HasGenereatedComponent(size_t type, CW::GameObject& obj) {
    if (_TestComponent == type) {
        return obj.HasComponent<TestComponent>(_component_manager, _entity_manager);
    }
    return false;
}
