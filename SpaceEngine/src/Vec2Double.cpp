#include "Vec2Double.h"

#include <math.h>

Vec2Double& Vec2Double::operator=(const Vec2Double& v)
{
    x = v.x;
    y = v.y;

    return *this;
}

bool Vec2Double::operator==(const Vec2Double& v) const
{
    return x == v.x && y == v.y;
}

bool Vec2Double::operator!=(const Vec2Double& v) const
{
    return x != v.x || y != v.y;
}

Vec2Double Vec2Double::operator+(const Vec2Double& v) const
{
    return Vec2Double(x + v.x, y + v.y);
}

Vec2Double Vec2Double::operator-(const Vec2Double& v) const
{
    return Vec2Double(x - v.x, y - v.y);
}

Vec2Double Vec2Double::operator*(const float n) const
{
    return Vec2Double(x * n, y * n);
}

Vec2Double Vec2Double::operator/(const float n) const
{
    return Vec2Double(x / n, y / n);
}

Vec2Double Vec2Double::operator-()
{
    return Vec2Double(-x, -y);
}

Vec2Double& Vec2Double::operator+=(const Vec2Double& v)
{
    x += v.x;
    y += v.y;

    return *this;
}

Vec2Double& Vec2Double::operator-=(const Vec2Double& v)
{
    x -= v.x;
    y -= v.y;

    return *this;
}

Vec2Double& Vec2Double::operator*=(const float n)
{
    x *= n;
    y *= n;

    return *this;
}

Vec2Double& Vec2Double::operator/=(const float n)
{
    x /= n;
    y /= n;

    return *this;
}

double Vec2Double::Magnitude() const
{
    return sqrt(x * x + y * y);
}

double Vec2Double::MagnitudeSquared() const
{
    return x * x + y * y;
}

double Vec2Double::Dot(const Vec2Double& v) const
{
    return x * v.x + y * v.y;
}

double Vec2Double::Cross(const Vec2Double& v) const
{
    return x * v.y - y * v.x;
}
