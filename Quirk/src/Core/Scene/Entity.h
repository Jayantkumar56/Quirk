
#pragma once

#include "entt.hpp"
#include "Core/core.h"
#include "Core/Scene/Scene.h"
#include "Core/Scene/Components.h"

namespace Quirk {

	class Entity {
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene): m_EntityHandle(handle), m_Scene(scene) {}
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			QK_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(m_EntityHandle, component);
			return component;
		}

		template<typename T, typename... Args>
		T& AddOrReplaceComponent(Args&&... args) {
			T& component = m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(m_EntityHandle, component);
			return component;
		}

		template<typename T>
		T& GetComponent() {
			QK_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent() {
			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent() {
			QK_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		inline operator bool()			const { return m_EntityHandle != entt::null; }
		inline operator entt::entity()  const { return m_EntityHandle; }
		inline operator uint32_t()		const { return (uint32_t)m_EntityHandle; }
		inline operator Scene* ()		const { return m_Scene; }

		bool operator==(const Entity& other) const {
			return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
		}

		bool operator!=(const Entity& other) const {
			return !(*this == other);
		}

		inline std::string& GetName() { return GetComponent<TagComponent>().Tag; }
		inline bool IsInvalidEntity() { return m_Scene == NULL || m_EntityHandle == entt::null; }

	private:
		entt::entity m_EntityHandle = entt::null;
		Scene* m_Scene = nullptr;
	};

}
