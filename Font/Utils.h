#ifndef UTILS_H
#define UTILS_H

#include <utf8.h>
#include <string>

#include <codecvt>
#include <string>
#include <fstream>
#include <vector>
#include <dirent.h>
#include <stdio.h>

#include <time.h>
#include <windows.h>
#include <glm/glm.hpp>

#include <sys/stat.h> // For stat in Filesystem::exists()

// http://gcc.gnu.org/onlinedocs/cpp/Predefined-Macros.html
// https://stackoverflow.com/a/5920028

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
const char SEP = '\\';
#elif __APPLE__ || __linux__ ||
const char SEP = '/';
#endif

#include "Timing.h"

inline void printAverageTPS() {
    static int simulatedTicks = 0;
    simulatedTicks++;

    static Timing::Time startTime = Timing::SystemClock().now();
    Timing::Time elapsed = Timing::SystemClock().now() - startTime;
    float sec = elapsed.toSeconds();

    float avg = sec / simulatedTicks;
    printf("Avg time: %f\n", avg);
}

namespace Encode {

// https://renenyffenegger.ch/notes/development/Base64/Encoding-and-decoding-base-64-with-cpp
// used as referrence. But algorithm is created entirely by me
static const char base64Table[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+-";

/*
    Algorithms uses 3 modes, or rather 'steps'
    First byte is first step, second byte is second step, third byte is third step,
    forth byte starts again at first step.
    At first step, we split a byte (8 bits) into two parts: 6 and 2 bits.
    i.e = 01110101 will split as 011101 and 01. Since they are stored both as 8 bits (index and remainder are uint8_t), they actually look in memory like this:
    '011101 00' and '000000 01'. For the first number (index), we move it 2 bits to the right, since there are '00's there, so it becomes what we want: '00011101' or, simply,
    011101. Since this index is 6 bits long, it can represent 64 different numbers, so we can use it as index into the table directly (in other words, mapping 6-bit values to characters).
    Lower 2 bits that we got (000000 01) is a remainder and will become UPPER 2 bits of 6-bit digit, that's why we 'create space' for next 4 bits by shifting it to the left:
    00 01 0000.
    In mode 2, we grab that remainder and fill it's lower 4 bits (which are 0s), with upper 4 bits of next byte. Since they are upper (i.e 1111 0000) we need to shift them 4 bits to the right (so it becomes 0000 1111). 'OR'ing with remainer, we get: 00 01 1111.
    For the new remainer, we put other (lower) 4 bits in it and make 'room' for 2 upcoming bits in the next byte.
    In the 3rd mode, we do the same - grab upper 2 bits of new byte, shift 6 to the right (1100 0000 becomes 0000 0011) and OR it with remainer (which was 00 1111 00), so we get (00 1111 11).
    The 6 bits that are left we just mask to get rid of upper 2 bits (1111 1111 becomes 0011 1111) and that's it.
*/

inline std::string base64(const char* bytes, int size) {
    std::string result;

    int mode = 0;

    uint8_t remainder, index, byte;
    for (int i = 0; i < size; i++) {
        byte = bytes[i];

        if (mode == 0) {
            index = (byte & 0xFC) >> 2;
            remainder = (byte & 0x03) << 4;
            result += base64Table[index];
        } else if (mode == 1) {
            index = remainder | (byte & 0xF0) >> 4;
            remainder = (byte & 0x0F) << 2;
            result += base64Table[index];
        } else if (mode == 2) {
            index = remainder | (byte & 0xC0) >> 6;
            remainder = (byte & 0x3F);
            result += base64Table[index];
            result += base64Table[remainder];
        }

        mode = (mode + 1) % 3;
    }

    return result;
}
}

inline bool stringEndsWith(const std::string& filepath, const std::string& ext) {
    return filepath.substr(filepath.length() - ext.length(), ext.length()) == ext;
}


inline std::wstring utf8to16(const std::string& text) {
    std::wstring buffer;
    utf8::utf8to16(text.begin(), text.end(), std::back_inserter(buffer));
    return buffer;
}

inline std::string utf16to8(const std::wstring& text) {
    std::string buffer;
    utf8::utf16to8(text.begin(), text.end(), std::back_inserter(buffer));
    return buffer;
}

template <typename T>
inline T lerp(const T& from, const T& to, float amount) {
    return from + (to - from) * amount;
}

// Function to lerp over time
// For example, at 60 fps, we can say 'interpolate over 1 second'
// this will result in ~63 frames of interpolation
template <typename T>
inline T lerpTime(const T& from, const T& to, float delta /* sec per tick */, float time /* seconds */) {
    return lerp(from, to, glm::clamp(delta / time, 0.0f, 1.0f));
}


inline float clampLerpTime(const float value, const float min, const float max, const float delta, const float time) {
    if (value < min) {
        return lerpTime(value, min, delta, time);
    } else if (value > max) {
        return lerpTime(value, max, delta, time);
    }
    return value;
}



//https://gist.github.com/itsmrpeck/be41d72e9d4c72d2236de687f6f53974
#define PI 3.14159265359f
#define PI_TIMES_TWO 6.28318530718f
inline float lerpRadians(float a, float b, float lerpFactor) { // Lerps from angle a to b (both between 0.f and PI_TIMES_TWO), taking the shortest path
    float result;
    float diff = b - a;
    if (diff < -PI) {
        // lerp upwards past PI_TIMES_TWO
        b += PI_TIMES_TWO;
        result = lerp(a, b, lerpFactor);
        if (result >= PI_TIMES_TWO) {
            result -= PI_TIMES_TWO;
        }
    } else if (diff > PI) {
        // lerp downwards past 0
        b -= PI_TIMES_TWO;
        result = lerp(a, b, lerpFactor);
        if (result < 0.f) {
            result += PI_TIMES_TWO;
        }
    } else {
        // straight lerp
        result = lerp(a, b, lerpFactor);
    }

    return result;
}



// TODO: https://www.geeksforgeeks.org/print-calendar-for-a-given-year/
// look into it. So, we have to implement something that converts uint32 to date.
// it will be something like:
// calculate number of years (year is exactly 365 days, so its easy)
// of course we need to convert 365 days into nanoseconds and % with it.
// get remainder
// calculate current month of year (given amount of days)
// get remainder
// calculate current day of month
// and calculate time, which is trivial too
// and i would need to use my own timepoint class maybe, idk.
// and it would be cool to integrate it with my timing class (rename Time to TimePoint etc...).
// to try all of this, i would recomment changing Timing::Time to nanosecond resolution
// and then use this time to first calculate current year, then calculate current month
// then day and so on
// or use this: https://howardhinnant.github.io/date/date.html
// BUT, don't actually use it.
inline std::wstring getDateTimeString(uint32_t timepoint) {
    time_t tp = timepoint;
    tm t = *localtime(&tp);

    char buffer[256];

    sprintf(buffer, "%02d.%02d.%d %02d:%02d",
            t.tm_mday,
            t.tm_mon + 1,
            t.tm_year + 1900,
            t.tm_hour,
            t.tm_min);

    return utf8to16(buffer);
}

namespace Filesystem {

// https://stackoverflow.com/a/12774387
inline bool exists(const std::string& name) {
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}

inline void createFolder(const std::string& path) {
    // Windows-only function
    CreateDirectory(path.c_str(), NULL);
}

inline void deleteFile(const std::string& path) {
    remove(path.c_str());
}

inline void getFolderContents(const std::string& path, std::vector<std::string>& files) {
    DIR* dir;
    dirent* ent;
    if ((dir = opendir(path.c_str()))) {
        readdir(dir); // ignore '.'
        readdir(dir); // ignore '..'

        while ((ent = readdir(dir))) {
            files.push_back(std::string(ent->d_name));
        }

        closedir(dir);
    }
}

}

namespace Random {

// https://stackoverflow.com/a/3747462
class PRNG {
public:
    PRNG() {
        m_seed = 0;
    }
    ~PRNG() {}

