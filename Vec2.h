#ifndef PHYSICSSIM_VEC2_H
#define PHYSICSSIM_VEC2_H

#include <cmath>
#include <iostream>

template <class T>
class Vec2 {
public:
    T x, y;

    Vec2() : x(0), y(0) {}
    Vec2(T x, T y) : x(x), y(y) {}
    Vec2(const Vec2& v) : x(v.x), y(v.y) {}

    friend Vec2 operator+(const Vec2& lhs, const Vec2& rhs) {
        return Vec2(lhs) += rhs;
    }

    friend Vec2 operator-(const Vec2& lhs, const Vec2& rhs) {
        return Vec2(lhs) -= rhs;
    }

    friend Vec2 operator+(const Vec2& lhs, double rhs) {
        return Vec2(lhs) += rhs;
    }

    friend Vec2 operator-(const Vec2& lhs, double rhs) {
        return Vec2(lhs) -= rhs;
    }

    friend Vec2 operator*(const Vec2& lhs, double rhs) {
        return Vec2(lhs) *= rhs;
    }

    friend Vec2 operator/(const Vec2& lhs, double rhs) {
        return Vec2(lhs) /= rhs;
    }

    Vec2& operator=(const Vec2& v) {
        x = v.x;
        y = v.y;
        return *this;
    }

    Vec2& operator+=(const Vec2& v) {
        x += v.x;
        y += v.y;
        return *this;
    }

    Vec2& operator-=(const Vec2& v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    Vec2& operator+=(double s) {
        x += s;
        y += s;
        return *this;
    }

    Vec2& operator-=(double s) {
        x -= s;
        y -= s;
        return *this;
    }

    Vec2& operator*=(double s) {
        x *= s;
        y *= s;
        return *this;
    }

    Vec2& operator/=(double s) {
        x /= s;
        y /= s;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const Vec2<T>& vec) {
        os << "x: " << vec.x << " y: " << vec.y;
        return os;
    }

    float length() const {
        return std::sqrt(x * x + y * y);
    }
};

typedef Vec2<float> Vec2f;
typedef Vec2<double> Vec2d;

#endif //PHYSICSSIM_VEC2_H
