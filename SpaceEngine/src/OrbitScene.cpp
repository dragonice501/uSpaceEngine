#include "OrbitScene.h"

#include "Vec2Double.h"

void OrbitScene::Setup()
{
    sun.mass = 1.989e30;
    sun.radius = 696347055;
    sun.size = 10;
    sun.color = 0xffffff00;

    mercury.mass = 3.285e23;
    mercury.radius = 2439765.5;
    mercury.semiMajorAxis = 5.793684e10;
    mercury.eccentricity = 0.205f;
    mercury.argPeriapsis = 29.124;
    mercury.size = 3;
    mercury.color = 0xFFFF3333;

    venus.mass = 4.867e24;
    venus.radius = 6051777;
    venus.semiMajorAxis = 1.08212291e11;
    venus.eccentricity = 0.007f;
    venus.argPeriapsis = 54.884f;
    venus.size = 4;
    venus.color = 0xf9c21a;

    earth.mass = 5.97e24;
    earth.radius = 6371000;
    earth.semiMajorAxis = 1.49598023e11;
    earth.eccentricity = 0.016f;
    earth.argPeriapsis = 114.302f;
    earth.size = 5;
    earth.color = 0xff00ff00;

    mars.mass = 6.39e23;
    mars.radius = 3389439;
    mars.semiMajorAxis = 2.2278831104e11;
    mars.eccentricity = 0.0934f;
    mars.argPeriapsis = 286.5f;
    mars.size = 4;
    mars.color = 0xFFFF0000;

    jupiter.mass = 1.898e27;
    jupiter.radius = 69911513;
    jupiter.semiMajorAxis = 7.786e11;
    jupiter.eccentricity = 0.0489f;
    jupiter.argPeriapsis = 273.867f;
    jupiter.size = 8;
    jupiter.color = 0xd8ca9d;

    saturn.mass = 5.685e26;
    saturn.radius = 58232503;
    saturn.semiMajorAxis = 1.433603635e12;
    saturn.eccentricity = 0.0565f;
    saturn.argPeriapsis = 339.392f;
    saturn.size = 7;
    saturn.color = 0xead6b8;

    uranus.mass = 8.681e25;
    uranus.radius = 25361652;
    uranus.semiMajorAxis = 2.871e12;
    uranus.eccentricity = 0.04717f;
    uranus.argPeriapsis = 96.998f;
    uranus.size = 6;
    uranus.color = 0xd1e7e7;

    neptune.mass = 1.024e26;
    neptune.radius = 24621354;
    neptune.semiMajorAxis = 4.494897792e12;
    neptune.eccentricity = 0.009f;
    neptune.argPeriapsis = 273.187f;
    neptune.size = 6;
    neptune.color = 0x3f54ba;

    pluto.mass = 1.309e22;
    pluto.radius = 1188307;
    pluto.semiMajorAxis = 5.9e12;
    pluto.eccentricity = 0.2488f;
    pluto.argPeriapsis = 113.834f;
    pluto.size = 2;
    pluto.color = 0xf6ddbd;

    planets.push_back(&mercury);
    planets.push_back(&venus);
    planets.push_back(&earth);
    planets.push_back(&mars);
    planets.push_back(&jupiter);
    planets.push_back(&saturn);
    planets.push_back(&uranus);
    planets.push_back(&neptune);
    planets.push_back(&pluto);

    for (auto planet : planets)
    {
        if (planet)
        {
            planet->CalculateParametersAndElements(sun);
            planet->CalculateOrbitPoints(systemSize);
        }
    }
}

void OrbitScene::Destroy()
{
}
 
