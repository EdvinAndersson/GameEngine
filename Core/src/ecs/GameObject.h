#pragma once

#include <cstdint>
#include <stdio.h>

#include "Entity.h"
#include "EntityManager.h"
#include "ComponentManager.h"
#include "Components.h"
#include "cglm/struct.h"

namespace CW {

	extern EntityManager *entity_manager;
	extern ComponentManager *component_manager;

	struct GameObject {
	public:
		static GameObject Instantiate();
		static GameObject Instantiate(vec3s position);
		static void Destory(GameObject &gameObject);
		
		bool operator<(const GameObject &rhs) const noexcept {
			return this->entity < rhs.entity;
		}
		template<typename T>
		T& AddComponent(ComponentManager *component_manager, EntityManager *entity_manager) {
			component_manager->AddComponent<T>(entity);

			Signature signature = entity_manager->GetSignature(entity);
			signature.set(component_manager->GetComponentType<T>(), true);
			entity_manager->SetSignature(entity, signature);

			return component_manager->GetComponent<T>(entity);
		}
		template<typename T>
		T& AddComponent() {
			return AddComponent<T>(component_manager, entity_manager);
		}
		template<typename T>
		T& GetComponent(ComponentManager *component_manager) {
			return component_manager->GetComponent<T>(entity);
		}
		template<typename T>
		T& GetComponent() {
			return GetComponent<T>(component_manager);
		}
		template<typename T>
		void RemoveComponent() {
			component_manager->RemoveComponent<T>(entity);

			Signature signature = entity_manager->GetSignature(entity);
			signature.set(component_manager->GetComponentType<T>(), false);
			entity_manager->SetSignature(entity, signature);
		}
		template<typename T>
		bool HasComponent(ComponentManager *component_manager, EntityManager *entity_manager) {
			Signature signature = entity_manager->GetSignature(entity);

			return signature.test(component_manager->GetComponentType<T>());
		}
		template<typename T>
		bool HasComponent() {
			return HasComponent<T>(component_manager, entity_manager);
		}
		Signature GetSignature() {
			return entity_manager->GetSignature(entity);
		}
		
		Entity entity = 0;
	};
}