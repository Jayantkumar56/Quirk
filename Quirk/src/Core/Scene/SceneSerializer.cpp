
#include "Qkpch.h"

#include "Core/Core.h"
#include "Core/Scene/Scene.h"
#include "Core/Scene/Entity.h"
#include "SceneSerializer.h"
#include "Core/Renderer/Geometry/PrimitiveMeshGenerator.h"

namespace YAML {

	static Emitter& operator << (Emitter& out, const glm::vec2& v) {
		out << Flow;
		out << BeginSeq << v.x << v.y << EndSeq;
		return out;
	}

	static Emitter& operator << (Emitter& out, const glm::vec3& v) {
		out << Flow;
		out << BeginSeq << v.x << v.y << v.z << EndSeq;
		return out;
	}

	static Emitter& operator << (Emitter& out, const glm::vec4& v) {
		out << Flow;
		out << BeginSeq << v.x << v.y << v.z << v.w << EndSeq;
		return out;
	}

	template<>
	struct convert<glm::vec2> {
		static Node encode(const glm::vec2& vec) {
			Node node;
			node.push_back(vec.x);
			node.push_back(vec.y);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& vec) {
			if (!node.IsSequence() || node.size() != 2)
				return false;

			vec[0] = node[0].as<float>();
			vec[1] = node[1].as<float>();

			return true;
		}
	};

	template<>
	struct convert<glm::vec3> {
		static Node encode(const glm::vec3& vec) {
			Node node;
			node.push_back(vec.x);
			node.push_back(vec.y);
			node.push_back(vec.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& vec) {
			if (!node.IsSequence() || node.size() != 3)
				return false;

			vec[0] = node[0].as<float>();
			vec[1] = node[1].as<float>();
			vec[2] = node[2].as<float>();

			return true;
		}
	};

	template<>
	struct convert<glm::vec4> {
		static Node encode(const glm::vec4& vec) {
			Node node;
			node.push_back(vec.x);
			node.push_back(vec.y);
			node.push_back(vec.z);
			node.push_back(vec.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& vec) {
			if (!node.IsSequence() || node.size() != 4)
				return false;

			vec[0] = node[0].as<float>();
			vec[1] = node[1].as<float>();
			vec[2] = node[2].as<float>();
			vec[3] = node[3].as<float>();

			return true;
		}
	};

}

namespace Quirk {

	void SceneSerializer::Serialize(const Ref<Scene>& scene, const std::filesystem::path& filePath) {
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

	bool SceneSerializer::Deserialize(const Ref<Scene>& scene, const std::filesystem::path& filePath) {
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

			std::string filePath = (component.Texture != nullptr) ? std::filesystem::proximate(component.Texture->GetPath()).string() : "";
			emitter << YAML::Key << "Texture" << YAML::Value << filePath;

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

		if (entity.HasComponent<MeshComponent>()) {
			auto& component = entity.GetComponent<MeshComponent>();

			emitter << YAML::Key << "MeshComponent";

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

			std::filesystem::path texturefilePath = deserializedComponent["Texture"].as<std::string>();
			if (std::filesystem::exists(texturefilePath))
				component.Texture = Texture2D::Create(texturefilePath);
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

		if (auto deserializedComponent = entityNode["MeshComponent"];  deserializedComponent) {
			auto& component      = entity.AddComponent<MeshComponent>();
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