void OrbitScene::Input()
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
                if (mouseButtonDown)
                {
                    systemOffset.x += sdlEvent.motion.xrel * systemSize * 0.0005f;
                    systemOffset.z -= sdlEvent.motion.yrel * systemSize * 0.0005f;
                }
                break;
            }
            case SDL_MOUSEWHEEL:
            {
                systemSize -= sdlEvent.wheel.y * systemSize * 0.1f;
                if (systemSize <= 2e11)
                {
                    systemSize = 2e11;
                    return;
                }
                else if (systemSize >= 262e11)
                {
                    systemSize = 260e11;
                    return;
                }

                float r = io.DisplaySize.x / 2;
                float t = io.DisplaySize.y / 2;
                float mouseX = (r - io.MousePos.x) / r;
                float mouseZ = ((t - io.MousePos.y) / t) * Graphics::yAspect;

                systemOffset.x += (systemSize / 25) * mouseX * sdlEvent.wheel.y;
                systemOffset.z -= ((systemSize) / 25) * mouseZ * sdlEvent.wheel.y;

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

void OrbitScene::Update(float deltaTime)
{
    for (auto planet : planets)
    {
        if (planet)
        {
            planet->currentPeriod += deltaTime * Engine::GetTimeAccel();
            if (planet->currentPeriod >= planet->period) planet->currentPeriod -= planet->period;

            // Find Mean Anomaly
            planet->meanAnomaly = planet->n * (planet->currentPeriod - planet->period);

            // Find Eccentric Anomaly
            double E = planet->meanAnomaly;
            for (size_t i = 0; i < 10; i++)
            {
                E =
                    E -
                    (E - planet->eccentricity * sin(E) - planet->meanAnomaly) /
                    (1 - planet->eccentricity * cos(E));
            }

            // Find True Anomaly
            planet->trueAnomaly =
                2 *
                atan(
                    sqrt((1 + planet->eccentricity) / (1 - planet->eccentricity)) *
                    tan(E / 2)
                );

            CalculatePosition(*planet);
        }
    }
}

void OrbitScene::Render()
{
    double min = systemSize / 2;

    double sunU = (min + systemOffset.x) / systemSize;
    double sunV = ((systemOffset.z / Graphics::yAspect) + min) / systemSize;

    Graphics::DrawFillCircle(Graphics::screenWidth * sunU, Graphics::screenHeight * sunV, 10.0f, sun.color);

    for (auto planet : planets)
    {
        if (planet)
        {
            DrawOrbit(*planet);

            double planetU = ((planet->position.x + systemOffset.x) + min) / systemSize;
            double planetV = ((planet->position.z + systemOffset.z) / Graphics::yAspect + min) / systemSize;

            Graphics::DrawFillCircle(Graphics::screenWidth * planetU, Graphics::screenHeight * planetV, 10.0f, planet->color);
        }
    }

    //ImGui::ShowDemoWindow();
}

void OrbitScene::CalculatePosition(CelestialBody& planet)
{
    double r = 
        (planet.semiMajorAxis * (1 - pow(planet.eccentricity, 2))) /
        (1 + planet.eccentricity * cos(planet.trueAnomaly));

    planet.position.x =
        r * (cos(planet.ascLongitude) * cos(planet.argPeriapsis + planet.trueAnomaly) -
            sin(planet.ascLongitude) * sin(planet.argPeriapsis + planet.trueAnomaly) * cos(planet.inclination));

    /*planet.position.y =
        r * sin(planet.argPeriapsis + planet.trueAnomaly) * sin(planet.inclination);*/

    planet.position.z =
        r * (sin(planet.ascLongitude) * cos(planet.argPeriapsis + planet.trueAnomaly) +
            cos(planet.ascLongitude) * sin(planet.argPeriapsis + planet.trueAnomaly) * cos(planet.inclination));
}

void OrbitScene::DrawOrbit(const CelestialBody& planet)
{
    double min = systemSize / 2;
    double xO = systemOffset.x;
    double zO = systemOffset.z;

    for (size_t i = 0; i < 32; i++)
    {
        size_t j = i + 1 >= 32 ? 0 : i + 1;

        float u = (planet.orbitPoints[i].x + xO + min) / systemSize;
        float v = (((planet.orbitPoints[i].z + zO) / Graphics::yAspect) + min) / systemSize;

        float w = (planet.orbitPoints[j].x + xO + min) / systemSize;
        float x = (((planet.orbitPoints[j].z + zO) / Graphics::yAspect) + min) / systemSize;

        Graphics::DrawLine(Graphics::screenWidth * u, Graphics::screenHeight * v, Graphics::screenWidth * w, Graphics::screenHeight * x, planet.color);
    }
}
