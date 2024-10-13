
#include "qkpch.h"

#include "Core/Core.h"

#include "Core/Application/Application.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "Core/Renderer/RendererAPI.h"

#include "Core/Imgui/ImguiUI.h"

// From Glad.h
#define GL_FALSE 0
#define GL_TRUE 1

namespace Quirk {

	HGLRC ImguiUI::GLContext = nullptr;
	ImguiUI::ContextData ImguiUI::s_MainWindowContextData;
	bool ImguiUI::s_DockingEnabled;
	PFNWGLCHOOSEPIXELFORMATARBPROC ImguiUI::ChoosePixelFormatARB;

	void ImguiUI::CleanupDeviceWGL(HWND hWnd, ContextData* data) {
		wglMakeCurrent(nullptr, nullptr);
		ReleaseDC(hWnd, data->DeviceContext);
	}

	bool ImguiUI::CreateDeviceWGL(HWND hWnd, ContextData* data) {
		data->DeviceContext = GetDC(hWnd);
		int pixelFormat = 0;
		unsigned int numPixelFormat = 0;

		const int pixelAttribs[] = {
			WGL_DRAW_TO_WINDOW_ARB,		GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB,		GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB,		GL_TRUE,
			WGL_ACCELERATION_ARB,		WGL_FULL_ACCELERATION_ARB,
			WGL_PIXEL_TYPE_ARB,			WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB,			32,
			WGL_DEPTH_BITS_ARB,			24,
			WGL_STENCIL_BITS_ARB,		8,
			WGL_SAMPLE_BUFFERS_ARB,		GL_TRUE,
			WGL_SAMPLES_ARB,			4,
			0, // End
		};

		QK_CORE_ASSERTEX(
			ChoosePixelFormatARB(data->DeviceContext, pixelAttribs, NULL, 1, &pixelFormat, &numPixelFormat),
			"Failed to choose pixel format!"
		);

		PIXELFORMATDESCRIPTOR PFD;
		QK_CORE_ASSERTEX(DescribePixelFormat(data->DeviceContext, pixelFormat, sizeof(PFD), &PFD),	"Failed to describe pixel format!");
		QK_CORE_ASSERTEX(SetPixelFormat(data->DeviceContext, pixelFormat, &PFD),					"Unable to set pixel format!");

		return true;
	}

	void ImguiUI::HookRendererCreateWindow(ImGuiViewport* viewport) {
		QK_CORE_ASSERT(viewport->RendererUserData == NULL, "Non Empty RendererUserData (from imgui)");

		ContextData* data = IM_NEW(ContextData);
		CreateDeviceWGL((HWND)viewport->PlatformHandle, data);
		viewport->RendererUserData = data;
	}

	void ImguiUI::HookRendererDestroyWindow(ImGuiViewport* viewport) {
		if (viewport->RendererUserData != NULL) {
			ContextData* data = (ContextData*)viewport->RendererUserData;
			CleanupDeviceWGL((HWND)viewport->PlatformHandle, data);
			IM_DELETE(data);
			viewport->RendererUserData = NULL;
		}
	}

	void ImguiUI::HookRendererSwapBuffers(ImGuiViewport* viewport, void*) {
		if (ContextData* data = (ContextData*)viewport->RendererUserData; data) {
			SwapBuffers(data->DeviceContext);
		}
	}

	void ImguiUI::HookPlatformRenderWindow(ImGuiViewport* viewport, void*) {
		if (ContextData* data = (ContextData*)viewport->RendererUserData; data) {
			wglMakeCurrent(data->DeviceContext, GLContext);
		}
	}

	void ImguiUI::Init() {
		ChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");

		s_DockingEnabled = false;
		const char* glsl_version = "#version 410";

		Window& window = Application::Get().GetWindow();
		OpenGLContext* context = (OpenGLContext*)window.GetGraphicalContext();

		s_MainWindowContextData.DeviceContext = context->GetDeviceContext();
		GLContext = context->GetGLContext();

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi-Viewport / Platform Windows
		io.ConfigViewportsNoTaskBarIcon = true;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowMinSize.x = 400.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			style.WindowRounding = 1.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer backends
		ImGui_ImplWin32_InitForOpenGL((HWND)window.GetNativeWindow());
		ImGui_ImplOpenGL3_Init(glsl_version);

		// Win32+GL needs specific hooks for viewport, as there are specific things needed to tie Win32 and GL api.
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
			platform_io.Renderer_CreateWindow = HookRendererCreateWindow;
			platform_io.Renderer_DestroyWindow = HookRendererDestroyWindow;
			platform_io.Renderer_SwapBuffers = HookRendererSwapBuffers;
			platform_io.Platform_RenderWindow = HookPlatformRenderWindow;
		}
	}

	void ImguiUI::Terminate() {
		// Cleanup
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	void ImguiUI::Begin() {
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if (s_DockingEnabled) {
			ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
		}
	}

	void ImguiUI::End() {
		ImGuiIO& io = ImGui::GetIO();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional Platform Windows
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();

			// Restore the OpenGL rendering context to the main window DC, since platform windows might have changed it.
			wglMakeCurrent(s_MainWindowContextData.DeviceContext, GLContext);
		}
	}

}
