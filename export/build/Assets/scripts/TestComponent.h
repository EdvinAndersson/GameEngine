#include "ecs/GameObject.h"
#include "ecs/Components.h"

struct TestComponent {
};

inline void TestComponent_OnAwake(CW::GameObject game_object, TestComponent comp) {
}
inline void TestComponent_OnStart(CW::GameObject game_object, TestComponent comp) {
}
inline void TestComponent_OnUpdate(CW::GameObject game_object, TestComponent comp) {
    printf("hello!\n");
}
inline void TestComponent_OnDestroy(CW::GameObject game_object, TestComponent comp) {
}