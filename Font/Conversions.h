#ifndef CONVERSIONS_H
#define CONVERSIONS_H

#include <utf8.h>
#include <string>
#include "Rect.h"

#include <codecvt>
#include <string>
#include <fstream>
#include <vector>
#include <dirent.h>
#include <stdio.h>

#include <time.h>
#include <windows.h>

#include "Utils.h"

// TODO: we should not be relying on SDL here, we need more flexibility!
inline std::string getUserDataPath() {
    // TODO: this getUser shit and etc.. should be done ONCE!
    const char* ORG_NAME = "53CR37 PR0J3C7";
    const char* APP_NAME = "Alpha 0.0.1";

    char* path = SDL_GetPrefPath(ORG_NAME, APP_NAME);
    std::string result(path);
    SDL_free(path);
    return result;
}


inline std::string getCharactersPath() {
    // SEP on windwos. '//' on unix
    std::string path = getUserDataPath() + SEP + "Characters";
    // Windows-only function
    CreateDirectory(path.c_str(), NULL);
    return path;
}

inline std::string getServersPath() {
    // SEP on windwos. '//' on unix
    std::string path = getUserDataPath() + SEP + "Servers";
    // Windows-only function
    CreateDirectory(path.c_str(), NULL);
    return path;
}

inline bool pointInRect(float x, float y, const Rect& rect) {
    return x >= rect.x && x < rect.x + rect.w && y >= rect.y && y < rect.y + rect.h;
}

inline double ptToPx(double pt, double dpi) {
    return pt / 72.0 * dpi;
}

inline double pxToPt(double px, double dpi) {
    return px * 72.0 / dpi;
}


// Rounds up the value to the next power of two, i.e 63 becomes 64, 65 becomes 128
// https://jameshfisher.com/2018/03/30/round-up-power-2/
inline uint64_t nextPow2(uint64_t x) {
    return x == 1 ? 1 : 1 << (64 - __builtin_clzl(x - 1));
}

template<typename T>
inline T quantize(T value, T length) {
    // TODO: is this better? i mean, if T is int?
	//     return (int)((value / (float)length)) * length;
	return (int)((value / length)) * length;
}

// why not use std::swap?
template<typename T>
inline void swapValues(T& a, T& b) {
    T t = a;
    a = b;
    b = t;
}


#endif // CONVERSIONS_H
