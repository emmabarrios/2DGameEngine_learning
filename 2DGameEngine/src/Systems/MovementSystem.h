#pragma once
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"

class MovementSystem : public System {
public:
	MovementSystem() {
		RequiredComponent<TransformComponent>();
		RequiredComponent<RigidBodyComponent>();
	}

	void Update() {
		for (auto entity: GetSystemEntities()) {

			// is a reference because we want to change the value of that component so we need its reference
			//TransformComponent& transform = entity.GetComponent<TransformComponent>();

			// is const because we just need to read the value
			//const RigidBodyComponent rigidBody = entity.GetComponent<RigidBodyComponent>();

			// let the compiler figure out the type instead
			auto& transform = entity.GetComponent<TransformComponent>();
			const auto rigidBody = entity.GetComponent<RigidBodyComponent>();

			transform.position.x += rigidBody.velocity.x;
			transform.position.y += rigidBody.velocity.y;
		}
	}
};
