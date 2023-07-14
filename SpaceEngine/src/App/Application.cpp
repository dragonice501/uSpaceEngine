#include "Application.h"
#include "../Graphics/GraphicsConstants.h"
#include "../Graphics/Font.h"
#include "../Phsyics/Force.h"
#include "../Phsyics/PhysicsConstants.h"
#include "../Phsyics/CollisionDetection.h"
#include "../Phsyics/Contact.h"
#include "../Utils/Utils.h"

#include <SDL.h>
#include <iostream>

bool Application::IsRunning()
{
    return running;
}

void Application::Setup()
{
    running = Graphics::OpenWindow();

    moon = new Body(CircleShape(150), Graphics::ScreenWidth() * 0.5f, Graphics::ScreenHeight() * 0.5f, 150.0f);
    moon->resolvePentration = false;
    CircleShape* moonShape = static_cast<CircleShape*>(moon->shape);

    ship = new Body(CircleShape(5), 0, 0, 1.0);
    CircleShape* shape = static_cast<CircleShape*>(ship->shape);
    ship->position = Vec2(moon->position.x, moon->position.y - moonShape->radius - shape->radius);
    ship->restitution = 0.5f;
}

void Application::Destroy()
{
    delete ship;
    delete moon;

    Graphics::CloseWindow();
}

void Application::Input() {
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
            {
                running = false;
                break;
            }
            case SDL_KEYDOWN:
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = false;

                if (event.key.keysym.sym == SDLK_COMMA)
                {
                    timeAccelIndex--;
                    if (timeAccelIndex < 0) timeAccelIndex = 0;
                    break;
                }
                if (event.key.keysym.sym == SDLK_PERIOD)
                {
                    timeAccelIndex++;
                    if (timeAccelIndex > 5) timeAccelIndex = 5;
                    break;
                }

                if (event.key.keysym.sym == SDLK_1)
                {
                    throttle = 0.1f;
                    break;
                }
                if (event.key.keysym.sym == SDLK_2)
                {
                    throttle = 0.2f;
                    break;
                }
                if (event.key.keysym.sym == SDLK_3)
                {
                    throttle = 0.3f;
                    break;
                }
                if (event.key.keysym.sym == SDLK_4)
                {
                    throttle = 0.4f;
                    break;
                }
                if (event.key.keysym.sym == SDLK_5)
                {
                    throttle = 0.5f;
                    break;
                }
                if (event.key.keysym.sym == SDLK_6)
                {
                    throttle = 0.6f;
                    break;
                }
                if (event.key.keysym.sym == SDLK_7)
                {
                    throttle = 0.7f;
                    break;
                }
                if (event.key.keysym.sym == SDLK_8)
                {
                    throttle = 0.8f;
                    break;
                }
                if (event.key.keysym.sym == SDLK_9)
                {
                    throttle = 0.9f;
                    break;
                }

                if (event.key.keysym.sym == SDLK_r)
                {
                    Reset();
                    break;
                }
                if (event.key.keysym.sym == SDLK_h)
                {
                    showTutorial = !showTutorial;
                    break;
                }

                if (event.key.keysym.sym == SDLK_v)
                {
                    showVectors = !showVectors;
                    break;
                }
                if (event.key.keysym.sym == SDLK_t)
                {
                    showTrajectory = !showTrajectory;
                    break;
                }

                if (event.key.keysym.sym == SDLK_w)
                {
                    throttleForwardPressed = true;
                    break;
                }
                if (event.key.keysym.sym == SDLK_s)
                {
                    throttleBackPressed = true;
                    break;
                }
                if (event.key.keysym.sym == SDLK_a)
                {
                    turnLeftPressed = true;
                    break;
                }
                if (event.key.keysym.sym == SDLK_d)
                {
                    turnRightPressed = true;
                    break;
                }

                if (event.key.keysym.sym == SDLK_z)
                {
                    throttle = throttleMax;
                    break;
                }
                else if (event.key.keysym.sym == SDLK_x)
                {
                    throttle = 0.0f;
                    break;
                }

                break;
            }
            case SDL_KEYUP:
            {
                if (event.key.keysym.sym == SDLK_w)
                    throttleForwardPressed = false;
                if (event.key.keysym.sym == SDLK_s)
                    throttleBackPressed = false;
                if (event.key.keysym.sym == SDLK_a)
                    turnLeftPressed = false;
                if (event.key.keysym.sym == SDLK_d)
                    turnRightPressed = false;
                break;
            }
            case SDL_MOUSEMOTION:
            {
                if (mouseButtonDown)
                {
                    Graphics::AdjustScreenOffset(Vec2(event.motion.xrel * MOUSE_SENSITIVITY, event.motion.yrel * MOUSE_SENSITIVITY));
                }
                break;
            }
            case SDL_MOUSEBUTTONDOWN:
            {
                if (event.button.button == SDL_BUTTON_RIGHT)
                    mouseButtonDown = true;

                break;
            }
            case SDL_MOUSEBUTTONUP:
            {
                if (event.button.button == SDL_BUTTON_RIGHT)
                    mouseButtonDown = false;

                break;
            }

            case SDL_MOUSEWHEEL:
            {
                Graphics::ScrollZoom(event.wheel.y);
            }
        }
    }
}

