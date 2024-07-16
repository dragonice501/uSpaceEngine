#pragma once

struct Vec2Double
{
	double x;
	double y;

	Vec2Double() : Vec2Double(0, 0) {}
	Vec2Double(double n) : Vec2Double(n, n) {}
	Vec2Double(double x, double y) : x(x), y(y) {}

	Vec2Double& operator = (const Vec2Double& v);
	bool operator == (const Vec2Double& v) const;
	bool operator != (const Vec2Double& v) const;

	Vec2Double operator + (const Vec2Double& v) const;
	Vec2Double operator - (const Vec2Double& v) const;
	Vec2Double operator * (const float n) const;
	Vec2Double operator / (const float n) const;
	Vec2Double operator - ();

	Vec2Double& operator += (const Vec2Double& v);
	Vec2Double& operator -= (const Vec2Double& v);
	Vec2Double& operator *= (const float n);
	Vec2Double& operator /= (const float n);

	double Magnitude() const;
	double MagnitudeSquared() const;
	
	double Dot(const Vec2Double& v) const;
	double Cross(const Vec2Double& v) const;
};