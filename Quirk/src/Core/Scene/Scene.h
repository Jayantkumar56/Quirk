

#pragma once

#include "Core/core.h"
#include "Core/Scene/Components.h"
#include "Core/AssetManager/EditorAssetManager.h"
#include "Core/Config.h"

#include "entt.hpp"

namespace Quirk {

	class Entity;

    // NOTE:
    // 
    // - Init must be called after construction Scene object

	class Scene {
		friend class Entity;
		friend class SceneSerializer;

	public:
		Scene(std::string name, uint16_t width, uint16_t height) noexcept : 
				m_Name           ( std::move(name) ),
				m_ViewportWidth  ( width           ),
				m_ViewportHeight ( height          ) 
		{}

        inline void Init(AssetManager* assetManager) noexcept {
            m_AssetManager = assetManager;
        }

		static Ref<Scene> Copy(const Scene* other);
        static inline Ref<Scene> Copy(const Ref<Scene>& other) { return Copy(other.get()); }

		Entity CreateEntity(const std::string& name = std::string(), const uint64_t uuid = UUID());
		void DestroyEntity(Entity entity);

		inline void DestroyAllEntities() { m_Registry.clear(); }

		void OnUpdate();

		void RenderSceneEditor(const glm::mat4& projectionViewMat, glm::vec3 cameraPos);
		void RenderSceneRuntime();

		void OnViewportResize(uint32_t width, uint32_t height);

		Entity FindEntityByName(std::string_view name);
		Entity GetPrimaryCameraEntity();

		std::string& GetName()           { return m_Name;     }
		const auto&  GetRegistry() const { return m_Registry; }

		template<typename... Components>
		auto GetAllEntitiesWith() { return m_Registry.view<Components...>(); }

	private:
		void RenderScene(const glm::mat4& projectionViewMat, glm::vec3 cameraPos);

		template<typename T>
		void OnComponentAdded(entt::entity entity, T& component) { }

		template<>
		void OnComponentAdded<CameraComponent>(entt::entity entity, CameraComponent& component) {
			if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
				component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
		}

	private:
		std::string    m_Name;
		entt::registry m_Registry;
		uint16_t       m_ViewportWidth;
		uint16_t       m_ViewportHeight;
        
        AssetManager*  m_AssetManager = nullptr;
	};

}
