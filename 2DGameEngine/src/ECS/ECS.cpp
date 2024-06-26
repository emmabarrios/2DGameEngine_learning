#include "ECS.h"
#include "../Logger/Logger.h"

int IComponent::nextId = 1;

int Entity::GetId() const {
	return id;
}

void System::AddEntityToSystem(Entity entity){
	entities.push_back(entity);
}

void  System::RemoveEntity(Entity entity){
	entities.erase(std::remove_if(entities.begin(), entities.end(), 
		[&entity](Entity other){
			//return entity.GetId() == other.GetId();
			// after the implementation of operator overloading we can compare elements
			// behind the scenes we are comparing ids
			return entity == other;
		}
	),entities.end());
}

std::vector<Entity>  System::GetSystemEntities() const{
	return entities;
}

const Signature& System::GetComponentSignature() const{
	return componentSignature;
}

Entity Registry::CreateEntity() {
	int entityId = numEntities++;
	Entity entity(entityId);
	entity.registry = this; // this is referring to the registry class, like in C#
	entitiesToBeAdded.insert(entity);

	if (entityId >= entityComponentSignatures.size()){
		entityComponentSignatures.resize(entityId + 1);
	}

	Logger::Log("Entity created with id = " + std::to_string(entityId));

	return entity;
}

void Registry::Update() {
	for (auto entity: entitiesToBeAdded){
		AddEntityToSystems(entity);
	}
	entitiesToBeAdded.clear();

}


void Registry::AddEntityToSystems(Entity entity) {
	const auto entityId = entity.GetId();
	const auto& entityComponentSignature = entityComponentSignatures[entityId];

	for (auto& system: systems) {
		const auto& systemComponentSignature = system.second->GetComponentSignature();

		// bitwise operation
		bool isInterested = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;
		
		if (isInterested) {
			system.second->AddEntityToSystem(entity);
		}
	}
}