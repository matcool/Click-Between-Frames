#pragma once

#include <queue>
#include <mutex>

#include <Geode/Geode.hpp>
#include <geode.custom-keybinds/include/Keybinds.hpp>

using namespace geode::prelude;

#include "platform.hpp"

#if defined(GEODE_IS_WINDOWS)
#include "windows.hpp"
#elif defined(GEODE_IS_ANDROID)
#include "android.hpp"
#endif

enum GameAction : int {
	p1Jump = 0,
	p1Left = 1,
	p1Right = 2,
	p2Jump = 3,
	p2Left = 4,
	p2Right = 5
};

enum State : bool {
	Release = 0,
	Press = 1
};

struct InputEvent {
	TimestampType time;
	PlayerButton inputType;
	bool inputState;
	bool isPlayer1;
};

struct Step {
	InputEvent input;
	double deltaFactor;
	bool endStep;
};

extern std::queue<struct InputEvent> inputQueue;
extern std::queue<struct InputEvent> inputQueueCopy;

extern std::array<std::unordered_set<size_t>, 6> inputBinds;
extern std::unordered_set<uint16_t> heldInputs;

extern std::mutex inputQueueLock;
extern std::mutex keybindsLock;

extern std::atomic<bool> enableRightClick;
extern std::atomic<bool> softToggle;

extern bool threadPriority;

constexpr size_t BUFFER_SIZE = 20;

void linuxCheckInputs();
void inputThread();