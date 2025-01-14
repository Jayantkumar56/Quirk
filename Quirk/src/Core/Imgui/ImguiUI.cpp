
#include "qkpch.h"

#include "Core/Core.h"
#include "Core/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "Core/Imgui/ImguiUI.h"

// From Glad.h
#define GL_FALSE 0
#define GL_TRUE 1

namespace Quirk {

#ifdef QK_PLATFORM_WINDOWS

	void ImguiUI::Init(Window& window, Ref<GraphicalContext>& context) {
		// TO DO: remove this graphical context
		GLContext = ((OpenGLContext*)context.get())->GetGLContext();

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		m_Context = ImGui::CreateContext();
		ImGui::SetCurrentContext(m_Context);

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi-Viewport / Platform Windows
		io.ConfigViewportsNoTaskBarIcon = true;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowMinSize.x = 400.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			style.WindowRounding = 10.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		InitForOpenGL(window);
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

		if (DockingEnabled) {
			ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
		}
	}

	void ImguiUI::End(const Ref<GraphicalContext>& context) {
		ImGuiIO& io = ImGui::GetIO();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional Platform Windows
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();

			// Restore the OpenGL rendering context to the main window DC, since platform windows might have changed it.
			context->MakeContextCurrent();
		}
	}

	void ImguiUI::InitForOpenGL(Window& window) {
		const char* glsl_version = "#version 410";

		ImGui_ImplWin32_InitForOpenGL((HWND)window.GetNativeHandle());
		ImGui_ImplOpenGL3_Init(glsl_version);

		// Win32+GL needs specific hooks for viewport, as there are specific things needed to tie Win32 and GL api.
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();

			platform_io.Renderer_CreateWindow = [] (ImGuiViewport* viewport) {
				QK_CORE_ASSERT(viewport->RendererUserData == NULL, "Non Empty RendererUserData (from imgui)");
				ImguiUI::ContextData* data = IM_NEW(ImguiUI::ContextData);

				data->DeviceContext = GetDC((HWND)viewport->PlatformHandle);
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
					OpenGLContext::s_WGL.ChoosePixelFormatARB(data->DeviceContext, pixelAttribs, NULL, 1, &pixelFormat, &numPixelFormat),
					"Failed to choose pixel format!"
				);

				PIXELFORMATDESCRIPTOR PFD;
				QK_CORE_ASSERTEX(DescribePixelFormat(data->DeviceContext, pixelFormat, sizeof(PFD), &PFD),	"Failed to describe pixel format!");
				QK_CORE_ASSERTEX(SetPixelFormat(data->DeviceContext, pixelFormat, &PFD),					"Unable to set pixel format!");

				viewport->RendererUserData = data;
			};

			platform_io.Renderer_DestroyWindow = [] (ImGuiViewport* viewport) {
				if (viewport->RendererUserData != NULL) {
					ImguiUI::ContextData* data = (ImguiUI::ContextData*)viewport->RendererUserData;

					wglMakeCurrent(nullptr, nullptr);
					ReleaseDC((HWND)viewport->PlatformHandle, data->DeviceContext);

					IM_DELETE(data);
					viewport->RendererUserData = NULL;
				}
			};

			platform_io.Renderer_SwapBuffers = [] (ImGuiViewport* viewport, void*) {
				if (ImguiUI::ContextData* data = (ImguiUI::ContextData*)viewport->RendererUserData; data) {
					SwapBuffers(data->DeviceContext);
				}
			};

			platform_io.Platform_RenderWindow = [] (ImGuiViewport* viewport, void*) {
				if (ImguiUI::ContextData* data = (ImguiUI::ContextData*)viewport->RendererUserData; data) {
					wglMakeCurrent(data->DeviceContext, GLContext);
				}
			};
		}
	}

#endif // QK_PLATFORM_WINDOWS

}

