#include "RandomSystemScene.h"

void RandomSystemScene::Setup()
{
	GenerateSun();
}

void RandomSystemScene::Destroy()
{
}

void RandomSystemScene::Input()
{
}

void RandomSystemScene::Update(float deltaTime)
{
}

void RandomSystemScene::Render()
{
	float sunU = sun.position.x / systemMax;
	float sunV = sunU;

	Graphics::DrawFillCircle(Graphics::screenWidth * sunU, Graphics::screenHeight * sunV, 10, 0xFFFFFF00);
}

void RandomSystemScene::GenerateSun()
{
	sun.mass = sunSize * pow(10, sunExponent);
	sun.radius = 696347055;
	sun.position = Vec3Double(systemMax / 2, 0, systemMax / 2);
}
