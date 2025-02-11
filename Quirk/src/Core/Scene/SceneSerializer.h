

#pragma once
#include "yaml-cpp/yaml.h"


namespace Quirk {

	class SceneSerializer {
	public: 
		static void Serialize(const Ref<Scene>& scene, const std::filesystem::path& filePath);
		static bool Deserialize(const Ref<Scene>& scene, const std::filesystem::path& filePath);

	private:
		static void SerializeEntity(YAML::Emitter& emitter, Entity entity);
		static void DeserializeEntity(YAML::Node entityNode, Entity entity);
	};

}
