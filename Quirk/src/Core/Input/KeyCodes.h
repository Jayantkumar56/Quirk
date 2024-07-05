

#pragma once

#ifdef QK_PLATFORM_WINDOWS


#define QK_Key_LeftMouseBtn					0x01  // Left mouse button
#define QK_Key_RightMouseBtn				0x02  // Right mouse button
#define QK_Key_CtrlBrkPrcs					0x03  // Control-break processing
#define QK_Key_MidMouseBtn					0x04  // Middle mouse button
												 	 
#define QK_Key_ThumbForward					0x05  // Thumb button back on mouse aka X1
#define QK_Key_ThumbBack					0x06  // Thumb button forward on mouse aka X2
												 	 
//0x07 : reserved								 	 
												 	 
#define QK_Key_BackSpace					0x08  // Backspace key
#define QK_Key_Tab							0x09  // Tab key
												 	 
//0x0A - 0x0B : reserved						 	 
												 	 
#define QK_Key_Clear						0x0C  // Clear key
#define QK_Key_Enter						0x0D  // Enter or Return key
												 	 
//0x0E - 0x0F : unassigned						 	 
												 	 
#define QK_Key_Shift						0x10  // Shift key
#define QK_Key_Control						0x11  // Ctrl key
#define QK_Key_Alt							0x12  // Alt key
#define QK_Key_Pause						0x13  // Pause key
#define QK_Key_CapsLock						0x14  // Caps lock key
												 	 
#define QK_Key_Kana							0x15  // Kana input mode
#define QK_Key_Hangeul						0x15  // Hangeul input mode
#define QK_Key_Hangul						0x15  // Hangul input mode
												 	 
//0x16 : unassigned								 	 
												 	 
#define QK_Key_Junju						0x17  // Junja input method
#define QK_Key_Final						0x18  // Final input method
#define QK_Key_Hanja						0x19  // Hanja input method
#define QK_Key_Kanji						0x19  // Kanji input method
												 	 
//0x1A : unassigned								 	 
												 	 
#define QK_Key_Escape						0x1B  // Esc key
												 	 
#define QK_Key_Convert						0x1C  // IME convert
#define QK_Key_NonConvert					0x1D  // IME Non convert
#define QK_Key_Accept						0x1E  // IME accept
#define QK_Key_ModeChange					0x1F  // IME mode change
												 	 
#define QK_Key_Space						0x20  // Space bar
#define QK_Key_PageUp						0x21  // Page up key
#define QK_Key_PageDown						0x22  // Page down key
#define QK_Key_End							0x23  // End key
#define QK_Key_Home							0x24  // Home key
#define QK_Key_LeftArrow					0x25  // Left arrow key
#define QK_Key_UpArrow						0x26  // Up arrow key
#define QK_Key_RightArrow					0x27  // Right arrow key
#define QK_Key_DownArrow					0x28  // Down arrow key
#define QK_Key_Select						0x29  // Select key
#define QK_Key_Print						0x2A  // Print key
#define QK_Key_Execute						0x2B  // Execute key
#define QK_Key_PrintScreen					0x2C  // Print screen key
#define QK_Key_Insert						0x2D  // Insert key
#define QK_Key_Delete						0x2E  // Delete key
#define QK_Key_Help							0x2F  // Help key
												 	 
#define QK_Key_0							0x30  // Top row 0 key (Matches '0')
#define QK_Key_1							0x31  // Top row 1 key (Matches '1')
#define QK_Key_2							0x32  // Top row 2 key (Matches '2')
#define QK_Key_3							0x33  // Top row 3 key (Matches '3')
#define QK_Key_4							0x34  // Top row 4 key (Matches '4')
#define QK_Key_5							0x35  // Top row 5 key (Matches '5')
#define QK_Key_6							0x36  // Top row 6 key (Matches '6')
#define QK_Key_7							0x37  // Top row 7 key (Matches '7')
#define QK_Key_8							0x38  // Top row 8 key (Matches '8')
#define QK_Key_9							0x39  // Top row 9 key (Matches '9')
												 	 
//0x3A - 0x40 : unassigned						 	 
												 	 
