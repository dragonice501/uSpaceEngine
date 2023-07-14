#pragma once

#include "../Phsyics/Body.h"

class Satellite
{
public:
	Satellite();
	Satellite(const float& x, const float& y, const float& radius, const float& mass, const float& soi);
	~Satellite();

	inline Body* GetBody() const { return body; }
	inline const float& GetSOI() const { return sphereOfInfluence; }

	void Update(const float& dt, const Satellite* satellite);
	void Render();

private:
	Body* body;

	float sphereOfInfluence = 0.0f;
};