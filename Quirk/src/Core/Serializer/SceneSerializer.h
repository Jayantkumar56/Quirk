

#pragma once
#include "yaml-cpp/yaml.h"


namespace Quirk {

	class SceneSerializer {
	public: 
		static void Serialize(const Ref<Scene>& scene, const std::string& filePath);
		static bool Deserialize(const Ref<Scene>& scene, const std::string& filePath);

	private:
		static void SerializeEntity(YAML::Emitter& emitter, Entity entity);
	};

}
