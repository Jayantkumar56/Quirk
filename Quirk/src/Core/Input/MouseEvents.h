

#pragma once

#include "Events.h"

namespace Quirk {

	class MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(float xPos, float yPos) : m_PosX(xPos), m_PosY(yPos) {}

		inline float GetPosX() const { return m_PosX; }
		inline float GetPosY() const { return m_PosY; }

		CATEGORY_AND_TYPE(EventCategory::InputEvent | EventCategory::MouseEvent, EventType::MouseMovedEvent);

	private:
		float m_PosX, m_PosY;
	};

	class MouseButtonDblClickEvent : public Event {
	public:
		MouseButtonDblClickEvent(uint8_t button, float xPos, float yPos) :
			m_Button(button),
			m_PosX(xPos), m_PosY(yPos)
		{}

		inline uint8_t GetButton() const { return m_Button; }
		CATEGORY_AND_TYPE(EventCategory::InputEvent | EventCategory::MouseEvent, EventType::MouseButtonDblClickEvent);

	private:
		uint8_t m_Button;
		float m_PosX, m_PosY;
	};

	class MouseButtonPressedEvent : public Event {
	public:
		MouseButtonPressedEvent(uint8_t button, float xPos, float yPos) :
			m_Button(button),
			m_PosX(xPos), m_PosY(yPos)
		{}

		inline uint8_t GetButton() const { return m_Button; }
		CATEGORY_AND_TYPE(EventCategory::InputEvent | EventCategory::MouseEvent, EventType::MouseButtonPressedEvent);

	private:
		uint8_t m_Button;
		float m_PosX, m_PosY;
	};

	class MouseButtonReleasedEvent : public Event {
	public:
		MouseButtonReleasedEvent(uint8_t button, float xPos, float yPos) :
			m_Button(button),
			m_PosX(xPos), m_PosY(yPos)
		{}

		inline uint8_t GetButton() const { return m_Button; }
		CATEGORY_AND_TYPE(EventCategory::InputEvent | EventCategory::MouseEvent, EventType::MouseButtonReleasedEvent);

	private:
		uint8_t m_Button;
		float m_PosX, m_PosY;
	};

	class MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(float offset) : m_Offset(offset) {}
		inline float GetOffset() const { return m_Offset; }

		CATEGORY_AND_TYPE(EventCategory::InputEvent | EventCategory::MouseEvent, EventType::MouseScrolledEvent);

	private:
		float m_Offset;
	};

	class MouseScrolledHEvent : public Event {
	public:
		MouseScrolledHEvent(float offset) : m_Offset(offset) {}
		inline float GetOffset() const { return m_Offset; }

		CATEGORY_AND_TYPE(EventCategory::InputEvent | EventCategory::MouseEvent, EventType::MouseScrolledHEvent);

	private:
		float m_Offset;
	};

}

