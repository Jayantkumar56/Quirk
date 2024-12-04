

#pragma once

#include "Core/core.h"
#include "Core/Scene/SceneCamera.h"
#include "Core/Renderer/Texture.h"
#include "Core/Utility/UUID.h"

#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"


namespace Quirk {

	struct UUIDComponent {
		UUID Uuid;

		UUIDComponent() = default;
		UUIDComponent(const UUIDComponent&) = default;
		UUIDComponent(const UUID& uuid) : Uuid(uuid) {}
		UUIDComponent(uint64_t uuid) : Uuid(uuid) {}
	};

	struct TagComponent {
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag) : Tag(tag) {}
	};

	struct TransformComponent {
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation) : Translation(translation) {}

		glm::mat4 GetTransform() const {
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
			return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct SpriteRendererComponent {
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> Texture;
		float TillingFactor = 1.0f;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color) : Color(color) {}
	};

	struct CameraComponent {
		SceneCamera Camera;
		bool IsPrimary = false;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	// Forward declaration
	class ScriptableEntity;

	struct NativeScriptComponent {
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity* (*InstantiateScript)(NativeScriptComponent&);
		void (*DestroyScript)(NativeScriptComponent*);

		void (*OnCreateFunction)(ScriptableEntity*);
		void (*OnDestroyFunction)(ScriptableEntity*);
		void (*OnUpdateFunction)(ScriptableEntity*);

		template<typename T>
		void Bind() {
			InstantiateScript = [](NativeScriptComponent& scriptComponent) {
				scriptComponent.Instance = static_cast<ScriptableEntity*>(new T());
				return scriptComponent.Instance;
			};

			DestroyScript = [](NativeScriptComponent* scriptComponent) {
				delete static_cast<T*>(scriptComponent->Instance);
				scriptComponent->Instance = nullptr;
			};

			OnCreateFunction  = [](ScriptableEntity* script) { ((T*)script)->OnCreate(); };

			OnDestroyFunction = [](ScriptableEntity* script) { ((T*)script)->OnDestroy(); };

			OnUpdateFunction  = [](ScriptableEntity* script) { ((T*)script)->OnUpdate(); };
		}
	};

	struct RigidBody2DComponent {
		enum BodyType{ Static = 0, Dynamic, Kinematic };

		BodyType Type = BodyType::Static;
		bool FixedRotation = false;

		// Runtime storage
		void* RuntimeBody = nullptr;

		RigidBody2DComponent() = default;
		RigidBody2DComponent(const RigidBody2DComponent&) = default;
	};

	struct BoxCollider2DComponent {
		glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Size   = { 0.5f, 0.5f };

		float Density     = 1.0f;
		float Friction    = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

		// Runtime storage
		void* RuntimeFixture = nullptr;

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
	};

	class ComponentsIterator {
	public:
		template<typename function>
		static inline void Iterate(function fun) {	
			fun.operator() < TagComponent > ("Tag");
			fun.operator() < TransformComponent > ("Transform");
			fun.operator() < SpriteRendererComponent > ("Sprite Renderer");
			fun.operator() < CameraComponent > ("Camera");
			fun.operator() < NativeScriptComponent > ("Native Script");
			fun.operator() < RigidBody2DComponent > ("RigidBody2D");
			fun.operator() < BoxCollider2DComponent > ("BoxCollider2D");
		}
	};

}