void Application::Update()
{
    static int timePreviousFrame;
    int timeToWait = MILLISECONDS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);
    if (timeToWait > 0)
    {
        SDL_Delay(timeToWait);
    }

    deltaTime = (SDL_GetTicks() - timePreviousFrame) / 1000.0f;
    if (deltaTime > 0.016f) deltaTime = 0.016f;
    deltaTime *= timeAccelerationSpeeds[timeAccelIndex];

    timePreviousFrame = SDL_GetTicks();

    UpdateShip(deltaTime);
}

void Application::Render()
{
    if(moon->shape->GetType() == CIRCLE)
    {
        CircleShape* circle = static_cast<CircleShape*>(moon->shape);
        Graphics::DrawFillCircle(moon->position.x, moon->position.y, circle->radius, 0xFFFFFFAA);
    }

    if (ship)
    {
        if (showVectors && ship->shape->GetType() == CIRCLE)
        {
            CircleShape* circle = static_cast<CircleShape*>(ship->shape);
            Vec2 velocityVector = ship->velocity;
            velocityVector.Normalize();

            Graphics::DrawLine(
                ship->position.x,
                ship->position.y,
                ship->position.x + velocityVector.x * circle->radius * 3.0f,
                ship->position.y + velocityVector.y * circle->radius * 3.0f,
                0xFFFFFF00, false);

            Graphics::DrawLine(
                ship->position.x,
                ship->position.y,
                ship->position.x + thrustVector.x * circle->radius * 3.0f,
                ship->position.y + thrustVector.y * circle->radius * 3.0f,
                0xFFFF0000, false);
        }

        if (ship->shape->GetType() == CIRCLE)
        {
            CircleShape* circle = static_cast<CircleShape*>(ship->shape);
            Graphics::DrawFillCircle(ship->position.x, ship->position.y, circle->radius, 0xFFFFAA00);
        }

        if (showTrajectory)
        {
            Body dummyParticle(*ship);
            Body dummyMoon(*moon);

            Vec2 attraction;
            Vec2 acceleration;

            Vec2 lastPosition;

            Contact contact;

            for (int i = 0; i < 500; i++)
            {
                lastPosition = dummyParticle.position;

                attraction = Force::GenerateGravitationalForce(dummyParticle, dummyMoon, G, 0, 100);
                dummyParticle.acceleration = attraction * dummyParticle.invMass;
                dummyParticle.velocity += dummyParticle.acceleration * 0.5f;
                dummyParticle.position += dummyParticle.velocity * 0.5f;

                if (CollisionDetection::IsColliding(&dummyParticle, &dummyMoon, contact)) break;

                Graphics::DrawPixel(dummyParticle.position.x + Graphics::screenOffset.x, dummyParticle.position.y + Graphics::screenOffset.y, 0xFFAAAAFF);
            }
        }
    }

    if(showTutorial)
        RenderTutorialText();
    if (ship)
        RenderUI();

    Graphics::DrawString(
        moon->position.x - Font::GetStringFontLength("tulli") * 0.5f,
        moon->position.y - Font::fontHeight * 0.5f,
        "tulli",
        0xFF000000, false);

    Graphics::RenderFrame();

    Graphics::ClearScreen(0xFF111122);
}

void Application::Reset()
{
    CircleShape* shape = static_cast<CircleShape*>(ship->shape);
    CircleShape* moonShape = static_cast<CircleShape*>(moon->shape);

    ship->position = Vec2(moon->position.x, moon->position.y - moonShape->radius - shape->radius);
    ship->velocity = Vec2(0.0f, 0.0f);
    ship->rotation = -M_PI * 0.5f;

    thrustVector = Vec2(0.0f, -1.0f);
    fuel = 1.0f;

    timeAccelIndex = 1;

    Graphics::ResetScreenOffset();
}

