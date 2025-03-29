#include "includes.hpp"

std::queue<struct InputEvent> inputQueue;

std::array<std::unordered_set<size_t>, 6> inputBinds;
std::unordered_set<uint16_t> heldInputs;

std::mutex inputQueueLock;
std::mutex keybindsLock;

std::atomic<bool> enableRightClick;
bool threadPriority;

#ifdef GEODE_IS_WINDOWS
void linuxCheckInputs() {
    DWORD waitResult = WaitForSingleObject(hMutex, 1);
    if (waitResult == WAIT_OBJECT_0) {
        LinuxInputEvent* events = static_cast<LinuxInputEvent*>(pBuf);
        for (int i = 0; i < BUFFER_SIZE; i++) {
			if (events[i].type == 0) break; // if there are no more events

			InputEvent input;
			bool player1 = true;

			USHORT scanCode = events[i].code;
			if (scanCode == 0x3110) { // left click
				input.inputType = PlayerButton::Jump;
			}
			else if (scanCode == 0x3111) { // right click
				if (!enableRightClick.load()) continue;
				input.inputType = PlayerButton::Jump;
				player1 = false;
			}
			else {
				USHORT keyCode = MapVirtualKeyExA(scanCode, MAPVK_VSC_TO_VK, GetKeyboardLayout(0));
				if (inputBinds[p1Jump].contains(keyCode)) input.inputType = PlayerButton::Jump;
				else if (inputBinds[p1Left].contains(keyCode)) input.inputType = PlayerButton::Left;
				else if (inputBinds[p1Right].contains(keyCode)) input.inputType = PlayerButton::Right;
				else {
					player1 = false;
					if (inputBinds[p2Jump].contains(keyCode)) input.inputType = PlayerButton::Jump;
					else if (inputBinds[p2Left].contains(keyCode)) input.inputType = PlayerButton::Left;
					else if (inputBinds[p2Right].contains(keyCode)) input.inputType = PlayerButton::Right;
					else continue;
            	}
			}

            input.inputState = !events[i].value;
			input.time = events[i].time;
            input.isPlayer1 = player1;
                
            inputQueueCopy.emplace(input);
        }
		ZeroMemory(events, sizeof(LinuxInputEvent[BUFFER_SIZE]));
        ReleaseMutex(hMutex);
    }
    else if (waitResult != WAIT_TIMEOUT) {
        log::error("WaitForSingleObject failed: {}", GetLastError());
    }
}
#endif