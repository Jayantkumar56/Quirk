

#include "Qkpch.h"

#include "Events.h"
#include "Input.h"

namespace Quirk {

	// Initializing static variables of event dispatcher
	Event* EventDispatcher::m_Event = nullptr;
	EventType EventDispatcher::m_EventType = EventType::None;

	// Initializing static variables of input data
	std::bitset<256> InputData::KeyPressed;
	std::bitset<256> InputData::KeyRepeat;

	InputData::MouseInfo InputData::MouseData;

	void Input::UpdateKey(int keyCode, int keyState) {
		QK_CORE_ASSERT(
			QK_KEY_STATE_START <= keyState && keyState <= QK_KEY_STATE_END,
			"Invalid KeyState provided!"
		);

		switch (keyState) {
			case QK_KEY_PRESSED: {
				InputData::KeyPressed.set(keyCode);
				break;
			}

			case QK_KEY_REPEAT: {
				break;
			}

			case QK_KEY_RELEASED: {
				InputData::KeyPressed.reset(keyCode);
				InputData::KeyRepeat.reset(keyCode);
				break;
			}
		}
	}

	void Input::UpdateMousePos(float PosX, float PosY) {
		InputData::MouseData.PreviousPosX = InputData::MouseData.CurrentPosX;
		InputData::MouseData.PreviousPosY = InputData::MouseData.CurrentPosY;

		InputData::MouseData.CurrentPosX = PosX;
		InputData::MouseData.CurrentPosY = PosY;
	}

}

