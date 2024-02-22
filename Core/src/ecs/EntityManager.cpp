#include "EntityManager.h"

namespace CW {
	EntityManager::EntityManager()
	{
		ResetEntities();
	}

	Entity EntityManager::CreateEntity()
	{
		CW_ASSERT(mLivingEntityCount < MAX_ENTITIES, "Too many entities in existence.");

		// Take an ID from the front of the queue
		Entity entity = mAvailableEntities.front();
		mAvailableEntities.pop();
		++mLivingEntityCount;

		return entity;
	}

	void EntityManager::DestroyEntity(Entity entity)
	{
		CW_ASSERT(entity < MAX_ENTITIES, "Entity out of range.");

		// Invalidate the destroyed entity's signature
		mSignatures[entity].reset();

		// Put the destroyed ID at the back of the queue
		mAvailableEntities.push(entity);
		--mLivingEntityCount;
	}

	void EntityManager::SetSignature(Entity entity, Signature signature)
	{
		CW_ASSERT(entity < MAX_ENTITIES, "Entity out of range.");

		// Put this entity's signature into the array
		mSignatures[entity] = signature;
	}

	Signature EntityManager::GetSignature(Entity entity)
	{
		CW_ASSERT(entity < MAX_ENTITIES, "Entity out of range.");

		// Get this entity's signature from the array
		return mSignatures[entity];
	}

	void EntityManager::ResetEntities() {
		while(!mAvailableEntities.empty()) mAvailableEntities.pop();

		// Initialize the queue with all possible entity IDs
		for (uint32_t id = 0; id < MAX_ENTITIES; ++id)
		{
			mAvailableEntities.push(Entity(id + 1));
		}
		mSignatures->reset();
		mLivingEntityCount = 0;
	}
}