    void setSeed(uint32_t seed) {
        m_seed = seed;
    }

    uint32_t next() {
        m_seed = (214013 * m_seed + 2531011);
        return (m_seed >> 16) & 0x7FFF;
    }
private:
    uint32_t m_seed;
};

// Circular bit-shift
// Explanation: https://stackoverflow.com/a/776523
inline uint32_t rotr32(uint32_t x, unsigned r) {
    return x >> r | x << (-r & 31);
}

// https://en.wikipedia.org/wiki/Permuted_congruential_generator#Example_code
class PCG {
public:
    PCG() {}
    virtual ~PCG() {}

    void setSeed(uint64_t seed) {
        state = seed + increment;
        next();
    }

    uint32_t next() {
        uint64_t x = state;
        unsigned count = (unsigned)(x >> 59);		// 59 = 64 - 5

        state = x * multiplier + increment;
        x ^= x >> 18;								// 18 = (64 - 27)/2
        return rotr32((uint32_t)(x >> 27), count);	// 27 = 32 - 5
    }
private:
    uint64_t state = 0xcafef00dd15ea5e5u;   // Must be odd
    static uint64_t const multiplier = 6364136223846793005u;
    static uint64_t const increment = 1442695040888963407u; // Or an arbitrary odd constant
};

class PCGFast {
public:
    PCGFast() {}
    virtual ~PCGFast() {}

    void setSeed(uint64_t seed) {
        mcg_state = 2 * seed + 1;
        next();
    }

    uint32_t next() {
        uint64_t x = mcg_state;
        unsigned count = (unsigned)(x >> 61); // 61 = 64 - 3

        mcg_state = x * multiplier;
        x ^= x >> 22;
        return (uint32_t)(x >> (22 + count)); // 22 = 32 - 3 - 7
    }
private:
    uint64_t mcg_state = 0xcafef00dd15ea5e5u;   // Must be odd
    static uint64_t const multiplier = 6364136223846793005u;
};

}










#endif // UTILS_H
