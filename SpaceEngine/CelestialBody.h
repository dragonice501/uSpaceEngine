#pragma once

class CelestialBody
{
private:
	double mMass;
	double mPosX;
	double mPosY;
	float mRadius;

public:
	CelestialBody() : CelestialBody(0, 0, 0, 0.0f) {}
	CelestialBody(const double mass, const double posX, const double posY, const float radius) :
		mMass(mass), mPosX(posX), mPosY(posY), mRadius(radius) {}

	~CelestialBody() = default;

	inline const double GetMass() const { return mMass; }
	inline const double GetPosX() const { return mPosX; }
	inline const double GetPosY() const { return mPosY; }
	inline const float GetRadius() const { return mRadius; }

	void SetPosX(const double x) { mPosX = x; }
	void SetPosY(const double y) { mPosY = y; }
};