#define QK_Key_A							0x41  // A key (Matches 'A')
#define QK_Key_B							0x42  // B key (Matches 'B')
#define QK_Key_C							0x43  // C key (Matches 'C')
#define QK_Key_D							0x44  // D key (Matches 'D')
#define QK_Key_E							0x45  // E key (Matches 'E')
#define QK_Key_F							0x46  // F key (Matches 'F')
#define QK_Key_G							0x47  // G key (Matches 'G')
#define QK_Key_H							0x48  // H key (Matches 'H')
#define QK_Key_I							0x49  // I key (Matches 'I')
#define QK_Key_J							0x4A  // J key (Matches 'J')
#define QK_Key_K							0x4B  // K key (Matches 'K')
#define QK_Key_L							0x4C  // L key (Matches 'L')
#define QK_Key_M							0x4D  // M key (Matches 'M')
#define QK_Key_N							0x4E  // N key (Matches 'N')
#define QK_Key_O							0x4F  // O key (Matches 'O')
#define QK_Key_P							0x50  // P key (Matches 'P')
#define QK_Key_Q							0x51  // Q key (Matches 'Q')
#define QK_Key_R							0x52  // R key (Matches 'R')
#define QK_Key_S							0x53  // S key (Matches 'S')
#define QK_Key_T							0x54  // T key (Matches 'T')
#define QK_Key_U							0x55  // U key (Matches 'U')
#define QK_Key_V							0x56  // V key (Matches 'V')
#define QK_Key_W							0x57  // W key (Matches 'W')
#define QK_Key_X							0x58  // X key (Matches 'X')
#define QK_Key_Y							0x59  // Y key (Matches 'Y')
#define QK_Key_Z							0x5A  // Z key (Matches 'Z')
												 	 
#define QK_Key_LeftWin						0x5B  // Left windows key
#define QK_Key_RightWin						0x5C  // Right windows key
#define QK_Key_Apps							0x5D  // Applications key
												 	 
//0x5E : reserved								 	 
												 	 
#define QK_Key_Sleep						0x5F  // Computer sleep key
												 	 
#define QK_Key_Numpad0						0x60  // Numpad 0
#define QK_Key_Numpad1						0x61  // Numpad 1
#define QK_Key_Numpad2						0x62  // Numpad 2
#define QK_Key_Numpad3						0x63  // Numpad 3
#define QK_Key_Numpad4						0x64  // Numpad 4
#define QK_Key_Numpad5						0x65  // Numpad 5
#define QK_Key_Numpad6						0x66  // Numpad 6
#define QK_Key_Numpad7						0x67  // Numpad 7
#define QK_Key_Numpad8						0x68  // Numpad 8
#define QK_Key_Numpad9						0x69  // Numpad 9
#define QK_Key_Multiply						0x6A  // Multiply key
#define QK_Key_Add							0x6B  // Add key
#define QK_Key_Separator					0x6C  // Separator key
#define QK_Key_Subtract						0x6D  // Subtract key
#define QK_Key_Decimal						0x6E  // Decimal key
#define QK_Key_Divide						0x6F  // Divide key
#define QK_Key_F1							0x70  // F1
#define QK_Key_F2							0x71  // F2
#define QK_Key_F3							0x72  // F3
#define QK_Key_F4							0x73  // F4
#define QK_Key_F5							0x74  // F5
#define QK_Key_F6							0x75  // F6
#define QK_Key_F7							0x76  // F7
#define QK_Key_F8							0x77  // F8
#define QK_Key_F9							0x78  // F9
#define QK_Key_F10							0x79  // F10
#define QK_Key_F11							0x7A  // F11
#define QK_Key_F12							0x7B  // F12
#define QK_Key_F13							0x7C  // F13
#define QK_Key_F14							0x7D  // F14
#define QK_Key_F15							0x7E  // F15
#define QK_Key_F16							0x7F  // F16
#define QK_Key_F17							0x80  // F17
#define QK_Key_F18							0x81  // F18
#define QK_Key_F19							0x82  // F19
#define QK_Key_F20							0x83  // F20
#define QK_Key_F21							0x84  // F21
#define QK_Key_F22							0x85  // F22
#define QK_Key_F23							0x86  // F23
#define QK_Key_F24							0x87  // F24
												 	 
