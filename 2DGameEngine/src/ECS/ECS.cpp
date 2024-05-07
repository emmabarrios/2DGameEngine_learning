#include "ECS.h"

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