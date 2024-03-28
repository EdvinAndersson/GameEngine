#include "PlayerMovement.h"

void PlayerMovement_OnAwake(CW::GameObject game_object, PlayerMovement& comp) {
}
void PlayerMovement_OnStart(CW::GameObject game_object, PlayerMovement& comp) {
}
void PlayerMovement_OnUpdate(CW::GameObject game_object, PlayerMovement& comp) {
    CW::Transform& transform = game_object.GetComponent<CW::Transform>(_component_manager);

    transform.rotation.x += 0.002;
}
void PlayerMovement_OnDestroy(CW::GameObject game_object, PlayerMovement& comp) {
}
