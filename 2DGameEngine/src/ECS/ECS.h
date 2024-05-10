#pragma once
#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <set>

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
	Registry()= default;
	void Update();
	Entity CreateEntity();
	void AddEntityToSystem(Entity entity);
	void KillEntity(Entity entity);
	template <typename T, typename  TArgs> void AddComponent(Entity entity, TArgs args);
	
};



/////////////////////////////////////////////////////////////////////////////////////////
// Pool
/////////////////////////////////////////////////////////////////////////////////////////
// Pool template class that will hold a vector of components of the same type.
/////////////////////////////////////////////////////////////////////////////////////////
class IPool {
public:
	virtual ~IPool() {};
};

template <typename T>
class Pool: public IPool{
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
		data[index] = objectl;
	}
	T& Get(int index) {
		return static_cast<T&>(data[index]);
	}
	T& operator [] (unsigned int index) {
		return data[index];
	}
};

// This is the standard way of writing template methods
// also programmers seems to write first the template thing and the rest of the method in a separate line
template <typename TComponent>
void System::RequiredComponent() {
	const auto componentId = Component<TComponent>::GetId();
	componentSignature.set(componentId);
}

template<typename T, typename Targs>
void Registry::AddComponent(Entity entity, Targs args) {
	const auto componentId = Component<T>::GetId();
	const auto entityId = entity.GetId();

	if (componentId >= componentPools.size()) {
		componentPools.resize(componentId + 1, nullptr);
	}

	if (!componentPools[componentId]) {
		Pool<T>* newComponentPool = new Pool<T>();
		componentPools[componentId] = newComponentPool;
	}

	Pool<T>* componentPool = Pool<T>(componentPools[componentId]);

	// instead of manually dereferencing the pointer, we can use -> to call methods on the object it points to.
	/*if (entityId >= (*componentPool).GetSize()) {
		(*componentPool).Resize(numEntities);
	}*/

	if (entityId >= componentPool->GetSize()) {
		componentPool->Resize(numEntities);
	}
}