

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
											static inline EventType GetStaticEventType() { return type; }\
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
		static inline void HandleEvent(Event& event) { 
			m_Event = &event;
			m_EventType = m_Event->GetEventType();
		}

		template<typename T>
		static bool Dispatch(std::function<bool(T&)> fun) {
			if (T::GetStaticEventType() == m_EventType) {
				m_Event->m_Handeled = fun(*(T*)m_Event);
				return true;
			}

			return false;
		}

		static inline EventType GetCurrentEventType() { return m_EventType; }

	private:
		static Event* m_Event;
		static EventType m_EventType;
	};


}

