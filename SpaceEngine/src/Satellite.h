#pragma once

#include "Body.h"

class Satellite
{
public:
	Satellite();
	Satellite(const char* newName, const float& x, const float& y, const float& radius, const float& mass, const float& soi, Satellite* influencingSatellite);
	~Satellite();

	inline Body* GetBody() const { return body; }
	inline const float& GetSOI() const { return sphereOfInfluence; }
	inline const uint32_t& GetColor() const { return color; }

	inline void SetColor(const uint32_t& newColor) { color = newColor; }
	//inline void SetName(const char* newName) { name = newName };

	void Update(const float& dt);
	void Render();

private:
	Body* body;
	Satellite* influencingSatellite;

	float sphereOfInfluence = 0.0f;

	uint32_t color;
	char name[8];
};