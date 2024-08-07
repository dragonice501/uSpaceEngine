#pragma once

#include "Vec2.h"
#include "Vec2Double.h"
#include "GraphicsConstants.h"

#include <SDL.h>
#include <stdint.h>
#include <vector>
#include <string>

struct Graphics
{
    static int windowWidth;
    static int windowHeight;
    static int screenWidth;
    static int screenHeight;

    static uint32_t* colorBuffer;
    static SDL_Texture* colorBufferTexture;
    static SDL_Window* window;
    static SDL_Renderer* renderer;

    static float yAspect;
    static Vec2Double screenOffset;
    static Vec2 mousePos;
    static double screenZoom;
    static int screenScales[SCREEN_SCALES];
    static size_t scalesIndex;

    static SDL_Window* GetWindow() { return window; }
    static SDL_Renderer* GetRenderer() { return renderer; }

    static bool OpenWindow();
    static void CloseWindow();

    static int WindowWidth();
    static int WindowHeight();
    static int ScreenWidth();
    static int ScreenHeight();

    static float YAspect() { return yAspect; }
    static void SetMousePos(const Vec2& pos) { mousePos = pos; }

    static const Vec2 GetMouseWorld()
    {
        return Vec2(
        (mousePos.x - screenOffset.x) * screenZoom,
        (mousePos.y - screenOffset.y) * screenZoom);
    }
    static const Vec2 GetMouseScreen() { return mousePos; }
    static const Vec2Double& GetScreenOffset() { return screenOffset; }
    static float GetZoom() { return screenZoom; }
    static int GetScreenScale();
    static size_t GetScreenScaleIndex() { return screenScales[scalesIndex]; }

    static void AddScreenOffset(const int x, const int y);
    static void SetScreenOffset(const double x, const double y) { screenOffset = Vec2Double(x, y); }
    static void IncrementZoom(const int scroll);
    static void SetZoom(const float zoom) { screenZoom = zoom; }

    static bool CircleOffScreen(const int x, const int y, const int radius);
    static bool PlanetOffScreen(const double x, const double y, const double radius);

    static void ClearScreen(const uint32_t& color);
    static void FlipScreen();
    static void PresentFrame();

    static void DrawPixel(const int& x, const int& y, const uint32_t& color);

    static void DrawLine(const int x0, const int y0, const int x1, const int y1, const uint32_t color);
    static void DrawULine(float u);
    static void DrawXYAxis(float u = 0, float v = 0);
    static void DrawGrid(const uint32_t color);
    static void DrawGrid(int xOffset = 0, int yOffset = 0, int size = 100);
    
    static void DrawRect(const int& x, const int& y, const int& width, const int& height, const uint32_t& color);
    static void DrawFillRect(const int& x, const int& y, const int& width, const int& height, const uint32_t& color);
    static void DrawCircle(const int x, const int y, const int radius, const float angle, const uint32_t color);
    static void DrawFillCircle(const double x, const double y, const double radius, const uint32_t color);
    static void DrawPlanet(const double x, const double y, const double radius, const uint32_t color);

    static void DrawPolygon(const int& x, const int& y, const std::vector<Vec2>& vertices, const uint32_t& color, const bool& lockToScreen);
    static void DrawFillPolygon(const int& x, const int& y, const std::vector<Vec2>& vertices, const uint32_t& color, const bool& lockToScreen);
    static void DrawTexture(const int& x, const int& y, const int& width, const int& height, const float& rotation, SDL_Texture* texture);

    static void DrawChar(const int& x, const int& y, const char& character, const uint32_t& color, const bool& lockToScreen);
    static void DrawString(const int& x, const int& y, const char* string, const uint32_t& color, const bool& lockToScreen);

    static void DisplayBresenhamCircle(const int& xc, const int& yc, const int& x0, const int& y0, const uint32_t& color, const bool& lockToScreen);
};