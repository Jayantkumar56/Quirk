

#include "Qkpch.h"
#include "Core/Scene/Scene.h"
#include "Core/Scene/Entity.h"
#include "Core/Renderer/Renderer2D.h"
#include "Core/Scene/ScriptableEntity.h"

namespace Quirk {

	Scene::Scene(uint16_t width, uint16_t height): m_ViewportWidth(width), m_ViewportHeight(height) {
	}

	Scene::~Scene() {
	}

	Entity Scene::CreateEntity(const std::string& name, const uint64_t uuid) {
		Entity entity = { m_Registry.create(), this };

		entity.AddComponent<UUIDComponent>(uuid);
		entity.AddComponent<TransformComponent>();

		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		return entity;
	}

	void Scene::DestroyEntity(Entity entity) {
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdate() {
		m_Registry.view<NativeScriptComponent>().each([this](auto entity, auto& scriptComponent) {
			if (!scriptComponent.Instance) {
				auto scriptableEntity = scriptComponent.InstantiateScript(scriptComponent);
				scriptableEntity->m_Entity = { entity, this };
				scriptComponent.OnCreateFunction(scriptComponent.Instance);
			}

			scriptComponent.OnUpdateFunction(scriptComponent.Instance);
		});
	}

	void Scene::RenderSceneEditor(const glm::mat4& projectionView) {
		Renderer2D::BeginScene(projectionView);

		auto renderables = m_Registry.view<TransformComponent, SpriteRendererComponent>();
		for (auto entity : renderables) {
			Renderer2D::SubmitQuadEntity({ entity, this });
		}

		Renderer2D::EndScene();
	}

	void Scene::RenderSceneRuntime() {
		Entity primaryCamera = GetPrimaryCameraEntity();

		if (!primaryCamera.IsInvalidEntity()) {
			QK_CORE_WARN("No primary camera exist in the scene");
			return;
		}


		const auto& projection	   = primaryCamera.GetComponent<CameraComponent>().Camera.GetProjection();
		const auto cameraTransform = glm::inverse(primaryCamera.GetComponent<TransformComponent>().GetTransform());

		Renderer2D::BeginScene(projection * cameraTransform);

		auto renderables = m_Registry.view<TransformComponent, SpriteRendererComponent>();
		for (auto entity : renderables) {
			Renderer2D::SubmitQuadEntity({ entity, this });
		}

		Renderer2D::EndScene();
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height) {
		if (m_ViewportWidth == width && m_ViewportHeight == height)
			return;

		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize our non-FixedAspectRatio cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view) {
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}
	}

	Entity Scene::GetPrimaryCameraEntity() {
		auto view = m_Registry.view<CameraComponent>();

		for (auto entity : view) {
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.IsPrimary)
				return Entity{ entity, this };
		}

		return {};
	}

	Entity Scene::FindEntityByName(std::string_view name) {
		auto view = m_Registry.view<TagComponent>();

		for (auto entity : view) {
			const TagComponent& tc = view.get<TagComponent>(entity);
			if (tc.Tag == name)
				return Entity{ entity, this };
		}

		return {};
	}

}
