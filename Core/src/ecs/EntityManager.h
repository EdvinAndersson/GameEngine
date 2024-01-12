#pragma once
#include <cassert>
#include <queue>
#include <bitset>

#include "Entity.h"
#include "Component.h"

namespace CW {

	class EntityManager
	{
	public:
		EntityManager();
		Entity CreateEntity();
		void DestroyEntity(Entity entity);
		void SetSignature(Entity entity, Signature signature);
		Signature GetSignature(Entity entity);

	private:
		// Queue of unused entity IDs
		std::queue<Entity> mAvailableEntities{};

		// Array of signatures where the index corresponds to the entity ID
		Signature mSignatures[MAX_ENTITIES];

		// Total living entities - used to keep limits on how many exist
		uint32_t mLivingEntityCount{};
	};
}