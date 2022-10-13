#ifndef PHYSICSSIM_ECS_H
#define PHYSICSSIM_ECS_H

#include <bitset>
#include <queue>
#include <array>
#include <cassert>
#include <unordered_map>
#include <typeinfo>
#include <memory>
#include <set>

#define assertm(exp, msg) assert(((void)msg, exp))

using Entity = std::uint32_t;

constexpr Entity MAX_ENTITIES = 5000;

using ComponentType = std::uint8_t;

constexpr ComponentType MAX_COMPONENTS = 32;

using Signature = std::bitset<MAX_COMPONENTS>;

class EntityManager {
private:
    std::queue<Entity> availableEntities {};
    std::array<Signature, MAX_ENTITIES> signatures {};
    std::uint32_t livingEntityCount {};

public:
    EntityManager() {
        for (Entity entity = 0; entity < MAX_ENTITIES; ++entity) {
            availableEntities.push(entity);
        }
    }

    Entity CreateEntity() {
        assertm(livingEntityCount < MAX_ENTITIES, "Too many entities in existence.");

        Entity id = availableEntities.front();
        availableEntities.pop();
        ++livingEntityCount;

        return id;
    }

    void DestroyEntity(Entity entity) {
        assertm(entity < MAX_ENTITIES, "Entity out of range.");

        signatures[entity].reset();

        availableEntities.push(entity);
        --livingEntityCount;
    }

    void SetSignature(Entity entity, Signature signature) {
        assertm(entity < MAX_ENTITIES, "Entity out of range.");

        signatures[entity] = signature;
    }

    Signature GetSignature(Entity entity) {
        assertm(entity < MAX_ENTITIES, "Entity out of range.");

        return signatures[entity];
    }
};

class IComponentArray {
public:
    virtual ~IComponentArray() = default;
    virtual void EntityDestroyed(Entity entity) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray {
private:
    std::array<T, MAX_ENTITIES> componentArray;
    std::unordered_map<Entity, std::size_t> entityToIndexMap;
    std::unordered_map<std::size_t, Entity> indexToEntityMap;
    std::size_t size;

public:
    void InsertData(Entity entity, T component) {
        assertm(entityToIndexMap.find(entity) == entityToIndexMap.end(), "Component added to same entity more than once.");

        std::size_t newIndex = size;
        entityToIndexMap[entity] = newIndex;
        indexToEntityMap[newIndex] = entity;
        componentArray[newIndex] = component;
        ++size;
    }

    void RemoveData(Entity entity) {
        assertm(entityToIndexMap.find(entity) != entityToIndexMap.end(), "Removing non-existent component.");

        std::size_t indexOfRemoved = entityToIndexMap[entity];
        std::size_t indexOfLast = size - 1;
        componentArray[indexOfRemoved] = componentArray[indexOfLast];

        Entity entityOfLast = indexToEntityMap[indexOfLast];
        entityToIndexMap[entityOfLast] = indexOfRemoved;
        indexToEntityMap[indexOfRemoved] = entityOfLast;

        entityToIndexMap.erase(entity);
        indexToEntityMap.erase(indexOfLast);

        --size;
    }

    T& GetData(Entity entity) {
        assertm(entityToIndexMap.find(entity) != entityToIndexMap.end(), "Retrieving non-existent component.");

        return componentArray[entityToIndexMap[entity]];
    }

    void EntityDestroyed(Entity entity) override {
        if (entityToIndexMap.find(entity) != entityToIndexMap.end()) {
            RemoveData(entity);
        }
    }
};


class ComponentManager {
private:
    std::unordered_map<const char*, ComponentType> componentTypes {};
    std::unordered_map<const char*, std::shared_ptr<IComponentArray>> componentArrays {};
    ComponentType nextComponentType {};
    template<typename T>
    std::shared_ptr<ComponentArray<T>> GetComponentArray() {
        const char* typeName = typeid(T).name();

        assertm(componentTypes.find(typeName) != componentTypes.end(), "Component not registered before use.");

        return std::static_pointer_cast<ComponentArray<T>>(componentArrays[typeName]);
    }

public:
    template<typename T>
    void RegisterComponent() {
        const char* typeName = typeid(T).name();

        assertm(componentTypes.find(typeName) == componentTypes.end(), "Registering component type more than once.");

        componentTypes.insert({typeName, nextComponentType});

        componentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});

