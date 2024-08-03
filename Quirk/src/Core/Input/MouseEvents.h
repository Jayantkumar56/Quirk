

#pragma once

#include "Events.h"

namespace Quirk {

	class MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(float xPrevPos, float yPrevPos, float xPos, float yPos) : 
			m_PrevPosX(xPrevPos), m_PrevPosY(yPrevPos),
			m_PosX(xPos), m_PosY(yPos) 
		{
		}

		inline float GetDeltaX() const { return m_PosX - m_PrevPosX; }
		inline float GetDeltaY() const { return m_PosY - m_PrevPosY; }
		inline float GetPosX() const { return m_PosX; }
		inline float GetPosY() const { return m_PosY; }

		CATEGORY_AND_TYPE(EventCategory::InputEvent | EventCategory::MouseEvent, EventType::MouseMovedEvent);

	private:
		float m_PrevPosX, m_PrevPosY;
		float m_PosX, m_PosY;
	};

	class MouseButtonDblClickEvent : public Event {
	public:
		MouseButtonDblClickEvent(int button, float xPos, float yPos) :
			m_Button(button),
			m_PosX(xPos), m_PosY(yPos)
		{}

		inline int GetButton() const { return m_Button; }
		inline float GetPosX() const { return m_PosX; }
		inline float GetPosY() const { return m_PosY; }

		CATEGORY_AND_TYPE(EventCategory::InputEvent | EventCategory::MouseEvent, EventType::MouseButtonDblClickEvent);

	private:
		int m_Button;
		float m_PosX, m_PosY;
	};

	class MouseButtonPressedEvent : public Event {
	public:
		MouseButtonPressedEvent(int button, float xPos, float yPos) :
			m_Button(button),
			m_PosX(xPos), m_PosY(yPos)
		{}

		inline int GetButton() const { return m_Button; }
		inline float GetPosX() const { return m_PosX; }
		inline float GetPosY() const { return m_PosY; }

		CATEGORY_AND_TYPE(EventCategory::InputEvent | EventCategory::MouseEvent, EventType::MouseButtonPressedEvent);

	private:
		int m_Button;
		float m_PosX, m_PosY;
	};

	class MouseButtonRepeatEvent : public Event {
	public:
		MouseButtonRepeatEvent(int button, float xPos, float yPos) :
			m_Button(button),
			m_PosX(xPos), m_PosY(yPos)
		{}

		inline int GetButton() const { return m_Button; }
		inline float GetPosX() const { return m_PosX; }
		inline float GetPosY() const { return m_PosY; }

		CATEGORY_AND_TYPE(EventCategory::InputEvent | EventCategory::MouseEvent, EventType::MouseButtonRepeatEvent);

	private:
		int m_Button;
		float m_PosX, m_PosY;
	};

	class MouseButtonReleasedEvent : public Event {
	public:
		MouseButtonReleasedEvent(int button, float xPos, float yPos) :
			m_Button(button),
			m_PosX(xPos), m_PosY(yPos)
		{}

		inline int GetButton() const { return m_Button; }
		inline float GetPosX() const { return m_PosX; }
		inline float GetPosY() const { return m_PosY; }

		CATEGORY_AND_TYPE(EventCategory::InputEvent | EventCategory::MouseEvent, EventType::MouseButtonReleasedEvent);

	private:
		int m_Button;
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

