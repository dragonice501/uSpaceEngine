#pragma once

#pragma once

struct Vec3Double
{
	double x;
	double y;
	double z;

	Vec3Double() : Vec3Double(0, 0, 0) {}
	Vec3Double(double n) : Vec3Double(n, n, n) {}
	Vec3Double(double x, double y, double z) : x(x), y(y), z(z) {}

	Vec3Double& operator = (const Vec3Double& v);
	bool operator == (const Vec3Double& v) const;
	bool operator != (const Vec3Double& v) const;

	Vec3Double operator + (const Vec3Double& v) const;
	Vec3Double operator - (const Vec3Double& v) const;
	Vec3Double operator * (const float n) const;
	Vec3Double operator / (const float n) const;
	Vec3Double operator - ();

	Vec3Double& operator += (const Vec3Double& v);
	Vec3Double& operator -= (const Vec3Double& v);
	Vec3Double& operator *= (const float n);
	Vec3Double& operator /= (const float n);

	double Magnitude() const;
	double MagnitudeSquared() const;

	double Dot(const Vec3Double& v) const;
	Vec3Double Cross(const Vec3Double& v) const;
};