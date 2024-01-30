#pragma once
#include "CWAssert.h"

namespace CW {
	// The one instance of virtual inheritance in the entire implementation.
	// An interface is needed so that the ComponentManager (seen later)
	// can tell a generic ComponentArray that an entity has been destroyed
	// and that it needs to update its array mappings.
	class IComponentArray
	{
	public:
		virtual ~IComponentArray() = default;
		virtual void EntityDestroyed(Entity entity) = 0;
		virtual void Reset() = 0;
	};


	template<typename T>
	class ComponentArray : public IComponentArray
	{
	public:
		void InsertData(Entity entity, T component) {
			CW_ASSERT(mEntityToIndexMap.find(entity) == mEntityToIndexMap.end(), "Component added to same entity more than once.");

			// Put new entry at end and update the maps
			size_t newIndex = mSize;
			mEntityToIndexMap[entity] = newIndex;
			mIndexToEntityMap[newIndex] = entity;
			mComponentArray[newIndex] = component;
			++mSize;
		}

		void RemoveData(Entity entity) {
			CW_ASSERT(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end(), "Removing non-existent component.");

			// Copy element at end into deleted element's place to maintain density
			size_t indexOfRemovedEntity = mEntityToIndexMap[entity];
			size_t indexOfLastElement = mSize - 1;
			mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement];

			// Update map to point to moved spot
			Entity entityOfLastElement = mIndexToEntityMap[indexOfLastElement];
			mEntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
			mIndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

			mEntityToIndexMap.erase(entity);
			mIndexToEntityMap.erase(indexOfLastElement);

			--mSize;
		}

		T &GetData(Entity entity) {
			CW_ASSERT(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end(), "Retrieving non-existent component.");

			// Return a reference to the entity's component
			return mComponentArray[mEntityToIndexMap[entity]];
		}

		void EntityDestroyed(Entity entity) override {
			if (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end())
			{
				// Remove the entity's component if it existed
				RemoveData(entity);
			}
		}

		void Reset() override {
			memset(mComponentArray, 0, sizeof(T) * MAX_ENTITIES);
			mEntityToIndexMap.clear();
			mIndexToEntityMap.clear();
			mSize = 0;
		}

	private:
		// The packed array of components (of generic type T),
		// set to a specified maximum amount, matching the maximum number
		// of entities allowed to exist simultaneously, so that each entity
		// has a unique spot.
		T mComponentArray[MAX_ENTITIES];

		// Map from an entity ID to an array index.
		std::unordered_map<Entity, size_t> mEntityToIndexMap;

		// Map from an array index to an entity ID.
		std::unordered_map<size_t, Entity> mIndexToEntityMap;

		// Total size of valid entries in the array.
		size_t mSize;
	};
}