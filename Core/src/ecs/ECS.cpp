#include "ECS.h"

#include "EntityManager.h"
#include "ComponentManager.h"
#include "Components.h"

//temp
#include "GameObject.h"
#include "AddComponents.h"

namespace CW {

    EntityManager *entity_manager = new EntityManager();
    ComponentManager *component_manager = new ComponentManager();

    ECS::ECS() {
        component_manager->RegisterComponent<Transform>();
        component_manager->RegisterComponent<MeshRenderer>();

        EventListen(PROJECT_LOAD);

#if 0
        ComponentType t = component_manager->GetComponentType<MeshRenderer>();
        printf("%i\n", t);

        auto a = component_manager->GetComponentValue(t);
        printf("%s\n", a);
        const char *name_begin = strchr(a, ':') + 2;
        printf("%s\n", name_begin);

        //GameObject obj = GameObject::Instantiate();
        
        FILE *file_output = fopen("Core/src/ecs/AddComponents.h", "w");
        fprintf(file_output, "#pragma once\n");
        fprintf(file_output, "#include \"GameObject.h\"\n");
        fprintf(file_output, "namespace CW {\n");
        fprintf(file_output, "  void AddC(GameObject obj) {\n");
        fprintf(file_output, "      obj.AddComponent<%s>();\n", name_begin);
        fprintf(file_output, "  }\n");
        fprintf(file_output, "}\n");
        fclose(file_output);
#endif
        //
        //AddC(obj);

        //printf("%i\n", obj.GetSignature());
    }
    void ECS::OnEvent(Event event) {
        switch (event.event_type)
        {
            case EventType::PROJECT_LOAD:
            {
                component_manager->ResetComponentArrays();
                entity_manager->ResetEntities();
            } break;
        }
    }
}