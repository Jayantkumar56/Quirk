

#pragma once

#include <bitset>

// defined the three key states
#define	QK_KEY_STATE_START		1
#define QK_KEY_PRESSED			1
#define QK_KEY_REPEAT			2
#define QK_KEY_RELEASED			3
#define QK_KEY_STATE_END		3


namespace Quirk {

	class InputData {
		friend class Input;

	public:
		struct MouseInfo {
			float PreviousPosX, PreviousPosY;
			float CurrentPosX, CurrentPosY;
		};

	private:
		static std::bitset<256> KeyPressed;
		static std::bitset<256> KeyRepeat;

		static MouseInfo MouseData;
	};	

	class Input {
		friend class Window;

	public:
		static inline bool IsKeyPressed(int keyCode) { return InputData::KeyPressed[keyCode]; }
		static inline bool IsKeyReleased(int keyCode) { return InputData::KeyPressed[keyCode] == 0; }

		static inline float MouseCurrentX() { return InputData::MouseData.CurrentPosX; }
		static inline float MouseCurrentY() { return InputData::MouseData.CurrentPosY; }
		static inline float MousePreviousX() { return InputData::MouseData.CurrentPosX; }
		static inline float MousePreviousY() { return InputData::MouseData.CurrentPosY; }

	protected:
		static void UpdateKey(int keyCode, int keyState);
		static void UpdateMousePos(float PosX, float PosY);
	};

}
