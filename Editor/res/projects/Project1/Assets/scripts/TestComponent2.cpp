#define EXPORTING_DLL
#include "TestComponent2.h"

void TestComponent2_OnAwake(CW::GameObject game_object, TestComponent2 comp) {
}
void TestComponent2_OnStart(CW::GameObject game_object, TestComponent2 comp) {
}
void TestComponent2_OnUpdate(CW::GameObject game_object, TestComponent2 comp) {
    printf("hello!\n");
}
void TestComponent2_OnDestroy(CW::GameObject game_object, TestComponent2 comp) {
}
