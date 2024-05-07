#pragma once
#include <bitset>
#include <vector>

const unsigned int MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> Signature;

class Entity {
private:
	int id;
public:
	Entity(int id): id(id){};
	int GetId() const;
};

struct IComponent {
protected:
	static int nextId;
};

// Used to assign a unique id to a component type
template <typename T>
class Component: public IComponent {
	// Returns the unique id of component<T>
	static int GetId(){
		static auto id = nextId++;
		return id;
	}
};

class System {
private:
	Signature componentSignature;
	std::vector<Entity> entities;
public:
	System() = default;
	~System() = default;

	void AddEntityToSystem(Entity entity);
	void RemoveEntity(Entity entity);
	std::vector<Entity> GetSystemEntities() const;
	const Signature& GetComponentSignature() const;

	// Defines the component type that entities must have to be considered by the system
	template <typename TComponent> void RequiredComponent();
};

class Registry {

};

// This is the standard way of writing template methods
// also programmers seems to write first the template thing and the rest of the method in a separate line
template <typename TComponent>
void System::RequiredComponent() {
	const auto componentId = Component<TComponent>::GetId();
	componentSignature.set(componentId);
}