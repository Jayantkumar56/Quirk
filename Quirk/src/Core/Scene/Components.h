

#pragma once

#include "Core/Core.h"
#include "Core/Scene/SceneCamera.h"
#include "Core/Renderer/Texture.h"
#include "Core/Utility/UUID.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"

#include "Core/Renderer/Geometry/Geometry.h"


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
		glm::vec3 Translation { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation    { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale       { 1.0f, 1.0f, 1.0f };

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

	struct MeshComponent {
		Mesh MeshObject;

		MeshComponent() = default;
		MeshComponent(const MeshComponent&) = default;
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

			OnCreateFunction  = [](ScriptableEntity* script) { ((T*)script)->OnCreate();  };
			OnDestroyFunction = [](ScriptableEntity* script) { ((T*)script)->OnDestroy(); };
			OnUpdateFunction  = [](ScriptableEntity* script) { ((T*)script)->OnUpdate();  };
		}

		NativeScriptComponent() = default;
		NativeScriptComponent(const NativeScriptComponent&) = default;
	};

	// ComponentTypes::Identifiers    includes components which identifies an entity (UUIDComponent, TagComponent for now)
	// ComponentTypes::All		      includes all components which could be used by an entity
	// ComponentTypes::NonIdentifiers includes all components except identifier components
	enum class ComponentTypes { All, Identifiers, NonIdentifiers };

	class ComponentTypesIterator {
	public:
		template<ComponentTypes types, class function>
		static inline constexpr typename std::enable_if<types == ComponentTypes::All, void>::type Iterate(function fun) {
			fun.operator() < UUIDComponent 		     > ( "UUID"			   );
			fun.operator() < TagComponent 			 > ( "Tag"			   );
			fun.operator() < TransformComponent 	 > ( "Transform"	   );
			fun.operator() < SpriteRendererComponent > ( "Sprite Renderer" );
			fun.operator() < CameraComponent 		 > ( "Camera"		   );
			fun.operator() < NativeScriptComponent   > ( "Native Script"   );
			fun.operator() < MeshComponent           > ( "Mesh"            );
		}

		template<ComponentTypes types, class function>
		static inline constexpr typename std::enable_if<types == ComponentTypes::Identifiers, void>::type Iterate(function fun) {
			fun.operator() < UUIDComponent > ("UUID");
			fun.operator() < TagComponent  > ("Tag" );
		}

		template<ComponentTypes types, class function>
		static inline constexpr typename std::enable_if<types == ComponentTypes::NonIdentifiers, void>::type Iterate(function fun) {
			fun.operator() < TransformComponent      > ( "Transform"	   );
			fun.operator() < SpriteRendererComponent > ( "Sprite Renderer" );
			fun.operator() < CameraComponent         > ( "Camera"		   );
			fun.operator() < NativeScriptComponent   > ( "Native Script"   );
			fun.operator() < MeshComponent           > ( "Mesh"            );
		}
	};

}
