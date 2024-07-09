

#pragma once

#include "Core/Input/Events.h"


namespace Quirk {

	class KeyPressedEvent : Event {
	public:
		KeyPressedEvent(int keyCode) : m_KeyCode(keyCode) {}
		inline int GetKeyCode() const { return m_KeyCode; }

		CATEGORY_AND_TYPE(EventCategory::InputEvent | EventCategory::KeyboardEvent, EventType::KeyPressedEvent);

	private:
		uint16_t m_KeyCode;
	};

	class KeyRepeatEvent : Event {
	public:
		KeyRepeatEvent(int keyCode) : m_KeyCode(keyCode) {}
		inline int GetKeyCode() const { return m_KeyCode; }

		CATEGORY_AND_TYPE(EventCategory::InputEvent | EventCategory::KeyboardEvent, EventType::KeyRepeatEvent);

	private:
		uint16_t m_KeyCode;
	};

	class KeyReleaseEvent : Event {
	public:
		KeyReleaseEvent(int keyCode) : m_KeyCode(keyCode) {}
		inline int GetKeyCode() const { return m_KeyCode; }

		CATEGORY_AND_TYPE(EventCategory::InputEvent | EventCategory::KeyboardEvent, EventType::KeyPressedEvent);

	private:
		uint16_t m_KeyCode;
	};

}