        ++nextComponentType;
    }

    template<typename T>
    ComponentType GetComponentType() {
        const char* typeName = typeid(T).name();

        assertm(componentTypes.find(typeName) != componentTypes.end(), "Component not registered before use.");

        return componentTypes[typeName];
    }

    template<typename T>
    void AddComponent(Entity entity, T component) {
        GetComponentArray<T>()->InsertData(entity, component);
    }

    template<typename T>
    void RemoveComponent(Entity entity) {
        GetComponentArray<T>()->RemoveData(entity);
    }

    template<typename T>
    T& GetComponent(Entity entity) {
        return GetComponentArray<T>()->GetData(entity);
    }

    void EntityDestroyed(Entity entity) {
        for(auto const& pair : componentArrays) {
            auto const& component = pair.second;

            component->EntityDestroyed(entity);
        }
    }
};

class System {
public:
    std::set<Entity> entities;
};

class SystemManager {
private:
    std::unordered_map<const char*, Signature> signatures {};
    std::unordered_map<const char*, std::shared_ptr<System>> systems {};

public:
    template<typename T>
    std::shared_ptr<T> RegisterSystem() {
        const char* typeName = typeid(T).name();

        assertm(systems.find(typeName) == systems.end(), "System registered more than once.");

        auto system = std::make_shared<T>();
        systems.insert({typeName, system});
        return system;
    }

    template<typename T>
    void SetSignature(Signature signature) {
        const char* typeName = typeid(T).name();

        assertm(systems.find(typeName) != systems.end(), "System has not been registered");

        signatures.insert({typeName, signature});
    }

    void EntityDestroyed(Entity entity) {
        for(auto const& pair : systems) {
            auto const& system = pair.second;

            system->entities.erase(entity);
        }
    }

    void EntitySignatureChanged(Entity entity, Signature entitySignature) {
        for(auto const& pair : systems) {
            auto const& type = pair.first;
            auto const& system = pair.second;
            auto const& sysSignature = signatures[type];

            if((entitySignature & sysSignature) == sysSignature) {
                system->entities.insert(entity);
            } else {
                system->entities.erase(entity);
            }
        }
    }
};

class Coordinator {
private:
    std::unique_ptr<ComponentManager> componentManager;
    std::unique_ptr<EntityManager> entityManager;
    std::unique_ptr<SystemManager> systemManager;

public:
    void Init() {
        componentManager = std::make_unique<ComponentManager>();
        entityManager = std::make_unique<EntityManager>();
        systemManager = std::make_unique<SystemManager>();
    }

    Entity CreateEntity() {
        return entityManager->CreateEntity();
    }

    void DestroyEntity(Entity entity) {
        entityManager->DestroyEntity(entity);
    }

    template<typename T>
    void RegisterComponent() {
        componentManager->RegisterComponent<T>();
    }

    template<typename T>
    void AddComponent(Entity entity, T component) {
        componentManager->AddComponent<T>(entity, component);

        auto sig = entityManager->GetSignature(entity);
        sig.set(componentManager->GetComponentType<T>(), true);
        entityManager->SetSignature(entity, sig);

        systemManager->EntitySignatureChanged(entity, sig);
    }

    template<typename T>
    void RemoveComponent(Entity entity, T component) {
        componentManager->RemoveComponent<T>(entity, component);

        auto sig = entityManager->GetSignature(entity);
        sig.set(componentManager->GetComponentType<T>(), true);
        entityManager->SetSignature(entity, sig);

        systemManager->EntitySignatureChanged(entity, sig);
    }

    template<typename T>
    T& GetComponent(Entity entity) {
        return componentManager->GetComponent<T>(entity);
    }

    template<typename T>
    ComponentType GetComponentType() {
        return componentManager->GetComponentType<T>();
    }

    template<typename T>
    std::shared_ptr<T> RegisterSystem() {
        return systemManager->RegisterSystem<T>();
    }

    template<typename T>
    void SetSystemSignature(Signature signature) {
        systemManager->SetSignature<T>(signature);
    }
};

#endif //PHYSICSSIM_ECS_H
