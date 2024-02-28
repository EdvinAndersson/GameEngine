#include "TestComponent.h"

void TestComponent_OnAwake(CW::GameObject game_object, TestComponent& comp) {
}
void TestComponent_OnStart(CW::GameObject game_object, TestComponent& comp) {
}
void TestComponent_OnUpdate(CW::GameObject game_object, TestComponent& comp) {
    printf("TestComponent_OnUpdate asdasdsadsasdsdds!\n");

    //CWEditor::Console::Log(CWEditor::LOG_WARNING, "HELLO!!!");
}
void TestComponent_OnDestroy(CW::GameObject game_object, TestComponent& comp) {
}
