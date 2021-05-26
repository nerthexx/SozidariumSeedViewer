#ifndef MATHCOMMON_H
#define MATHCOMMON_H

namespace ice {

template <typename T>
struct tvec2 {
    T x, y;

    tvec2() {}

    tvec2(T x, T y): x(x), y(y) {}

	// Without this template, function will perform only tvec<T> to tvec<T> operations. (i.e onle int to int, float to float)
	// So, to have tvec2 any type of variable that has = operator, we just need to tempalte this function.
	template <typename U>
    tvec2(const tvec2<U>& value) {
        x = value.x;
        y = value.y;
    }

    tvec2 operator+(const T value) const {
        return { x + value, y + value };
    }

    tvec2 operator+(const tvec2& value) const {
        return { x + value.x, y + value.y };
    }

	tvec2 operator-(const T value) const {
        return { x - value, y - value };
    }

    tvec2 operator-(const tvec2& value) const {
        return { x - value.x, y - value.y };
    }

    tvec2 operator*(const T value) const {
        return { x * value, y * value };
    }

    tvec2 operator*(const tvec2& value) const {
        return { x * value.x, y * value.y };
    }
};

typedef tvec2<int> ivec2;
typedef tvec2<float> vec2;

inline bool pointInRect(const ivec2& point, const ivec2& rectMin, const ivec2& rectMax) {
    return point.x >= rectMin.x && point.y >= rectMin.y &&
           point.x <= rectMax.x && point.y <= rectMax.y;
}

inline float fastSqrt(float n) {
    n = 1.0f / n;
    long i;
    float x, y;

    x = n * 0.5f;
    y = n;
    i = *(long *)&y;
    i = 0x5f3759df - (i >> 1);
    y = *(float *)&i;
    y = y * (1.5f - (x * y * y));

    return y;
}

template <typename T>
inline distance(const tvec2<T>& p0, const tvec2<T>& p1) {
	tvec2<T> diff = p0 - p1;
	return fastSqrt(diff.x * diff.x + diff.y * diff.y);
}

}

#endif // MATHCOMMON_H
