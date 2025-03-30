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

extern bool linuxNative;

inline LARGE_INTEGER largeFromTimestamp(TimestampType t) {
	LARGE_INTEGER res;
	res.QuadPart = t;
	return res;
}

inline TimestampType timestampFromLarge(LARGE_INTEGER l) {
	return l.QuadPart;
}

#endif