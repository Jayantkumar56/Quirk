

#include "Qkpch.h"
#include "Core/Scene/Scene.h"
#include "Core/Scene/Entity.h"
#include "Core/Renderer/Renderer2D.h"
#include "Core/Scene/ScriptableEntity.h"
#include "Core/Utility/Time.h"

#include "box2d/box2d.h"

namespace Quirk {

	static b2BodyType RigidBody2DTypeToBox2DBodyType(RigidBody2DComponent::BodyType type) {
		switch (type) {
			case Quirk::RigidBody2DComponent::Static:	 return b2_staticBody;
			case Quirk::RigidBody2DComponent::Dynamic:	 return b2_dynamicBody;
			case Quirk::RigidBody2DComponent::Kinematic: return b2_kinematicBody;
		}

		QK_ASSERT(false, "Invalid Body type provided");
		return b2_staticBody;
	}

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

		// Updating physics
		auto timeStep = Time::GetDeltaTime();
		b2World_Step(*m_PhysicsWorld, (float)timeStep, 4);

		auto view = m_Registry.view<RigidBody2DComponent>();
		for (auto e : view) {
			Entity entity = { e, this };

			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d		= entity.GetComponent<RigidBody2DComponent>();

			b2BodyId* bodyId = (b2BodyId*)rb2d.RuntimeBody;

			b2Vec2 position = b2Body_GetPosition(*bodyId);
			transform.Translation.x = position.x;
			transform.Translation.y = position.y;

			transform.Rotation.z = b2Rot_GetAngle(b2Body_GetRotation(*bodyId));
		}
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

		if (primaryCamera.IsInvalidEntity()) {
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

	void Scene::OnRuntimeStart() {
		b2WorldDef worldDef = b2DefaultWorldDef();
		m_PhysicsWorld = new b2WorldId(b2CreateWorld(&worldDef));

		auto view = m_Registry.view<RigidBody2DComponent>();
		for (auto e : view) {
			Entity entity = { e, this };

			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d	    = entity.GetComponent<RigidBody2DComponent>();

			b2BodyDef bodyDef  = b2DefaultBodyDef();
			bodyDef.type	   = RigidBody2DTypeToBox2DBodyType(rb2d.Type);
			bodyDef.position.x = transform.Translation.x;
			bodyDef.position.y = transform.Translation.y;
			bodyDef.rotation   = b2MakeRot(transform.Rotation.z);
			bodyDef.fixedRotation = rb2d.FixedRotation;

			b2BodyId* bodyId = new b2BodyId(b2CreateBody(*m_PhysicsWorld, &bodyDef));
			rb2d.RuntimeBody = bodyId;

			if (entity.HasComponent<BoxCollider2DComponent>()) {
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

				b2Polygon boxShape = b2MakeBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y);
				b2ShapeDef boxShapeDef  = b2DefaultShapeDef();
				boxShapeDef.density     = bc2d.Density;
				boxShapeDef.friction    = bc2d.Friction;
				boxShapeDef.restitution = bc2d.Restitution;

				b2ShapeId* shapeId = new b2ShapeId(b2CreatePolygonShape(*bodyId, &boxShapeDef, &boxShape));
				bc2d.RuntimeFixture = shapeId;
			}
		}
	}

	void Scene::OnRuntimeStop() {
		b2DestroyWorld(*m_PhysicsWorld);
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
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