//0x88 - 0x8F : UI navigation					 	 
												 	 
#define QK_Key_NavigationView				0x88  // reserved
#define QK_Key_NavigationMenu				0x89  // reserved
#define QK_Key_NavigationUp					0x8A  // reserved
#define QK_Key_NavigationDown				0x8B  // reserved
#define QK_Key_NavigationLeft				0x8C  // reserved
#define QK_Key_NavigationRight				0x8D  // reserved
#define QK_Key_NavigationAccept				0x8E  // reserved
#define QK_Key_NavigationCancel				0x8F  // reserved
												 	 
#define QK_Key_NumLock						0x90  // Num lock key
#define QK_Key_ScrollLock					0x91  // Scroll lock key
												 	 
#define QK_Key_NumpadEqual					0x92  // Numpad 
												 	 
#define QK_Key_FJ_Jisho						0x92  // Dictionary key
#define QK_Key_FJ_Masshou					0x93  // Unregister word key
#define QK_Key_FJ_Touroku					0x94  // Register word key
#define QK_Key_FJ_Loya						0x95  // Left OYAYUBI key
#define QK_Key_FJ_Roya						0x96  // Right OYAYUBI key
												 	 
//0x97 - 0x9F : unassigned						 	 
												 	 
#define QK_Key_LeftShift					0xA0  // Left shift key
#define QK_Key_RightShift					0xA1  // Right shift key
#define QK_Key_LeftCtrl						0xA2  // Left control key
#define QK_Key_RightCtrl					0xA3  // Right control key
#define QK_Key_LeftMenu						0xA4  // Left menu key
#define QK_Key_RightMenu					0xA5  // Right menu
												 	 
#define QK_Key_BrowserBack					0xA6  // Browser back button
#define QK_Key_BrowserForward				0xA7  // Browser forward button
#define QK_Key_BrowserRefresh				0xA8  // Browser refresh button
#define QK_Key_BrowserStop					0xA9  // Browser stop button
#define QK_Key_BrowserSearch				0xAA  // Browser search button
#define QK_Key_BrowserFavorites				0xAB  // Browser favorites button
#define QK_Key_BrowserHome					0xAC  // Browser home button
												 	 
#define QK_Key_VolumeMute					0xAD  // Volume mute button
#define QK_Key_VolumeDown					0xAE  // Volume down button
#define QK_Key_VolumeUp						0xAF  // Volume up button
#define QK_Key_NextTrack					0xB0  // Next track media button
#define QK_Key_PrevTrack					0xB1  // Previous track media button
#define QK_Key_Stop							0xB2  // Stop media button
#define QK_Key_PlayPause					0xB3  // Play/pause media button
#define QK_Key_Mail							0xB4  // Launch mail button
#define QK_Key_MediaSelect					0xB5  // Launch media select button
#define QK_Key_App1							0xB6  // Launch app 1 button
#define QK_Key_App2							0xB7  // Launch app 2 button
												 	 
//0xB8 - 0xB9 : reserved						 	 
												 	 
#define QK_Key_OEM1							0xBA  // ;: key for US or misc keys for others
#define QK_Key_Plus							0xBB  // Plus key
#define QK_Key_Comma						0xBC  // Comma key
#define QK_Key_Minus						0xBD  // Minus key
#define QK_Key_Period						0xBE  // Period key
#define QK_Key_OEM2							0xBF  // ? for US or misc keys for others
#define QK_Key_OEM3							0xC0  // ~ for US or misc keys for others
												 	 
//0xC1 - 0xC2 : reserved						 	 
												 	 
