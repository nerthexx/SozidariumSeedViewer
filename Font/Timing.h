#ifndef TIMING_H
#define TIMING_H

#include <stdint.h>
#include <windows.h>

namespace Timing {

class Time {
public:
    Time(): m_microseconds(0) {}
    Time(int64_t microseconds): m_microseconds(microseconds) {}
    virtual ~Time() {}

    int64_t toMicroseconds() const {
        return m_microseconds;
    }

    int32_t toMilliseconds() const {
        return m_microseconds / 1000;
    }

    float toSeconds() const {
        return m_microseconds / 1000000.0f;
    }

    // Operators

    bool operator ==(const Time& other) const {
        return m_microseconds == other.m_microseconds;
    }

    bool operator !=(const Time& other) const {
        return m_microseconds != other.m_microseconds;
    }

    bool operator <(const Time& other) const {
        return m_microseconds < other.m_microseconds;
    }

    bool operator >(const Time& other) const {
        return m_microseconds > other.m_microseconds;
    }

    bool operator <=(const Time& other) const {
        return m_microseconds <= other.m_microseconds;
    }

    bool operator >=(const Time& other) const {
        return m_microseconds >= other.m_microseconds;
    }

    Time operator -() const {
        return Time(-m_microseconds);
    }

    Time operator -(const Time& other) const {
        return Time(m_microseconds - other.m_microseconds);
    }

    Time operator +(const Time& other) const {
        return Time(m_microseconds + other.m_microseconds);
    }

    inline float operator /(const Time& other) {
        return toSeconds() / other.toSeconds();
    }

    void reset() {
        m_microseconds = 0;
    }

private:
    int64_t m_microseconds;
};

inline Time operator -=(Time& left, const Time& right) {
    return left = left - right;
}

inline Time operator +=(Time& left, const Time& right) {
    return left = left + right;
}




inline Time Microseconds(int64_t microseconds) {
    return Time(microseconds);
}

inline Time Milliseconds(int32_t milliseconds) {
    return Time(1000 * milliseconds);
}

inline Time Seconds(float seconds) {
    return Time(1000000 * seconds);
}

inline Time Minutes(float minutes) {
    return Time(60000000 * minutes);
}

struct ThreadTimer {
public:
    ThreadTimer() {
        m_handle = CreateWaitableTimer(NULL, TRUE, NULL);
    }

    virtual  ~ThreadTimer() {
        CloseHandle(m_handle);
        m_handle = NULL;
    }

    void wait(const Time& time) {
        ///*
        LARGE_INTEGER dueTime;
        // Note: using anything here other than uint32 (ms) results in timer being innacurate. This may be because OS needs to convert data and loses speed, etc.
        dueTime.QuadPart = -10000 * time.toMilliseconds(); // convert to '100 ns'
        SetWaitableTimer(m_handle, &dueTime, 0, NULL, NULL, FALSE);
        WaitForSingleObject(m_handle, INFINITE);
        //*/


        //timeBeginPeriod(1);
        //::Sleep(time.toMilliseconds());
        //timeEndPeriod(1);
    }
private:
    HANDLE m_handle;
};

// TODO: CHANGE TIMING TO NANOSECONDS!!!! this is really important! For seeds and security
// you would ask "is 64 bits enough for nanoseconds?"
// yes! 64 bits of nanoseconds equal to 584 years of time. Plenty of time.
// Also, make it global ffs
// and use mutexes or something (or dont, since thread dont overwrite freq variable)
// https://softwareengineering.stackexchange.com/a/379539

struct SystemClock {
    SystemClock() {
        QueryPerformanceFrequency(&freq);
    }

    virtual ~SystemClock() {}

    Time now() {
        LARGE_INTEGER currentTime;
        QueryPerformanceCounter(&currentTime);
        return Time(1000000 * currentTime.QuadPart / freq.QuadPart);
    }

    LARGE_INTEGER freq;
};


}
#endif // TIMING_H
