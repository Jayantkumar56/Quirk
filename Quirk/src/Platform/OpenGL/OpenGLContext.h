

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

		struct ContextData {
			HWND WindowHandle;
			HGLRC GLContext;
			HDC DeviceContext;

			ContextData() : WindowHandle(nullptr), GLContext(nullptr), DeviceContext(nullptr) {}
		};

	public:
		static WGL s_WGL;

	public:
		OpenGLContext() = default;
		~OpenGLContext();

		virtual void Init() override;
		virtual inline void SwapBuffer() override;

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

