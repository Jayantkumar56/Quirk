

#pragma once

#include <utility>
#include "Core/core.h"

namespace Quirk {

	enum class EventType {
		MouseMovedEvent, MouseButtonDblClickEvent,
		MouseScrolledEvent, MouseScrolledHEvent,
		MouseButtonPressedEvent, MouseButtonReleasedEvent, 
		KeyPressedEvent, KeyReleasedEvent, KeyRepeatEvent
	};

	enum EventCategory : uint16_t{
		None				= 0,
		ApplicationEvent	= BIT(0),
		InputEvent			= BIT(1),
		MouseEvent			= BIT(2),
		KeyboardEvent		= BIT(3)
	};

#define CATEGORY_AND_TYPE(category, type)	virtual inline uint16_t GetEventCategory() override { return category; }\
											virtual inline EventType GetEventType() override { return type; }

	class Event {
	public:
		virtual inline uint16_t GetEventCategory() = 0;
		virtual inline EventType GetEventType() = 0;
		inline bool IsHandled() const { return m_Handeled; }

		inline bool IsCategory(EventCategory category) { return GetEventCategory() & category; }

	private:
		bool m_Handeled = false;
	};

}

