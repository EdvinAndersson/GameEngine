#include "ecs/GameObject.h"
#include "ecs/Components.h"

struct ComponentTemplate {
};

inline void OnAwake(CW::GameObject obj, ComponentTemplate comp) {
}
inline void OnStart(CW::GameObject obj, ComponentTemplate comp) {
}
inline void OnUpdate(CW::GameObject obj, ComponentTemplate comp) {
}
inline void OnDestroy(CW::GameObject obj, ComponentTemplate comp) {
}