
#pragma once

#if QK_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif // QK_PLATFORM_WINDOWS

#include "Core/core.h"
#include "Core/Renderer/RendererAPI.h"
#include "Core/Renderer/GraphicalContext.h"

namespace Quirk {

	struct WindowSpecification {
		std::wstring Title = L"Untitled";
		uint16_t	 Width; 
		uint16_t	 Height;
		int32_t		 PosX  = 0;
		int32_t		 PosY  = 0;
		bool		 VSyncOn;
	};

	class Window {
	public:
		Window(const WindowSpecification& spec) : 
			m_Window (spec, this ),
			m_Title	 (spec.Title ),
			m_Width	 (spec.Width ),
			m_Height (spec.Height),
			m_PosX	 (spec.PosX	 ),
			m_PosY	 (spec.PosY	 ),
			m_VSyncOn(spec.VSyncOn)
		{
		}

		~Window() {
			m_Context->DestroyContext(*this);
		}

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

		// temporary function 
		void CreateGraphicalContext(RendererAPI::API api) {
			m_Context = GraphicalContext::Create(api);
			m_Context->Init(*this);
		}

		// Call this only after Rendering API is set
		inline void SetAsCurrentContext() {
			if (m_Context.get() == nullptr) {
				m_Context = GraphicalContext::Create();
				m_Context->Init(*this);
			}

			m_Context->MakeContextCurrent();
		}

		inline void SetVSync(bool toggle) {
			QK_CORE_ASSERT(m_Context.get() != nullptr, "GraphicalContext not created yet!");

			if      (m_VSyncOn && !toggle) { m_Context->SetVSync(0); }
			else if (!m_VSyncOn && toggle) { m_Context->SetVSync(1); }

			m_VSyncOn = toggle;
		}

		inline void SwapBuffers()					 const { m_Context->SwapBuffer(); }
		inline Ref<GraphicalContext> GetGraphicalContext() { return m_Context;		  }

	private:
#if QK_PLATFORM_WINDOWS
		// made native window-object friend, 
		// so the data could be modified right from the native object
		friend class WindowsWindow;
		WindowsWindow m_Window;
#endif // QK_PLATFORM_WINDOWS

		std::wstring m_Title;
		uint16_t	 m_Width;
		uint16_t	 m_Height;
		int32_t		 m_PosX;
		int32_t		 m_PosY;
		bool		 m_VSyncOn;
		Ref<GraphicalContext> m_Context;
	};

}
