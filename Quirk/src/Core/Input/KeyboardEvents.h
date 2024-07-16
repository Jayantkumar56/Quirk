

#pragma once

#include "Core/Input/Events.h"


namespace Quirk {

	class KeyPressedEvent : public Event {
	public:
		KeyPressedEvent(int keyCode) : m_KeyCode(keyCode) {}
		inline int GetKeyCode() const { return m_KeyCode; }

		CATEGORY_AND_TYPE(EventCategory::InputEvent | EventCategory::KeyboardEvent, EventType::KeyPressedEvent);

	private:
		int m_KeyCode;
	};

	class KeyRepeatEvent : public Event {
	public:
		KeyRepeatEvent(int keyCode) : m_KeyCode(keyCode) {}
		inline int GetKeyCode() const { return m_KeyCode; }

		CATEGORY_AND_TYPE(EventCategory::InputEvent | EventCategory::KeyboardEvent, EventType::KeyRepeatEvent);

	private:
		int m_KeyCode;
	};

	class KeyReleaseEvent : public Event {
	public:
		KeyReleaseEvent(int keyCode) : m_KeyCode(keyCode) {}
		inline int GetKeyCode() const { return m_KeyCode; }

		CATEGORY_AND_TYPE(EventCategory::InputEvent | EventCategory::KeyboardEvent, EventType::KeyPressedEvent);

	private:
		int m_KeyCode;
	};

}

