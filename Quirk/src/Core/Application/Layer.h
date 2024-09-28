

#pragma once

#include "Core/Input/Events.h"
#include <string>

namespace Quirk {

	class Layer {
	public:
		Layer(const std::string& name): m_DebugName(name) {}
		virtual ~Layer() = default;

		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;

		virtual bool OnEvent(Event& event) = 0;
		virtual void OnUpdate() = 0;
		virtual void OnImguiUiUpdate() = 0;

	protected:
		std::string m_DebugName;
	};

	class LayerStack {
	public:
		LayerStack() = delete;
		~LayerStack() = delete;

		static bool HandleEvent(Event& event);
		static void UpdateLayers();
		static void UpdateImguiUiLayers();

		static void PushLayer(Layer* layer);
		static void PopLayer();
		static void AddLayer(Layer* layer, int position);
		static void RemoveLayer(Layer* layer);

		static void PushOverlay(Layer* overlay);
		static void PopOverlay();
		static void AddOverlay(Layer* overlay, int position);
		static void RemoveOverlay(Layer* overlay);

	private:
		static std::vector<Layer*> m_Layers;
		static std::vector<Layer*> m_OverLays;
	};

}

