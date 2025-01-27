

#pragma once

#include "Core/Core.h"

#ifdef QK_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include<Windows.h>
typedef void (*Wglproc)(void);
#endif // QK_PLATFORM_WINDOWS

#include "wglext.h"
#include "Core/Renderer/GraphicalContext.h"

namespace Quirk {

#ifdef QK_PLATFORM_WINDOWS

	class OpenGLContext : public GraphicalContext {
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

	public:
		static void Init();
		inline static WGL s_WGL;

	public:
		OpenGLContext(Window& window);
		~OpenGLContext() = default;

		virtual void DestroyContext(Window& window) override;

		virtual inline void MakeContextCurrent() override {
			QK_CORE_ASSERTEX(wglMakeCurrent(m_DeviceContext, m_GLContext), "Failed to make GL context current!");
		}

		virtual inline void SetVSync(int interval) {
			QK_CORE_ASSERTEX(s_WGL.SwapIntervalEXT(interval), "Failed to Set VSync!");
		}

		virtual inline void SwapBuffer() {
			QK_CORE_ASSERTEX(SwapBuffers(m_DeviceContext), "Failed to Swap Buffer");
		}

		inline HGLRC GetGLContext()   const { return m_GLContext;     }
		inline HDC GetDeviceContext() const { return m_DeviceContext; }

	private:
		static Wglproc GetProcAddressWGL(const char* procName);
		static LRESULT CALLBACK WndProcTemp(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		HGLRC m_GLContext;
		HDC m_DeviceContext;
	};

#endif // QK_PLATFORM_WINDOWS

}