void Application::UpdateShip(float dt)
{
    if (throttleForwardPressed)
    {
        throttle += throttleRate * deltaTime;

        if (throttle > throttleMax) throttle = throttleMax;
    }
    else if (throttleBackPressed)
    {
        throttle -= throttleRate * deltaTime;

        if (throttle < 0.0f) throttle = 0.0f;
    }

    if (turnRightPressed)
    {
        thrustVector = thrustVector.Rotate(M_PI * turnSpeed * deltaTime);
    }
    if (turnLeftPressed)
    {
        thrustVector = thrustVector.Rotate(-M_PI * turnSpeed * deltaTime);
    }

    Vec2 attraction = Force::GenerateGravitationalForce(*ship, *moon, G, 0, 100);
    ship->AddForce(attraction);

    if (throttle != 0.0f && fuel != 0.0f)
    {
        ship->AddForce(thrustVector * thrust * throttle);
        fuel -= fuelMaxBurnRate * throttle * deltaTime;
        if (fuel < 0.0f) fuel = 0.0f;
    }

    ship->Update(deltaTime);

    Contact contact;
    if (CollisionDetection::IsColliding(ship, moon, contact))
    {
        contact.ResolveCollision();
    }
}

void Application::RenderTutorialText()
{
    Graphics::DrawString(10, 10, "get the rocket into orbit!!!", 0xFFADD8E6, true);

    Graphics::DrawString(10, 30, "press 'w' or 's' to throttle up or down.", 0xFFADD8E6, true);
    Graphics::DrawString(10, 40, "press 'x' to max your throttle.", 0xFFADD8E6, true);
    Graphics::DrawString(10, 50, "press 'z' to zero it.", 0xFFADD8E6, true);
    Graphics::DrawString(10, 60, "press '1-9' for different throttles.", 0xFFADD8E6, true);
    Graphics::DrawString(10, 70, "press 'a' and 'd' to turn.", 0xFFADD8E6, true);

    Graphics::DrawString(10, 90, "press 'v' to toggle your vectors.", 0xFFADD8E6, true);
    Graphics::DrawString(10, 100, "the red line is where you're pointing.", 0xFFADD8E6, true);
    Graphics::DrawString(10, 110, "the yellow line is where you're going.", 0xFFADD8E6, true);

    Graphics::DrawString(10, 130, "press 't' to toggle your trajectory.", 0xFFADD8E6, true);

    Graphics::DrawString(10, 150, "press ',' or '.' to change the time acceleration!", 0xFFADD8E6, true);
    
    Graphics::DrawString(10, 170, "press 'r' to reset", 0xFFADD8E6, true);
    Graphics::DrawString(10, 180, "press 'ecs' to quit", 0xFFADD8E6, true);
    Graphics::DrawString(10, 190, "press 'h' to hide the tutorial", 0xFFADD8E6, true);
}

