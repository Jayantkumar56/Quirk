

#pragma once

#include "Core/Application/Layer.h"

#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_opengl3.h"


namespace Quirk {

	class  ImguiLayer : public Layer {
		struct ContextData {
			HDC DeviceContext;
			ContextData() : DeviceContext(nullptr) {}
		};

	public:
		ImguiLayer();
		~ImguiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual bool OnEvent(Event& event) override;
		virtual void OnUpdate() override;

		void Begin() const;
		void End() const;

	private:
		static void CleanupDeviceWGL(HWND hWnd, ContextData* data);
		static bool CreateDeviceWGL(HWND hWnd, ContextData* data);

		static void HookRendererCreateWindow(ImGuiViewport* viewport);
		static void HookRendererDestroyWindow(ImGuiViewport* viewport);
		static void HookRendererSwapBuffers(ImGuiViewport* viewport, void*);
		static void HookPlatformRenderWindow(ImGuiViewport* viewport, void*);

	private:
		ContextData m_MainWindowContextData;
		static HGLRC GLContext;
	};

}