#define QK_Key_Gamepad_A					0xC3  // Gamepad A button
#define QK_Key_Gamepad_B					0xC4  // Gamepad B button
#define QK_Key_Gamepad_X					0xC5  // Gamepad X button
#define QK_Key_Gamepad_Y					0xC6  // Gamepad Y button
#define QK_Key_GamepadRightBumper			0xC7  // Gamepad right bumper
#define QK_Key_GamepadLeftBumper			0xC8  // Gamepad left bumper
#define QK_Key_GamepadLeftTrigger			0xC9  // Gamepad left trigger
#define QK_Key_GamepadRightTrigger			0xCA  // Gamepad right trigger
#define QK_Key_GamepadDPadUp				0xCB  // Gamepad DPad up
#define QK_Key_GamepadDPadDown				0xCC  // Gamepad DPad down
#define QK_Key_GamepadDPadLeft				0xCD  // Gamepad DPad left
#define QK_Key_GamepadDPadRight				0xCE  // Gamepad DPad right
#define QK_Key_GamepadMenu					0xCF  // Gamepad menu button
#define QK_Key_GamepadView					0xD0  // Gamepad view button
#define QK_Key_GamepadLeftStickBtn			0xD1  // Gamepad left stick button
#define QK_Key_GamepadRightStickBtn			0xD2  // Gamepad right stick button
#define QK_Key_GamepadLeftStickUp			0xD3  // Gamepad left stick up
#define QK_Key_GamepadLeftStickDown			0xD4  // Gamepad left stick down
#define QK_Key_GamepadLeftStickRight		0xD5  // Gamepad left stick right
#define QK_Key_GamepadLeftStickLeft			0xD6  // Gamepad left stick left
#define QK_Key_GamepadRightStickUp			0xD7  // Gamepad right stick up
#define QK_Key_GamepadRightStickDown		0xD8  // Gamepad right stick down
#define QK_Key_GamepadRightStickRight		0xD9  // Gamepad right stick right
#define QK_Key_GamepadRightStickLeft		0xDA  // Gamepad right stick left
												 	 
#define QK_Key_OEM4							0xDB  // [ for US or misc keys for others
#define QK_Key_OEM5							0xDC  // \ for US or misc keys for others
#define QK_Key_OEM6							0xDD  // ] for US or misc keys for others
#define QK_Key_OEM7							0xDE  // ' for US or misc keys for others
#define QK_Key_OEM8							0xDF  // Misc keys for others
												 	 
//0xE0 : reserved								 	 
												 	 
#define QK_Key_OEMAX						0xE1  // AX key on Japanese AX keyboard
#define QK_Key_OEM102						0xE2  // "<>" or "\|" on RT 102-key keyboard
#define QK_Key_ICOHelp						0xE3  // Help key on ICO
#define QK_Key_ICO00						0xE4  // 00 key on ICO
												 	 
#define QK_Key_ProcessKey					0xE5  // Process key input method
#define QK_Key_OEMCLEAR						0xE6  // OEM specific
#define QK_Key_Packet						0xE7  // IDK man try to google it
												 	 
//0xE8 : unassigned								 	 
												 	 
#define QK_Key_OEMReset						0xE9  // OEM reset button
#define QK_Key_OEMJump						0xEA  // OEM jump button
#define QK_Key_OEMPA1						0xEB  // OEM PA1 button
#define QK_Key_OEMPA2						0xEC  // OEM PA2 button
#define QK_Key_OEMPA3						0xED  // OEM PA3 button
#define QK_Key_OEMWSCtrl					0xEE  // OEM WS Control button
#define QK_Key_OEMCusel						0xEF  // OEM CUSEL button
#define QK_Key_OEMAttn						0xF0  // OEM ATTN button
#define QK_Key_OEMFinish					0xF1  // OEM finish button
#define QK_Key_OEMCopy						0xF2  // OEM copy button
#define QK_Key_OEMAuto						0xF3  // OEM auto button
#define QK_Key_OEMEnlw						0xF4  // OEM ENLW
#define QK_Key_OEMBackTab					0xF5  // OEM back tab
												 	 
#define QK_Key_Attn							0xF6  // Attn
#define QK_Key_CrSel						0xF7  // CrSel
#define QK_Key_ExSel						0xF8  // ExSel
#define QK_Key_EraseEOF						0xF9  // Erase EOF key
#define QK_Key_Play							0xFA  // Play key
#define QK_Key_Zoom							0xFB  // Zoom key
#define QK_Key_NoName						0xFC  // No name
#define QK_Key_PA1							0xFD  // PA1 key
#define QK_Key_OEMClear						0xFE  // OEM Clear key

#endif // QK_PLATFORM_WINDOWS

