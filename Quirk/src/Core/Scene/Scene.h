

#pragma once

#include "Core/core.h"
#include "Core/Scene/Components.h"

#include "entt.hpp"

namespace Quirk {

	class Entity;

	class Scene {
		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;

	public:
		Scene(uint16_t width, uint16_t height);
		~Scene();

		Entity CreateEntity(const std::string& name = std::string(), const uint64_t uuid = UUID());
		void DestroyEntity(Entity entity);

		void OnUpdate();
		void RenderScene(const glm::mat4& projectionView);
		void OnViewportResize(uint32_t width, uint32_t height);

		Entity FindEntityByName(std::string_view name);
		Entity GetPrimaryCameraEntity();

		template<typename... Components>
		auto GetAllEntitiesWith() {
			return m_Registry.view<Components...>();
		}

	private:
		template<typename T>
		void OnComponentAdded(entt::entity entity, T& component) {
		}

		template<>
		void OnComponentAdded<CameraComponent>(entt::entity entity, CameraComponent& component) {
			if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
				component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
		}

	private:
		entt::registry m_Registry;
		uint16_t m_ViewportWidth, m_ViewportHeight;
	};

}
