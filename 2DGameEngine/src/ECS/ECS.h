#pragma once
#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <set>
#include <memory>

const unsigned int MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> Signature;

class Entity {
private:
	int id;
public:
	Entity(int id): id(id){};
	//Entity(const Entity& entity) = default;

	int GetId() const;

	// operator overloading
	// we are evaluating what it means for an entity to be == or != to another entity
	bool operator == (const Entity& other) const {
		return id == other.id;
	}
	bool operator != (const Entity& other) const {
		return id != other.id;
	}
	bool operator < (const Entity& other) const {
		return id < other.id;
	}
	// we can also overload the asignment operator
	Entity& operator = (const Entity& other) = default;
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

/////////////////////////////////////////////////////////////////////////////////////////
// System
/////////////////////////////////////////////////////////////////////////////////////////
// The system processes entities that contain a specific signature.
/////////////////////////////////////////////////////////////////////////////////////////
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


/////////////////////////////////////////////////////////////////////////////////////////
// Pool
/////////////////////////////////////////////////////////////////////////////////////////
// Pool template class that will hold a vector of components of the same type.
/////////////////////////////////////////////////////////////////////////////////////////
class IPool {
public:
	virtual ~IPool() {}
};

template <typename T>
class Pool : public IPool {
private:
	std::vector<T> data;
public:
	Pool(int size = 100) {
		data.resize(size);
	}

	virtual ~Pool() = default;

	bool isEmpty() const {
		return data.empty();
	}
	int GetSize() {
		return data.size();
	}
	void Resize(int size) {
		data.resize(size);
	}
	void Clear() {
		data.clear();
	}
	void Add(T object) {
		data.push_back(object);
	}
	void Set(int index, T object) {
		data[index] = object;
	}
	T& Get(int index) {
		return static_cast<T&>(data[index]);
	}
	T& operator [] (unsigned int index) {
		return data[index];
	}
};

/////////////////////////////////////////////////////////////////////////////////////////
// Registry
/////////////////////////////////////////////////////////////////////////////////////////
// The Registry manages the creation of entities, add systems and components.
/////////////////////////////////////////////////////////////////////////////////////////
class Registry {
private:
	int numEntities = 0;
	std::set<Entity> entitiesToBeAdded;
	std::set<Entity> entitiesTobeKilled;

	std::vector<IPool*> componentPools;

	// Vector of component signatures per entity, saying which component is turned on for a given entity
	// Vector index = entity id
	std::vector<Signature> entityComponentSignatures;
	std::unordered_map<std::type_index, System*> systems;
public:
	Registry() = default;

	// Registry update() finally processes the entities that are waiting to be added/killed
	void Update();

	// Entity management
	Entity CreateEntity();

	//void AddEntityToSystem(Entity entity);

	//void KillEntity(Entity entity);

	// Component management 
	template <typename TComponent, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);
	template <typename TComponent> void RemoveComponent(Entity entity);
	template <typename TComponent> bool HasComponent(Entity entity) const;

	// System management
	template <typename TSystem, typename ...TArgs> void AddSystem(TArgs&& ...args);
	template <typename TSystem> void RemoveSystem();
	template <typename TSystem> bool HasSystem() const;
	template <typename TSystem> TSystem& GetSystem() const;

	// Checks the component signature of an entity and add the entity to the systems that are interested in it
	void AddEntityToSystems(Entity entity); 
	
};



// Component management functions
template <typename TComponent>
void System::RequiredComponent() {
	const auto componentId = Component<TComponent>::GetId();
	componentSignature.set(componentId);
}

template<typename TComponent, typename ...TArgs>
void Registry::AddComponent(Entity entity, TArgs && ...args) {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	// if the component id is greater than the current size of the componentPools, the resize the vector
	if (componentId >= componentPools.size()) {
		componentPools.resize(componentId + 1, nullptr);
	}

	// If we still dont have a pool for that component type, create one
	// if(componentPools[componentId] == nullptr)
	if (!componentPools[componentId]) {
		Pool<TComponent>* newComponentPool = new Pool<TComponent>();
		componentPools[componentId] = newComponentPool;
	}

	// Get the pool of component values for that component type
	Pool<TComponent>* componentPool = Pool<TComponent>(componentPools[componentId]);

	// instead of manually dereferencing the pointer, we can use -> to call methods on the object it points to.
	/*if (entityId >= (*componentPool).GetSize()) {
		(*componentPool).Resize(numEntities);
	}*/

	// if the entity id is greater than the current size of the component pool, then resize the pool
	if (entityId >= componentPool->GetSize()) {
		componentPool->Resize(numEntities);
	}

	// Create a new Component object of type T, and forward the various parameters to the constructor of the component
	TComponent newComponent(std::forward<TArgs>(args));

	// Add the new component to the component pool list, using the entity id as an index
	componentPool->Set(entityId, newComponent);

	// Finally, change the component signature of the entity and set the component id on the bitset to 1
	entityComponentSignatures[entityId].set(componentId);
}

template<typename TComponent>
void Registry::RemoveComponent(Entity entity) {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	entityComponentSignatures[entityId].set(componentId, false);
}

template<typename TComponent>
bool Registry::HasComponent(Entity entity) const {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	return entityComponentSignatures[entityId].test(componentId);
}


// System management functions: black box, dont fully understand how unordered maps work
template<typename TSystem, typename ...TArgs>
void Registry::AddSystem(TArgs && ...args) {
	TSystem* newSystem = new TSystem(std::forward<TArgs>(args)...);
	systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
}

template<typename TSystem>
inline void Registry::RemoveSystem() {
	auto system = systems.find(std::type_index(typeid(TSystem)));
	systems.erase(system);
}

template<typename TSystem>
bool Registry::HasSystem() const {
	return systems.find(std::type_index(typeid(TSystem))) != systems.end();
}

template<typename TSystem>
TSystem& Registry::GetSystem() const {
	auto system = systems.find(std::type_index(typeid(TSystem)));
	return *(std::static_pointer_cast<TSystem>(system->second));
}

