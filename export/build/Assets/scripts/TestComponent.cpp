#include "TestComponent.h"

void TestComponent_OnAwake(CW::GameObject game_object, TestComponent& comp) {
}
void TestComponent_OnStart(CW::GameObject game_object, TestComponent& comp) {
}
void TestComponent_OnUpdate(CW::GameObject game_object, TestComponent& comp) {
    CW::Transform& transform = game_object.GetComponent<CW::Transform>(_component_manager);

    transform.rotation.y += 0.002;
}
void TestComponent_OnDestroy(CW::GameObject game_object, TestComponent& comp) {
}
