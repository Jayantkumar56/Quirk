
#include "Qkpch.h"

#include "Core/core.h"
#include "Core/Scene/Scene.h"
#include "Core/Scene/Entity.h"
#include "SceneSerializer.h"

namespace YAML {

	Emitter& operator << (Emitter& out, const glm::vec3& v) {
		out << Flow;
		out << BeginSeq << v.x << v.y << v.z << EndSeq;
		return out;
	}

	Emitter& operator << (Emitter& out, const glm::vec4& v) {
		out << Flow;
		out << BeginSeq << v.x << v.y << v.z << v.w << EndSeq;
		return out;
	}

}

namespace Quirk {

	void SceneSerializer::Serialize(const Ref<Scene>& scene, const std::string& filePath) {
		YAML::Emitter out;

		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "New Scene";

		out << YAML::Key << "Entities" << YAML::BeginSeq;

		for (auto entity : scene->m_Registry.view<entt::entity>()) {
			Entity entityToSerialize = { entity, scene.get() };
			if (!entityToSerialize)
				break;

			SerializeEntity(out, entityToSerialize);
		}

		out << YAML::EndSeq;
		out << YAML::EndMap;

		if (!out.good()) {
			QK_WARN("Unable to serialize scene!");
			return;
		}

		std::ofstream outFile(filePath);
		outFile << out.c_str();
	}

	bool SceneSerializer::Deserialize(const Ref<Scene>& scene, const std::string& filePath) {
		return false;
	}

	void SceneSerializer::SerializeEntity(YAML::Emitter& emitter, Entity entity) {
		emitter << YAML::BeginMap;

		emitter << YAML::Key << "Entity" << YAML::Value << (uint64_t)entity.GetComponent<UUIDComponent>().Uuid;

		if (entity.HasComponent<TagComponent>()) {
			emitter << YAML::Key << "TagComponent";
			emitter << YAML::BeginMap;
			emitter << YAML::Key << "Tag" << YAML::Value << entity.GetComponent<TagComponent>().Tag;
			emitter << YAML::EndMap;
		}

		if (entity.HasComponent<TransformComponent>()) {
			auto& component = entity.GetComponent<TransformComponent>();

			emitter << YAML::Key << "TransformComponent";
			emitter << YAML::BeginMap;

			emitter << YAML::Key << "Translation" << YAML::Value << component.Translation;
			emitter << YAML::Key << "Rotation" << YAML::Value << component.Rotation;
			emitter << YAML::Key << "Scale" << YAML::Value << component.Scale;

			emitter << YAML::EndMap;
		}

		// TO DO: serialize correct texture
		if (entity.HasComponent<SpriteRendererComponent>()) {
			auto& component = entity.GetComponent<SpriteRendererComponent>();

			emitter << YAML::Key << "SpriteRendererComponent";
			emitter << YAML::BeginMap;

			emitter << YAML::Key << "Color" << YAML::Value << component.Color;
			emitter << YAML::Key << "Texture" << YAML::Value << "";
			emitter << YAML::Key << "TillingFactor" << YAML::Value << component.TillingFactor;

			emitter << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>()) {
			auto& component = entity.GetComponent<CameraComponent>();

			emitter << YAML::Key << "CameraComponent";
			emitter << YAML::BeginMap;

			emitter << YAML::Key << "Camera";
			emitter << YAML::BeginMap;			// Camera

			emitter << YAML::Key << "ProjectionType";
			if (component.Camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				emitter << YAML::Value << "Perspective";
			if (component.Camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				emitter << YAML::Value << "Orthographic";

			emitter << YAML::Key << "PerspectiveFOV" << YAML::Value << glm::degrees(component.Camera.GetPerspectiveVerticalFOV());
			emitter << YAML::Key << "PerspectiveNear" << YAML::Value << component.Camera.GetPerspectiveNearClip();
			emitter << YAML::Key << "PerspectiveFar" << YAML::Value << component.Camera.GetPerspectiveFarClip();

			emitter << YAML::Key << "m_OrthographicSize" << YAML::Value << component.Camera.GetOrthographicSize();
			emitter << YAML::Key << "OrthographicNear" << YAML::Value << component.Camera.GetOrthographicNearClip();
			emitter << YAML::Key << "OrthographicFar" << YAML::Value << component.Camera.GetOrthographicFarClip();

			emitter << YAML::EndMap;			// Camera

			emitter << YAML::Key << "Primary" << YAML::Value << component.Primary;
			emitter << YAML::Key << "FixedAspectRatio" << YAML::Value << component.FixedAspectRatio;

			emitter << YAML::EndMap;
		}

		emitter << YAML::EndMap;
	}

}
