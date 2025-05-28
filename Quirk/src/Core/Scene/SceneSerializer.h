

#pragma once

#include "Core/Utility/View.h"

#include "yaml-cpp/yaml.h"


namespace Quirk {

    class Scene;
    class Entity;

	class SceneSerializer {
	public: 
		static void Serialize(View<Scene> scene, const std::filesystem::path& filePath);
		static bool Deserialize(View<Scene> scene, const std::filesystem::path& filePath);

	private:
		static void SerializeEntity(YAML::Emitter& emitter, Entity entity);
		static void DeserializeEntity(YAML::Node entityNode, Entity entity);
	};

}
