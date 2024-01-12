#pragma once

#include <cstdint>
#include <stdio.h>

#include "Entity.h"
#include "EntityManager.h"
#include "ComponentManager.h"
#include "Components.h"

namespace CW {

	extern EntityManager *entityManager;
	extern ComponentManager *componentManager;

	struct GameObject {
	public:
		static GameObject Instantiate();
		static void Destory(GameObject &gameObject);
		
		bool operator<(const GameObject &rhs) const noexcept {
			return this->entity < rhs.entity;
		}
		template<typename T>
		T& AddComponent() {
			componentManager->AddComponent<T>(entity);

			Signature signature = entityManager->GetSignature(entity);
			signature.set(componentManager->GetComponentType<T>(), true);
			entityManager->SetSignature(entity, signature);

			return componentManager->GetComponent<T>(entity);
		}
		template<typename T>
		T& GetComponent() {
			return componentManager->GetComponent<T>(entity);
		}
		template<typename T>
		void RemoveComponent() {
			componentManager->RemoveComponent<T>(entity);

			Signature signature = entityManager->GetSignature(entity);
			signature.set(componentManager->GetComponentType<T>(), false);
			entityManager->SetSignature(entity, signature);
		}
		template<typename T>
		bool HasComponent() {
			Signature signature = entityManager->GetSignature(entity);

			return signature.test(componentManager->GetComponentType<T>());
		}

		Entity GetEntity() {
			return entity;
		}
	private:
		Entity entity;
	};
}