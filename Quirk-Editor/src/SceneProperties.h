

#pragma once

#include "Quirk.h"
#include "SceneViewport.h"

namespace Quirk {

	class SceneProperties : public Layer {
	public:
		SceneProperties(const SceneViewport& sceneViewport, const std::string& name = "Scene Properties");
		virtual ~SceneProperties() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual bool OnEvent(Event& event) override;
		virtual void OnUpdate() override;
		virtual void OnImguiUiUpdate() override;

	private:
		const SceneViewport& m_SceneViewport;
	};

}
