#include "Vec3Double.h"

#include <math.h>

Vec3Double& Vec3Double::operator=(const Vec3Double& v)
{
    x = v.x;
    y = v.y;
    z = v.z;

    return *this;
}

bool Vec3Double::operator==(const Vec3Double& v) const
{
    return x == v.x && y == v.y && z == v.z;
}

bool Vec3Double::operator!=(const Vec3Double& v) const
{
    return x != v.x || y != v.y || z != v.z;
}

Vec3Double Vec3Double::operator+(const Vec3Double& v) const
{
    return Vec3Double(x + v.x, y + v.y, z + v.z);
}

Vec3Double Vec3Double::operator-(const Vec3Double& v) const
{
    return Vec3Double(x - v.x, y - v.y, z - v.z);
}

Vec3Double Vec3Double::operator*(const float n) const
{
    return Vec3Double(x * n, y * n, z * n);
}

Vec3Double Vec3Double::operator/(const float n) const
{
    return Vec3Double(x / n, y / n, z / n);
}

Vec3Double Vec3Double::operator-()
{
    return Vec3Double(-x, -y, -z);
}

Vec3Double& Vec3Double::operator+=(const Vec3Double& v)
{
    x += v.x;
    y += v.y;
    z += v.z;

    return *this;
}

Vec3Double& Vec3Double::operator-=(const Vec3Double& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;

    return *this;
}

Vec3Double& Vec3Double::operator*=(const float n)
{
    x *= n;
    y *= n;
    z *= n;

    return *this;
}

Vec3Double& Vec3Double::operator/=(const float n)
{
    x /= n;
    y /= n;
    z /= n;

    return *this;
}

double Vec3Double::Magnitude() const
{
    return sqrt(x * x + y * y + z * z);
}

double Vec3Double::MagnitudeSquared() const
{
    return x * x + y * y + z * z;
}

Vec3Double Vec3Double::Normal()
{
    double mag = this->Magnitude();
    return *this / mag;
}

double Vec3Double::Dot(const Vec3Double& v) const
{
    return x * v.x + y * v.y + z * v.z;
}

Vec3Double Vec3Double::Cross(const Vec3Double& v) const
{
    return Vec3Double(
        y * v.z - z * v.y,
        z * v.x - x * v.z,
        x * v.y - y * v.z);
}
