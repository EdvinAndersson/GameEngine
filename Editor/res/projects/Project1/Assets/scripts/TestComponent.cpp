#define EXPORTING_DLL
#include "TestComponent.h"

void TestComponent_OnAwake(CW::GameObject game_object, TestComponent& comp) {
}
void TestComponent_OnStart(CW::GameObject game_object, TestComponent& comp) {
}
void TestComponent_OnUpdate(CW::GameObject game_object, TestComponent& comp) {
    printf("TestComponent_OnUpdate!\n");
}
void TestComponent_OnDestroy(CW::GameObject game_object, TestComponent& comp) {
}
