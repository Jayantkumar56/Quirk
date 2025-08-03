

#include "Core/Core.h"
#include "Core/Scene/Scene.h"
#include "Core/Scene/Entity.h"
#include "SceneSerializer.h"
#include "Core/Assets/Geometry/PrimitiveMeshGenerator.h"
#include "Core/Utility/YamlUtility.h"


namespace Quirk {

	void SceneSerializer::Serialize(View<Scene> scene, const std::filesystem::path& filePath) {
		YAML::Emitter out;

		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "New Scene";

		out << YAML::Key << "Entities" << YAML::BeginSeq;

		for (auto entity : scene->m_Registry.view<entt::entity>()) {
			Entity entityToSerialize = { entity, scene.Get() };
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

	bool SceneSerializer::Deserialize(View<Scene> scene, const std::filesystem::path& filePath) {
		std::ifstream fileStream(filePath);
		std::stringstream strStream;

		strStream << fileStream.rdbuf();

		YAML::Node data = YAML::Load(strStream);
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();

		auto entities = data["Entities"];
		for (auto entity : entities) {
			Entity createdEntity = scene->CreateEntity(entity["TagComponent"]["Tag"].as<std::string>(), entity["Entity"].as<uint64_t>());
			DeserializeEntity(entity, createdEntity);
		}

		return true;
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
			emitter << YAML::Key << "Rotation"    << YAML::Value << component.Rotation;
			emitter << YAML::Key << "Scale"       << YAML::Value << component.Scale;

			emitter << YAML::EndMap;
		}

		if (entity.HasComponent<SpriteRendererComponent>()) {
			auto& component = entity.GetComponent<SpriteRendererComponent>();

			emitter << YAML::Key << "SpriteRendererComponent";
			emitter << YAML::BeginMap;

			emitter << YAML::Key << "Color" << YAML::Value << component.Color;

			emitter << YAML::Key << "Texture" << YAML::Value << (uint64_t)component.Texture;

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

			emitter << YAML::Key << "PerspectiveFOV"   << YAML::Value << glm::degrees(component.Camera.GetPerspectiveVerticalFOV());
			emitter << YAML::Key << "PerspectiveNear"  << YAML::Value << component.Camera.GetPerspectiveNearClip();
			emitter << YAML::Key << "PerspectiveFar"   << YAML::Value << component.Camera.GetPerspectiveFarClip();

			emitter << YAML::Key << "OrthographicSize" << YAML::Value << component.Camera.GetOrthographicSize();
			emitter << YAML::Key << "OrthographicNear" << YAML::Value << component.Camera.GetOrthographicNearClip();
			emitter << YAML::Key << "OrthographicFar"  << YAML::Value << component.Camera.GetOrthographicFarClip();

			emitter << YAML::EndMap;			// Camera

			emitter << YAML::Key << "Primary"          << YAML::Value << component.IsPrimary;
			emitter << YAML::Key << "FixedAspectRatio" << YAML::Value << component.FixedAspectRatio;

			emitter << YAML::EndMap;
		}

		if (entity.HasComponent<MeshRendererComponent>()) {
			auto& component = entity.GetComponent<MeshRendererComponent>();

			emitter << YAML::Key << "MeshRendererComponent";

			emitter << YAML::BeginMap;
			emitter << YAML::Key << "MeshType"  << YAML::Value << MeshTypeToString(component.MeshObject.Type).data();
			emitter << YAML::Key << "Ambient"   << YAML::Value << component.MaterialProperties.Ambient;
			emitter << YAML::Key << "Diffuse"   << YAML::Value << component.MaterialProperties.Diffuse;
			emitter << YAML::Key << "Specular"  << YAML::Value << component.MaterialProperties.Specular;
			emitter << YAML::Key << "Shininess" << YAML::Value << component.MaterialProperties.Shininess;
			emitter << YAML::EndMap;
		}

		if (entity.HasComponent<LightComponent>()) {
			auto& component = entity.GetComponent<LightComponent>();

			emitter << YAML::Key << "LightComponent";

			emitter << YAML::BeginMap;
			emitter << YAML::Key << "LightType" << YAML::Value << LightTypeToString(component.Type).data();
			emitter << YAML::Key << "Color"     << YAML::Value << component.Color;
			emitter << YAML::Key << "Ambient"   << YAML::Value << component.Ambient;
			emitter << YAML::Key << "Diffuse"   << YAML::Value << component.Diffuse;
			emitter << YAML::Key << "Specular"  << YAML::Value << component.Specular;
			emitter << YAML::EndMap;
		}

		emitter << YAML::EndMap;
	}

	void SceneSerializer::DeserializeEntity(YAML::Node entityNode, Entity entity) {
		// Transform component already exists for any created entity
		{
			auto& transform = entity.GetComponent<TransformComponent>();
			auto deserializedComponent = entityNode["TransformComponent"];

			transform.Translation = deserializedComponent["Translation"].as<glm::vec3>();
			transform.Rotation	  = deserializedComponent["Rotation"].as<glm::vec3>();
			transform.Scale		  = deserializedComponent["Scale"].as<glm::vec3>();
		}

		if(auto deserializedComponent = entityNode["SpriteRendererComponent"];  deserializedComponent) {
			auto& component = entity.AddComponent<SpriteRendererComponent>(deserializedComponent["Color"].as<glm::vec4>());
			component.Texture = deserializedComponent["Texture"].as<uint64_t>();
		}

		if (auto deserializedComponent = entityNode["CameraComponent"];			deserializedComponent) {
			auto& component = entity.AddComponent<CameraComponent>();

			//		Camera Deserialization
			auto deserializedCamera = deserializedComponent["Camera"];

			component.Camera.m_PerspectiveFOV  = glm::radians(deserializedCamera["PerspectiveFOV"].as<float>());
			component.Camera.m_PerspectiveNear = deserializedCamera["PerspectiveNear"].as<float>();
			component.Camera.m_PerspectiveFar  = deserializedCamera["PerspectiveFar"].as<float>();

			component.Camera.m_OrthographicSize = deserializedCamera["OrthographicSize"].as<float>();
			component.Camera.m_OrthographicNear = deserializedCamera["OrthographicNear"].as<float>();
			component.Camera.m_OrthographicFar  = deserializedCamera["OrthographicFar"].as<float>();

			if (deserializedCamera["ProjectionType"].as<std::string>() == "Perspective")
				component.Camera.SetProjectionType(SceneCamera::ProjectionType::Perspective);
			else if (deserializedCamera["ProjectionType"].as<std::string>() == "Orthographic")
				component.Camera.SetProjectionType(SceneCamera::ProjectionType::Orthographic);
			//		Camera Deserialization

			component.IsPrimary		   = deserializedComponent["Primary"].as<bool>();
			component.FixedAspectRatio = deserializedComponent["FixedAspectRatio"].as<bool>();
		}

		if (auto deserializedComponent = entityNode["MeshRendererComponent"];  deserializedComponent) {
			auto& component      = entity.AddComponent<MeshRendererComponent>();
			component.MeshObject = PrimitiveMeshGenerator::Generate(StringToMeshType(deserializedComponent["MeshType"].as<std::string>()));
			component.MaterialProperties.Ambient   = deserializedComponent["Ambient"].as<glm::vec3>();
			component.MaterialProperties.Diffuse   = deserializedComponent["Diffuse"].as<glm::vec3>();
			component.MaterialProperties.Specular  = deserializedComponent["Specular"].as<glm::vec3>();
			component.MaterialProperties.Shininess = deserializedComponent["Shininess"].as<float>();
		}

		if (auto deserializedComponent = entityNode["LightComponent"];  deserializedComponent) {
			auto& component = entity.AddComponent<LightComponent>();

			component.Type     = StringToLightType(deserializedComponent["LightType"].as<std::string>());
			component.Color    = deserializedComponent["Color"].as<glm::vec3>();
			component.Ambient  = deserializedComponent["Ambient"].as<float>();
			component.Diffuse  = deserializedComponent["Diffuse"].as<float>();
			component.Specular = deserializedComponent["Specular"].as<float>();
		}
	}

}