void Application::RenderUI()
{
    Graphics::DrawString(10, Graphics::ScreenHeight() - 310, std::to_string(Graphics::screenZoom).c_str(), 0xFFADD8E6, true);

    const char* timeAccelString = "time acceleration: x ";
    Graphics::DrawString(10, Graphics::ScreenHeight() - 300, timeAccelString, 0xFFADD8E6, true);

    Graphics::DrawString(
        10 + Font::GetStringFontLength(timeAccelString),
        Graphics::ScreenHeight() - 300.0f,
        std::to_string(timeAccelerationSpeeds[timeAccelIndex]).c_str(),
        0xFFADD8E6, true);

    // Orbit

    const char* apoapsisString = "apoapsis: ";
    Graphics::DrawString(10, Graphics::ScreenHeight() - 250, apoapsisString, 0xFFADD8E6, true);

    const char* periapsisString = "periapsis: ";
    Graphics::DrawString(10, Graphics::ScreenHeight() - 240, periapsisString, 0xFFADD8E6, true);

    const char* taString = "ta: ";
    Graphics::DrawString(10, Graphics::ScreenHeight() - 220, taString, 0xFFADD8E6, true);
    const char* eccString = "ecc: ";
    Graphics::DrawString(10, Graphics::ScreenHeight() - 210, eccString, 0xFFADD8E6, true);
    const char* fiString = "fi: ";
    Graphics::DrawString(10, Graphics::ScreenHeight() - 200, fiString, 0xFFADD8E6, true);
    const char* tString = "t: ";
    Graphics::DrawString(10, Graphics::ScreenHeight() - 190, tString, 0xFFADD8E6, true);
    const char* aString = "a: ";
    Graphics::DrawString(10, Graphics::ScreenHeight() - 180, aString, 0xFFADD8E6, true);

    const char* fpaString = "fpa: ";
    Graphics::DrawString(10, Graphics::ScreenHeight() - 160, fpaString, 0xFFADD8E6, true);

    const char* gravityString = "gravity: ";
    Graphics::DrawString(10, Graphics::ScreenHeight() - 150, gravityString, 0xFFADD8E6, true);
    /*Graphics::DrawString(
        10 + Font::GetStringFontLength(gravityString),
        Graphics::ScreenHeight() - 150,
        std::to_string(Force::GenerateGravitationalForce(*ship, *moon, G, 0, 100).Magnitude()).c_str(), 0xFFADD8E6);*/

    const char* altitudeString = "altitude: ";
    Graphics::DrawString(10, Graphics::ScreenHeight() - 110, altitudeString, 0xFFADD8E6, true);

    CircleShape* moonShape = static_cast<CircleShape*>(moon->shape);
    CircleShape* shipShape = static_cast<CircleShape*>(ship->shape);
    Graphics::DrawString(
        10 + Font::GetStringFontLength(altitudeString),
        Graphics::ScreenHeight() - 110,
        std::to_string((ship->position - moon->position).Magnitude() - moonShape->radius - shipShape->radius).c_str(),
        0xFFADD8E6, true);

    const char* velocityString = "velocity: ";
    Graphics::DrawString(10, Graphics::ScreenHeight() - 100, velocityString, 0xFFADD8E6, true);
    Graphics::DrawString(
        10 + Font::GetStringFontLength(velocityString),
        Graphics::ScreenHeight() - 100,
        std::to_string(ship->velocity.Magnitude()).c_str(),
        0xFFADD8E6, true);

    float foeDot = (thrustVector.UnitVector().Dot((ship->position - moon->position).UnitVector())) * 90.0f;
    const char* foeString = "foe: ";
    Graphics::DrawString(10, Graphics::ScreenHeight() - 90, foeString, 0xFFADD8E6, true);
    Graphics::DrawString(
        10 + Font::GetStringFontLength(foeString),
        Graphics::ScreenHeight() - 90,
        std::to_string(foeDot).c_str(),
        0xFFADD8E6, true);

    const char* fuelString = "fuel: ";
    Graphics::DrawString(10, Graphics::ScreenHeight() - 70, fuelString, 0xFFADD8E6, true);
    Graphics::DrawString(
        10 + Font::GetStringFontLength(fuelString),
        Graphics::ScreenHeight() - 70,
        std::to_string(fuel * 100.0f).c_str(),
        0xFFADD8E6, true);
    Graphics::DrawFillRect(10, Graphics::ScreenHeight() - 62, 104, 14, 0xFFFFFFFF);
    Graphics::DrawFillRect(12, Graphics::ScreenHeight() - 60, 100, 10, 0xFF000055);
    Graphics::DrawFillRect(12, Graphics::ScreenHeight() - 60, fuel * 100, 10, 0xFF00AAAA);

    Graphics::DrawString(10, Graphics::ScreenHeight() - 40, "throttle", 0xFFADD8E6, true);

    Graphics::DrawFillRect(10, Graphics::ScreenHeight() - 32, 104, 24, 0xFFFFFFFF);
    Graphics::DrawFillRect(12, Graphics::ScreenHeight() - 30, 100, 20, 0xFF005500);
    Graphics::DrawFillRect(12, Graphics::ScreenHeight() - 30, throttle * 100, 20, 0xFF00AA00);

    Graphics::DrawLine(45, Graphics::ScreenHeight() - 10, 45, Graphics::ScreenHeight() - 25, 0xFFFFFFFF, true);
    Graphics::DrawLine(78, Graphics::ScreenHeight() - 10, 78, Graphics::ScreenHeight() - 25, 0xFFFFFFFF, true);

    Graphics::DrawLine(23, Graphics::ScreenHeight() - 10, 23, Graphics::ScreenHeight() - 20, 0xFFFFFFFF, true);
    Graphics::DrawLine(34, Graphics::ScreenHeight() - 10, 34, Graphics::ScreenHeight() - 20, 0xFFFFFFFF, true);
    Graphics::DrawLine(56, Graphics::ScreenHeight() - 10, 56, Graphics::ScreenHeight() - 20, 0xFFFFFFFF, true);
    Graphics::DrawLine(67, Graphics::ScreenHeight() - 10, 67, Graphics::ScreenHeight() - 20, 0xFFFFFFFF, true);
    Graphics::DrawLine(89, Graphics::ScreenHeight() - 10, 89, Graphics::ScreenHeight() - 20, 0xFFFFFFFF, true);
    Graphics::DrawLine(100, Graphics::ScreenHeight() - 10, 100, Graphics::ScreenHeight() - 20, 0xFFFFFFFF, true);
}
