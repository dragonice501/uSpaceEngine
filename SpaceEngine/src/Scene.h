#pragma once

#include "Engine.h"

#include "PhysicsConstants.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_sdl2.h"

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