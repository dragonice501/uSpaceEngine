#include "Ship.h"
#include "../Graphics/Graphics.h"

Ship::Ship()
{
}

Ship::Ship(const float& x, const float& y, const int& radius, const float& mass)
{
	body = new Body(CircleShape(radius), x, y, mass);
}

Ship::~Ship()
{
	delete body;
}

void Ship::Update(float dt)
{
}

void Ship::Render()
{
}
