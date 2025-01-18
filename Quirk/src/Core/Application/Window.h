
#pragma once

#if QK_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif // QK_PLATFORM_WINDOWS

#include "Core/Core.h"
#include "Core/Renderer/RendererAPI.h"
#include "Core/Renderer/GraphicalContext.h"

namespace Quirk {

	struct WindowSpecification {
		std::wstring Title = L"Untitled";
		uint16_t	 Width; 
		uint16_t	 Height;

		// signifies position of window frame or position of client area when having custom titlebar
		int32_t	PosX  = 0;
		int32_t	PosY  = 0;
		bool	VSyncOn;
		bool	Maximized;
	};

	class Window {
	public:
		Window(const WindowSpecification& spec) : 
			m_Title	   (spec.Title  ),
			m_Width	   (spec.Width  ),
			m_Height   (spec.Height ),
			m_PosX	   (spec.PosX   ),
			m_PosY	   (spec.PosX   ),
			m_VSyncOn  (spec.VSyncOn),
			m_Maximized(spec.Maximized),
			m_Window   (spec, this    )
		{
		}

		~Window() {}

		void OnUpdate() { m_Window.OnUpdate(); }

		// Native Window related Calls	 ********************
		
		inline void* GetNativeHandle()	    { return m_Window.GetNativeHandle(); }
		inline bool  IsCursorLocked() const { return m_Window.IsCursorLocked();	 }
		inline bool  TrackingCursor() const { return m_Window.TrackingCursor();	 }
		inline void  LockCursor()		    { m_Window.LockCursor();			 }
		inline void  UnlockCursor()		    { m_Window.UnlockCursor();			 }

		// Native Window related Calls	 ********************

		// Getters for local member varialbes ***************
		
		inline const void* GetNativeWindowObject() { return (void*)&m_Window; }
		inline std::wstring GetTitle()			   { return m_Title;   }
		inline int32_t		GetPosX()		 const { return m_PosX;    }
		inline int32_t		GetPosY()		 const { return m_PosY;    }
		inline uint16_t		GetWidth()		 const { return m_Width;   }
		inline uint16_t		GetHeight()		 const { return m_Height;  }
		inline bool			IsVSyncOn()		 const { return m_VSyncOn; }
		inline float		GetAspectRatio() const { return static_cast<float>(m_Width) / static_cast<float>(m_Height); }

		// Getters for local member varialbes ***************

	private:
		std::wstring m_Title;
		uint16_t	 m_Width;
		uint16_t	 m_Height;

		// position of the client area
		int32_t	m_PosX;						
		int32_t	m_PosY;						
		bool	m_VSyncOn;
		bool	m_Maximized;

		// made native window-object friend, 
		// so the data could be modified right from the native object
#if QK_PLATFORM_WINDOWS
		friend class WindowsWindow;
		WindowsWindow m_Window;
#endif // QK_PLATFORM_WINDOWS
	};

}
