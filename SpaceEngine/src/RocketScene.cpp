#include "RocketScene.h"

#include <iostream>
#include <algorithm>

void RocketScene::Setup()
{
	planet.mass = 5.2915158e22;
	planet.radius = 6e6;

	rocket.mass = 9.908e6;
	rocket.position.y = planet.radius + 10;

    Graphics::IncrementZoom(100000);
}

void RocketScene::Destroy()
{
}

void RocketScene::Input()
{
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent))
    {
        ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
        ImGuiIO& io = ImGui::GetIO();

        switch (sdlEvent.type)
        {
        case SDL_QUIT:
        {
            Engine::SetIsRunning(false);
            break;
        }
        case SDL_MOUSEMOTION:
        {
            Graphics::SetMousePos(Vec2(io.MousePos.x, Graphics::screenHeight - io.MousePos.y - 1));

            if (mouseButtonDown)
            {
                Graphics::AddScreenOffset(sdlEvent.motion.xrel, -sdlEvent.motion.yrel);
            }
            break;
        }
        case SDL_MOUSEWHEEL:
        {
            Vec2 previousMousePos = Graphics::GetMouseWorld();

            Graphics::IncrementZoom(sdlEvent.wheel.y);

            Vec2 newMousePos = Graphics::GetMouseWorld();

            Vec2 offset = (newMousePos - previousMousePos) / Graphics::GetZoom();

            Graphics::AddScreenOffset(offset.x, offset.y);

            break;
        }
        case SDL_MOUSEBUTTONDOWN:
        {
            switch (sdlEvent.button.button)
            {
            case SDL_BUTTON_LEFT:
            {
                mouseButtonDown = true;
                break;
            }
            }
            break;
        }
        case SDL_MOUSEBUTTONUP:
        {
            switch (sdlEvent.button.button)
            {
            case SDL_BUTTON_LEFT:
            {
                mouseButtonDown = false;
                break;
            }
            }
            break;
        }
        case SDL_KEYDOWN:
        {
            if (sdlEvent.key.keysym.sym == SDLK_ESCAPE)
                Engine::SetIsRunning(false);

            if (sdlEvent.key.keysym.sym == SDLK_COMMA)
            {
                Engine::DecrementTimeAccel();
                break;
            }
            if (sdlEvent.key.keysym.sym == SDLK_PERIOD)
            {
                Engine::IncrementTimeAccel();
                break;
            }
            break;
        }
        }
    }
}

void RocketScene::Update(float deltaTime)
{
    dT = deltaTime;
}

void RocketScene::Render()
{
    Vec2Double offset = Graphics::GetScreenOffset();
    double u = offset.x / Graphics::screenWidth;
    double v = offset.y / Graphics::screenHeight;

    Graphics::DrawPlanet(0, 0, planet.radius, 0xFFFFff55);
    Graphics::DrawGrid(offset.x, offset.y, Graphics::GetScreenScale());
    Graphics::DrawPlanet(rocket.position.x, rocket.position.y, 10, 0xFFFF0000);
    Graphics::DrawXYAxis(u, v);

    static bool p;
    ImGui::Begin("Rocket Scene", &p, ImGuiWindowFlags_NoMove);
    ImGui::Text(("DeltaTime: " + std::to_string(dT)).c_str());

    ImGui::Text(("System Size: " + std::to_string(100)).c_str());
    ImGui::Text(("World X: " + std::to_string(Graphics::GetMouseWorld().x)).c_str());
    ImGui::Text(("World Y : " + std::to_string(Graphics::GetMouseWorld().y)).c_str());
    ImGui::Text(("Screen X: " + std::to_string(Graphics::GetMouseScreen().x)).c_str());
    ImGui::Text(("Screen Y : " + std::to_string(Graphics::GetMouseScreen().y)).c_str());
    ImGui::Text(("Zoom: " + std::to_string(Graphics::GetZoom())).c_str());
    ImGui::Text(("Zoom Size: " + std::to_string(Graphics::GetZoom() * 100)).c_str());
    ImGui::Text(("GridScale : " + std::to_string(Graphics::GetScreenScaleIndex())).c_str());

    ImGui::End();
}

void RocketScene::GeneratePoints(float start, float end, int count)
{
    points.clear();

    float step = (end - start) / count;

    for (size_t i = 0; i < count; i++)
    {
        float a = start + step * i;

        points.push_back(GeneratePoint(a));
    }
}

Vec3Double RocketScene::GeneratePoint(float p)
{
    return Vec3Double(p * 20, sin(p) * 100, 0);
}

void RocketScene::Tessellate(Vec3Double a, Vec3Double b, int t)
{
    float x = (a.x + b.x) * 0.5f;
    Vec3Double ab(x, sin(x * 0.02f) * 100.0f + Graphics::screenHeight * 0.5f, 0);

    points.push_back(ab);
    std::sort(points.begin(), points.end(), [](Vec3Double x, Vec3Double y)
        {
            return x.x < y.x;
        });

    t--;
    if (t <= 0) return;

    Tessellate(a, ab, t);
    Tessellate(ab, b, t);
}

float RocketScene::ComputeTriangleArea(const Vec3Double& a, const Vec3Double& b, const Vec3Double& c)
{
    return 0.0f;
}
