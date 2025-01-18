

#pragma once

#include <utility>
#include<functional>
#include "Core/Core.h"

namespace Quirk {

	enum class EventType : uint16_t{
		None = 0,
		WindowCloseEvent,				WindowResizeEvent,				WindowMoveEvent,
		WindowMinimizeEvent,			WindowMaximizeEvent,			WindowToggledFocusEvent,
		MouseEnteredEvent,				MouseExitEvent,					MouseMovedEvent,
		MouseScrolledEvent,				MouseScrolledHEvent,			MouseButtonRepeatEvent,
		MouseButtonPressedEvent,		MouseButtonReleasedEvent,		MouseButtonDblClickEvent,	
		KeyPressedEvent,				KeyReleasedEvent,				KeyRepeatEvent
	};

	enum EventCategory : uint16_t{
		None				= 0,
		WindowEvent			= BIT(0),
		InputEvent			= BIT(1),
		MouseEvent			= BIT(2),
		KeyboardEvent		= BIT(3)
	};

#define CATEGORY_AND_TYPE(category, type)	virtual inline uint16_t GetEventCategory() override { return category; }\
											static inline  EventType GetStaticEventType() { return type; }\
											virtual inline EventType GetEventType() override { return type; }

	class Event {
		friend class EventDispatcher;

	public:
		virtual inline uint16_t GetEventCategory() = 0;
		virtual inline EventType GetEventType() = 0;
		inline bool IsHandled() const { return m_Handeled; }

		inline bool IsCategory(EventCategory category) { return GetEventCategory() & category; }

	private:
		bool m_Handeled = false;
	};

	class EventDispatcher {
	public:
		static inline void RegisterEventCallback(std::function<void(Event&)> callback) {
			m_EventCallbackFn = callback;
		}

		// should be called to propagate an event
		static inline void DispatchEvent(Event& event) {
			m_Event = &event;
			m_EventCallbackFn(event);
		}

		// to handle event of certain type
		template<typename T>
		static inline bool HandleEvent(std::function<bool(T&)> fun) {
			if (T::GetStaticEventType() == m_Event->GetEventType()) {
				m_Event->m_Handeled = fun(*(T*)m_Event);
				return true;
			}

			return false;
		}

	private:
		// (for memory allocation) m_Event, m_EventCallbackFn are declared in Input.cpp
		inline static Event* m_Event = nullptr;
		inline static std::function<void(Event&)> m_EventCallbackFn = nullptr;
	};


}

