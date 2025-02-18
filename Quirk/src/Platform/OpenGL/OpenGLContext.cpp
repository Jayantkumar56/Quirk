

#include "Qkpch.h"
#include "Core/Core.h"

#include "OpenGLContext.h"
#include "glad/glad.h"

#include "Core/Frame/Window.h"

namespace Quirk {

#ifdef QK_PLATFORM_WINDOWS

	Wglproc OpenGLContext::GetProcAddressWGL(const char* procName) {	
		if (const Wglproc proc = (Wglproc)wglGetProcAddress(procName); proc) {
			return proc;
		}
		return (Wglproc)GetProcAddress(OpenGLContext::s_WGL.OpenGL32DLL, procName);
	}

	void OpenGLContext::Init() {
		// Creating a temporary window
		HWND tempWindowHandle = ::CreateWindowExW(
			0,
			WindowsWindow::GetWindowClassName().data(),	    // Window class
			L"Temp Window",									// Window text
			WS_OVERLAPPEDWINDOW,							// Window style
			CW_USEDEFAULT, CW_USEDEFAULT,					// Window Position
			800, 600,										// Window Width and Height
			NULL,											// Parent window    
			NULL,											// Menu
			WindowsWindow::GetApplicationHInstance(),		// Instance handle
			NULL											// Additional application data
		);

		QK_CORE_ASSERT(tempWindowHandle, "Failed to create dummy window!");

		HDC tempDeviceContext = ::GetDC(tempWindowHandle);
		QK_CORE_ASSERT(tempWindowHandle, "Windows failed to provide a device context!");

		PIXELFORMATDESCRIPTOR tempPFD = {};
		tempPFD.nSize           = sizeof(PIXELFORMATDESCRIPTOR);
		tempPFD.nVersion        = 1;
		tempPFD.dwFlags         = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		tempPFD.iPixelType      = PFD_TYPE_RGBA;
		tempPFD.cColorBits      = 32;
		tempPFD.cRedBits        = 0;
		tempPFD.cRedShift       = 0;
		tempPFD.cGreenBits      = 0;
		tempPFD.cGreenShift     = 0;
		tempPFD.cBlueBits       = 0;
		tempPFD.cBlueShift      = 0;
		tempPFD.cAlphaBits      = 0;
		tempPFD.cAlphaShift     = 0;
		tempPFD.cAccumBits      = 0;
		tempPFD.cAccumRedBits   = 0;
		tempPFD.cAccumGreenBits = 0;
		tempPFD.cAccumBlueBits  = 0;
		tempPFD.cAccumAlphaBits = 0;
		tempPFD.cDepthBits      = 24;
		tempPFD.cStencilBits    = 8;
		tempPFD.cAuxBuffers     = 0;
		tempPFD.iLayerType      = PFD_MAIN_PLANE;
		tempPFD.bReserved       = 0;
		tempPFD.dwLayerMask     = 0;
		tempPFD.dwVisibleMask   = 0;
		tempPFD.dwDamageMask    = 0;

		int pixelFmt = ChoosePixelFormat(tempDeviceContext, &tempPFD);
		QK_CORE_ASSERT(pixelFmt, "Failed to choose pixel format!");
		QK_CORE_ASSERTEX(SetPixelFormat(tempDeviceContext, pixelFmt, &tempPFD), "Failed to set pixel format!");

		HGLRC tempGLContext = wglCreateContext(tempDeviceContext);
		QK_CORE_ASSERT(tempGLContext, "Failed to create GL context!");
		QK_CORE_ASSERTEX(wglMakeCurrent(tempDeviceContext, tempGLContext), "Failed to make GL context current!");

		s_WGL.ChoosePixelFormatARB      = (PFNWGLCHOOSEPIXELFORMATARBPROC)      wglGetProcAddress("wglChoosePixelFormatARB"     );
		s_WGL.GetExtensionsStringEXT    = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)    wglGetProcAddress("wglGetExtensionsStringEXT"   );
		s_WGL.GetExtensionsStringARB    = (PFNWGLGETEXTENSIONSSTRINGARBPROC)    wglGetProcAddress("wglGetExtensionsStringARB"   );
		s_WGL.CreateContextAttribsARB   = (PFNWGLCREATECONTEXTATTRIBSARBPROC)   wglGetProcAddress("wglCreateContextAttribsARB"  );
		s_WGL.SwapIntervalEXT           = (PFNWGLSWAPINTERVALEXTPROC)           wglGetProcAddress("wglSwapIntervalEXT"          );
		s_WGL.GetPixelFormatAttribivARB = (PFNWGLGETPIXELFORMATATTRIBIVARBPROC) wglGetProcAddress("wglGetPixelFormatAttribivARB");

		s_WGL.OpenGL32DLL = LoadLibraryA("opengl32.dll");
		QK_CORE_ASSERT(s_WGL.OpenGL32DLL, "Failed to load openGL32.dll");
		QK_CORE_ASSERTEX(gladLoadGLLoader((GLADloadproc)GetProcAddressWGL), "Failed to initialize Glad!");
		FreeModule(s_WGL.OpenGL32DLL);

		QK_CORE_ASSERTEX(wglDeleteContext(tempGLContext),                "Failed to delete context!");
		QK_CORE_ASSERTEX(ReleaseDC(tempWindowHandle, tempDeviceContext), "Failed to release DC!"    );
		QK_CORE_ASSERTEX(DestroyWindow(tempWindowHandle),                "Failed to destroy window!");
	}

	OpenGLContext::OpenGLContext(Window& window){
		WindowsWindow* nativeWndObj = (WindowsWindow*)window.GetNativeWindowObject();
		m_DeviceContext = GetDC((HWND)nativeWndObj->GetNativeHandle());
		QK_CORE_ASSERT(m_DeviceContext, "Windows failed to provide a device context!");

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
			s_WGL.ChoosePixelFormatARB(m_DeviceContext, pixelAttribs, NULL, 1, &pixelFormat, &numPixelFormat),
			"Failed to choose pixel format!"
		);

		PIXELFORMATDESCRIPTOR PFD;
		QK_CORE_ASSERTEX(
			DescribePixelFormat(m_DeviceContext, pixelFormat, sizeof(PFD), &PFD),
			"Failed to describe pixel format!"
		);
		QK_CORE_ASSERTEX(SetPixelFormat(m_DeviceContext, pixelFormat, &PFD), "Unable to set pixel format!");

		const int attribs[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
			WGL_CONTEXT_MINOR_VERSION_ARB, 6,
			WGL_CONTEXT_LAYER_PLANE_ARB,   0, // main plane
			WGL_CONTEXT_FLAGS_ARB,         WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB | WGL_CONTEXT_DEBUG_BIT_ARB,
			WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0
		};

		m_GLContext = s_WGL.CreateContextAttribsARB(m_DeviceContext, NULL, attribs);
		QK_CORE_ASSERT(m_GLContext, "Could not create wgl context!");
		QK_CORE_ASSERTEX(wglMakeCurrent(m_DeviceContext, m_GLContext), "Failed to make GL context current!");
	}

	void OpenGLContext::DestroyContext(Window& window) {
		QK_CORE_ASSERTEX(wglDeleteContext(m_GLContext), "Failed to delete context!");
		QK_CORE_ASSERTEX(ReleaseDC((HWND)window.GetNativeHandle(), m_DeviceContext), "Failed to release DC!");
	}

#endif // QK_PLATFORM_WINDOWS

}

