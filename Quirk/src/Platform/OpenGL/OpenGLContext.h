

#pragma once

#include "Core/Core.h"
#ifdef QK_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include<Windows.h>

typedef void (*Wglproc)(void);
#endif // QK_PLATFORM_WINDOWS


#if (QK_RENDERING_API == QK_RENDERER_OPENGL)

#include "glad/glad.h"
#include "wglext.h"
#include "Core/Renderer/GraphicalContext.h"

namespace Quirk {

	class OpenGLContext : public GraphicalContext {
	public:
		OpenGLContext() = default;
		virtual ~OpenGLContext() {}

		virtual void Init(Window* ContextData) override = 0;
		virtual void SwapBuffer() override = 0;
	};

#ifdef QK_PLATFORM_WINDOWS

	class OpenGLWindowsContext : public OpenGLContext {
	private:
		struct WGL {
			HINSTANCE                           OpenGL32DLL;
			PFNWGLCHOOSEPIXELFORMATARBPROC		ChoosePixelFormatARB;
			PFNWGLSWAPINTERVALEXTPROC           SwapIntervalEXT;
			PFNWGLGETPIXELFORMATATTRIBIVARBPROC GetPixelFormatAttribivARB;
			PFNWGLGETEXTENSIONSSTRINGEXTPROC    GetExtensionsStringEXT;
			PFNWGLGETEXTENSIONSSTRINGARBPROC    GetExtensionsStringARB;
			PFNWGLCREATECONTEXTATTRIBSARBPROC   CreateContextAttribsARB;
		};

		struct ContextData {
			HWND WindowHandle;
			HGLRC GLContext;
			HDC DeviceContext;

			ContextData() : WindowHandle(nullptr), GLContext(nullptr), DeviceContext(nullptr) {}
		};

	public:
		static WGL s_WGL;

	public:
		OpenGLWindowsContext() = default;
		~OpenGLWindowsContext();

		virtual void Init(Window* ContextData) override;
		virtual void SwapBuffer() override;

		inline HGLRC GetGLContext() const { return m_ContextData.GLContext; }
		inline HDC GetDeviceContext() const { return m_ContextData.DeviceContext; }

	private:
		static Wglproc GetProcAddressWGL(const char* procName);
		static LRESULT CALLBACK WndProcTemp(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		ContextData m_ContextData;
	};

#endif // QK_PLATFORM_WINDOWS

}

#endif // (QK_RENDERING_API == QK_RENDERER_OPENGL)

