

#pragma once
#include "yaml-cpp/yaml.h"


namespace Quirk {

	class SceneSerializer {
	public: 
		static void Serialize(const Ref<Scene>& scene, const std::wstring& filePath);
		static bool Deserialize(const Ref<Scene>& scene, const std::wstring& filePath);

	private:
		static void SerializeEntity(YAML::Emitter& emitter, Entity entity);
		static void DeserializeEntity(YAML::Node entityNode, Entity entity);
	};

}
