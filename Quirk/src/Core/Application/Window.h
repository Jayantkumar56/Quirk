
#pragma once

#if QK_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif // QK_PLATFORM_WINDOWS

#include "Core/Core.h"
#include "Core/Renderer/RendererAPI.h"
#include "Core/Renderer/GraphicalContext.h"

namespace Quirk {

	struct WindowSpecification {
		std::string Title;
		uint16_t	 Width; 
		uint16_t	 Height;

		// signifies position of window frame or position of client area when having custom titlebar
		int32_t	PosX  = 0;
		int32_t	PosY  = 0;
		bool	VSyncOn;
		bool	Maximized;
		bool	CustomTitleBar = false;
	};

	class Window {
	private:
		enum StateFlags {						// Stored in the m_StateFlags variable as
			Maximized					=  0,			    /*   1 << 0,   */
			CustomTitleBarEnabled		=  1,			    /*   1 << 1,   */
			MoveWithCursor				=  2,			    /*   1 << 2,   */
			CursorOverMinimiseButton	=  3,			    /*   1 << 3,   */
			CursorOverMaximiseButton	=  4,			    /*   1 << 4,   */
			CursorOverCloseButton		=  5,			    /*   1 << 5,   */
		};

	public:
		Window(const WindowSpecification& spec) :
				m_Width  (spec.Width ),
				m_Height (spec.Height),
				m_PosX   (spec.PosX  ),
				m_PosY   (spec.PosX  ),
				// CustomTitleBar flag should be set before creating native window object (especially for WindowsWindow)
				m_StateFlags(((int)spec.CustomTitleBar << StateFlags::CustomTitleBarEnabled)),
				m_Window    (spec, this    )
		{
			SetIsMaximised(spec.Maximized);
		}

		// two different objects should not hold ownership of same native window
		Window(Window& other)			 = delete;
		Window& operator=(Window& other) = delete;

		// NOTE: A thread cannot destroy a window created by a different thread,
		//       so Window should not be moved to such different thread bound object
		Window(Window&& other) noexcept : 
				m_Width     (other.m_Width            ),
				m_Height    (other.m_Height           ),
				m_PosX      (other.m_PosX             ),
				m_PosY      (other.m_PosY             ),
				m_StateFlags(other.m_StateFlags       ),
				m_Window    (std::move(other.m_Window))
		{
		}

		// NOTE: A thread cannot destroy a window created by a different thread,
		//       so Window should not be moved to such different thread bound object
		Window& operator=(Window&& other) noexcept {
			m_Width  = other.m_Width;
			m_Height = other.m_Height;
			m_PosX   = other.m_PosX;
			m_PosY   = other.m_PosY;
			m_Window = std::move(other.m_Window);
			return *this;
		}

		~Window() = default;

		void OnUpdate() { m_Window.OnUpdate(); }

		// Native Window related Calls	 ********************
		
		inline void* GetNativeHandle()	    noexcept { return m_Window.GetNativeHandle(); }
		inline bool  IsCursorLocked() const noexcept { return m_Window.IsCursorLocked();  }
		inline bool  TrackingCursor() const noexcept { return m_Window.TrackingCursor();  }
		inline void  LockCursor()		    { m_Window.LockCursor();			 }
		inline void  UnlockCursor()		    { m_Window.UnlockCursor();			 }

		// Native Window related Calls	 ********************

		// Getters for local member varialbes ***************
		
		inline const void* GetNativeWindowObject() noexcept { return (void*)&m_Window; }
		inline int32_t	   GetPosX()		 const noexcept { return m_PosX;    }
		inline int32_t	   GetPosY()		 const noexcept { return m_PosY;    }
		inline uint16_t	   GetWidth()		 const noexcept { return m_Width;   }
		inline uint16_t	   GetHeight()		 const noexcept { return m_Height;  }
		inline float	   GetAspectRatio()  const noexcept { return static_cast<float>(m_Width) / static_cast<float>(m_Height); }

		inline bool IsMaximised()			  const noexcept { return GetStateFlag(StateFlags::Maximized);			   }
		inline bool IsCustomTitleBarEnabled() const noexcept { return GetStateFlag(StateFlags::CustomTitleBarEnabled); }
		inline bool GetCanMoveWithCursor()    const noexcept { return GetStateFlag(StateFlags::MoveWithCursor);        }
		inline bool IsCursorOverCloseButton() const noexcept { return GetStateFlag(StateFlags::CursorOverCloseButton); }
		inline bool IsCursorOverMaximiseButton() const noexcept { return GetStateFlag(StateFlags::CursorOverMaximiseButton); }
		inline bool IsCursorOverMinimiseButton() const noexcept { return GetStateFlag(StateFlags::CursorOverMinimiseButton); }

		// Getters for local member varialbes ***************

		// Setters for local member variables ***************

		inline void SetIsMaximised          (bool toggle)	 noexcept { SetStateFlag(StateFlags::Maximized,                toggle); }
		inline void SetCustomTitleBarEnabled(bool toggle)    noexcept { SetStateFlag(StateFlags::CustomTitleBarEnabled,    toggle); }
		inline void SetCanMoveWithCursor    (bool toggle)    noexcept { SetStateFlag(StateFlags::MoveWithCursor,           toggle); }
		inline void SetCursorOverCloseButton(bool toggle)    noexcept { SetStateFlag(StateFlags::CursorOverCloseButton,    toggle); }
		inline void SetCursorOverMaximiseButton(bool toggle) noexcept { SetStateFlag(StateFlags::CursorOverMaximiseButton, toggle); }
		inline void SetCursorOverMinimiseButton(bool toggle) noexcept { SetStateFlag(StateFlags::CursorOverMinimiseButton, toggle); }

		// Setters for local member variables ***************

	private:
		inline void SetStateFlag(StateFlags state, bool toggle) noexcept {
			// first clearing the state bit then setting the value according to toggle 
			m_StateFlags = (m_StateFlags & ~(1 << state)) | ((int)toggle << state);
		}
		inline bool GetStateFlag(StateFlags state) const noexcept { return (bool)(m_StateFlags & (1 << state)); }

	private:
		uint16_t	 m_Width;
		uint16_t	 m_Height;

		// position of the client area
		int32_t	m_PosX;						
		int32_t	m_PosY;

		// stores all the states flags and boolean properties of the window
		int m_StateFlags;

		// made native window-object friend, 
		// so the data could be modified right from the native object
#if QK_PLATFORM_WINDOWS
		friend class WindowsWindow;
		WindowsWindow m_Window;
#endif // QK_PLATFORM_WINDOWS
	};

}
