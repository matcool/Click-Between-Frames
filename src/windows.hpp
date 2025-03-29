#pragma once

#include <Geode/Geode.hpp>

#ifdef GEODE_IS_WINDOWS

struct __attribute__((packed)) LinuxInputEvent {
	LARGE_INTEGER time;
	USHORT type;
	USHORT code;
	int value;
};

extern HANDLE hSharedMem;
extern HANDLE hMutex;
extern LPVOID pBuf;


/*
notify the player if theres an issue with input on Linux
*/
class $modify(CreatorLayer) {
	bool init() {
		if (!CreatorLayer::init()) return false;

		DWORD waitResult = WaitForSingleObject(hMutex, 5);
		if (waitResult == WAIT_OBJECT_0) {
			if (static_cast<LinuxInputEvent*>(pBuf)[0].type == 3 && !softToggle.load()) {
				log::error("Linux input failed");
				FLAlertLayer* popup = FLAlertLayer::create(
					"CBF Linux", 
					"Failed to read input devices.\nOn most distributions, this can be resolved with the following command: <cr>sudo usermod -aG input $USER</c> (reboot afterward; this will make your system slightly less secure).\nIf the issue persists, please contact the mod developer.", 
					"OK"
				);
				popup->m_scene = this;
				popup->show();
			}
			ReleaseMutex(hMutex);
		}
		else if (waitResult == WAIT_TIMEOUT) {
			log::error("Mutex stalling");
		}
		else {
			// log::error("CreatorLayer WaitForSingleObject failed: {}", GetLastError());
		}
		return true;
	} 
};

#endif