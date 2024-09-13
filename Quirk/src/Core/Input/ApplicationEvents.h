

#pragma once

#include "Events.h"

namespace Quirk {

	class WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() = default;
		CATEGORY_AND_TYPE(EventCategory::WindowEvent, EventType::WindowCloseEvent);
	};

	class WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(uint16_t width, uint16_t height): m_Width(width), m_Height(height) {}

		inline uint16_t GetWidth() const { return m_Width; }
		inline uint16_t GetHeight() const { return m_Height; }

		CATEGORY_AND_TYPE(EventCategory::WindowEvent, EventType::WindowResizeEvent);

	private:
		uint16_t m_Width, m_Height;
	};

	class WindowMoveEvent : public Event {
	public:
		WindowMoveEvent(uint16_t xPos, uint16_t yPos) : m_PosX(xPos), m_PosY(yPos) {}

		inline uint32_t GetPosX() const { return m_PosX; }
		inline uint32_t GetPosY() const { return m_PosY; }

		CATEGORY_AND_TYPE(EventCategory::WindowEvent, EventType::WindowMoveEvent);

	private:
		uint16_t m_PosX, m_PosY;
	};

	class WindowMinimizeEvent : public Event {
	public:
		WindowMinimizeEvent() = default;
		CATEGORY_AND_TYPE(EventCategory::WindowEvent, EventType::WindowMinimizeEvent);
	};

	class WindowMaximizeEvent : public Event {
	public:
		WindowMaximizeEvent() = default;
		CATEGORY_AND_TYPE(EventCategory::WindowEvent, EventType::WindowMaximizeEvent);
	};

	class WindowToggledFocusEvent : public Event {
	public:
		WindowToggledFocusEvent(bool isFocused) : m_IsFocused(isFocused) {}
		inline bool GetIsFocused() const { return m_IsFocused; }
		CATEGORY_AND_TYPE(EventCategory::WindowEvent, EventType::WindowToggledFocusEvent);

	private:
		bool m_IsFocused;
	};

}
