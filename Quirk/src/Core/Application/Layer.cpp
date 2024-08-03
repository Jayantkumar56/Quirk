
#include "Qkpch.h"
#include "Layer.h"

#include "Core/Imgui/ImguiLayer.h"

namespace Quirk {

	Layer::Layer(const std::string& name) :
			m_DebugName(name)
	{
	}

	bool LayerStack::HandleEvent(Event& event) {
		// providing event to overlays first and in the reverse order
		for (int i = static_cast<int>(m_OverLays.size()) - 1; i > -1; --i) {
			if (m_OverLays[i]->OnEvent(event))
				return true;
		}

		for (int i = static_cast<int>(m_Layers.size()) - 1; i > -1; --i) {
			if (m_Layers[i]->OnEvent(event))
				return true;
		}

		return false;
	}

	void LayerStack::UpdateLayers() {
		size_t i = 0, size = m_Layers.size();
		for ( ; i < size; ++i) {
			m_Layers[i]->OnUpdate();
		}

		for (i = 0, size = m_OverLays.size(); i < size; ++i) {
			m_OverLays[i]->OnUpdate();
		}
	}

    void LayerStack::UpdateImguiUiLayers() {
		ImguiLayer::Begin();

		size_t i = 0, size = m_Layers.size();
		for (; i < size; ++i) {
			m_Layers[i]->OnImguiUiUpdate();
		}

		for (i = 0, size = m_OverLays.size(); i < size; ++i) {
			m_OverLays[i]->OnImguiUiUpdate();
		}

		ImguiLayer::End();
    }

	void LayerStack::PushLayer(Layer* layer) {
		m_Layers.push_back(layer);
		layer->OnAttach();
	}

	void LayerStack::PopLayer() {
		m_Layers.pop_back();
	}

	void LayerStack::AddLayer(Layer* layer, int position) {
		m_Layers.insert(m_Layers.begin() + position, layer);
	}

	void LayerStack::RemoveLayer(Layer* layer) {
		size_t size = m_Layers.size();

		for (size_t i = 0; i < size -1; ++i) {
			if (m_Layers[i] == layer) 
				std::swap(m_Layers[i], m_Layers[i + 1]);
		}

		m_Layers.pop_back();
	}

	void LayerStack::PushOverlay(Layer* overlay) {
		m_OverLays.push_back(overlay);
		overlay->OnAttach();
	}

	void LayerStack::PopOverlay() {
		m_OverLays.pop_back();
	}

	void LayerStack::AddOverlay(Layer* overlay, int position) {
		m_OverLays.insert(m_OverLays.begin() + position, overlay);
	}

	void LayerStack::RemoveOverlay(Layer* overlay) {
		size_t size = m_OverLays.size();

		for (size_t i = 0; i < size - 1; ++i) {
			if (m_OverLays[i] == overlay)
				std::swap(m_OverLays[i], m_OverLays[i + 1]);
		}

		m_OverLays.pop_back();
	}

}
