#pragma once
#include <unordered_map>
#include <memory>

#include "Entity.h"
#include "Component.h"
#include "ComponentArray.h"
#include "CWAssert.h"

namespace CW {

	class ComponentManager
	{
	public:
		template<typename T>
		void RegisterComponent()
		{
			size_t typeHash = typeid(T).hash_code();
			CW_ASSERT(mComponentTypes.find(typeHash) == mComponentTypes.end(), "Registering component type more than once.");

			// Add this component type to the component type map
			mComponentTypes.insert({ typeHash, mNextComponentType });

			// Create a ComponentArray pointer and add it to the component arrays map
			mComponentArrays.insert({ typeHash, std::make_shared<ComponentArray<T>>() });

			// Increment the value so that the next component registered will be different
			++mNextComponentType;
		}

		template<typename T>
		ComponentType GetComponentType()
		{
			size_t typeHash = typeid(T).hash_code();

			CW_ASSERT(mComponentTypes.find(typeHash) != mComponentTypes.end(), "Component not registered before use.");

			// Return this component's type - used for creating signatures
			return mComponentTypes[typeHash];
		}

		template<typename T>
		void AddComponent(Entity &entity)
		{
			// Add a component to the array for an entity
			GetComponentArray<T>()->InsertData(entity, T());
		}

		template<typename T>
		void RemoveComponent(Entity &entity)
		{
			// Remove a component from the array for an entity
			GetComponentArray<T>()->RemoveData(entity);
		}

		template<typename T>
		T &GetComponent(Entity &entity)
		{
			// Get a reference to a component from the array for an entity
			return GetComponentArray<T>()->GetData(entity);
		}

		void EntityDestroyed(Entity &entity)
		{
			// Notify each component array that an entity has been destroyed
			// If it has a component for that entity, it will remove it
			for (auto const &pair : mComponentArrays)
			{
				auto const &component = pair.second;

				component->EntityDestroyed(entity);
			}
		}
		// Convenience function to get the statically casted pointer to the ComponentArray of type T.
		template<typename T>
		std::shared_ptr<ComponentArray<T>> GetComponentArray()
		{
			size_t typeHash = typeid(T).hash_code();

			CW_ASSERT(mComponentTypes.find(typeHash) != mComponentTypes.end(), "Component not registered before use.");

			return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeHash]);
		}
		void ResetComponentArrays() {

			for (auto const &pair : mComponentArrays)
			{
				auto const &component = pair.second;

				component->Reset();
			}
			mComponentTypes.clear();
			mComponentArrays.clear();
		}
		const size_t GetComponentValue(ComponentType type) {
			for (auto it = mComponentTypes.begin(); it != mComponentTypes.end(); ++it) {
				if (it->second == type) return it->first;
			}
			return 0;
		}
	private:
		// Map from type string pointer to a component type
		std::unordered_map<size_t, ComponentType> mComponentTypes{};

		// Map from type string pointer to a component array
		std::unordered_map<size_t, std::shared_ptr<IComponentArray>> mComponentArrays{};

		// The component type to be assigned to the next registered component - starting at 0
		ComponentType mNextComponentType{};
	};
}