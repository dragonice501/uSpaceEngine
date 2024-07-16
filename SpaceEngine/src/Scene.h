#pragma once

#include "Engine.h"

#include <SDL.h>

class Scene
{
public:
	virtual void Setup() = 0;
	virtual void Destroy() = 0;
	virtual void Input() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;
};