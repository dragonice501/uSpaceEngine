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
	inline const uint32_t& GetColor() const { return color; }

	inline void SetColor(const uint32_t& newColor) { color = newColor; }
	//inline void SetName(const char* newName) { name = newName };

	void Update(const float& dt, const Satellite* satellite);
	void Render();

private:
	Body* body;

	float sphereOfInfluence = 0.0f;

	uint32_t color;
	char name[];
};