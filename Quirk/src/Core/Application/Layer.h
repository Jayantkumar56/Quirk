

#pragma once

#include <string>
#include <vector>

#include "Core/Input/Events.h"

namespace Quirk {

	class Layer {
	public:
		Layer(const std::string& name);
		virtual ~Layer() {}

		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;

		virtual bool OnEvent(Event& event) = 0;
		virtual void OnUpdate() = 0;

	protected:
		std::string m_DebugName;
	};

	class LayerStack {
	public:
		LayerStack() = default;
		~LayerStack() = default;

		bool HandleEvent(Event& event);
		void UpdateLayers();

		void PushLayer(Layer* layer);
		void PopLayer();
		void AddLayer(Layer* layer, int position);
		void RemoveLayer(Layer* layer);

		void PushOverlay(Layer* overlay);
		void PopOverlay();
		void AddOverlay(Layer* overlay, int position);
		void RemoveOverlay(Layer* overlay);

	private:
		std::vector<Layer*> m_Layers;
		std::vector<Layer*> m_OverLays;
	};

}

