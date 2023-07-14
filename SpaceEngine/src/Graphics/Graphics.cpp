#include "Graphics.h"
#include "GraphicsConstants.h"
#include "Font.h"
#include <iostream>

int Graphics::windowWidth = 0;
int Graphics::windowHeight = 0;
int Graphics::screenWidth = 0;
int Graphics::screenHeight = 0;

uint32_t* Graphics::colorBuffer = nullptr;
SDL_Texture* Graphics::colorBufferTexture = nullptr;
SDL_Window* Graphics::window = nullptr;
SDL_Renderer* Graphics::renderer = nullptr;

Vec2 Graphics::screenOffset = { 0.0f, 0.0f };
float Graphics::screenZoom = 1.0f;

bool Graphics::OpenWindow()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cerr << "Error initializing SDL" << std::endl;
        return false;
    }

    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);
    windowWidth = display_mode.w;
    windowHeight = display_mode.h;
    screenWidth = windowWidth * SCREEN_SCALE;
    screenHeight = windowHeight * SCREEN_SCALE;
    window = SDL_CreateWindow(nullptr, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_BORDERLESS);
    if (!window)
    {
        std::cerr << "Error creating SDL window" << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        std::cerr << "Error creating SDL renderer" << std::endl;
        return false;
    }

    colorBuffer = new uint32_t[screenWidth * screenHeight];
    colorBufferTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, screenWidth, screenHeight);

    return true;
}

void Graphics::CloseWindow(void)
{
    delete[] colorBuffer;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int Graphics::WindowWidth()
{
    return windowWidth;
}

int Graphics::WindowHeight()
{
    return windowHeight;
}

int Graphics::ScreenWidth()
{
    return screenWidth;
}

int Graphics::ScreenHeight()
{
    return screenHeight;
}

void Graphics::AdjustScreenOffset(const Vec2& offset)
{
    screenOffset.x += offset.x;
    screenOffset.y += offset.y;
}

void Graphics::ResetScreenOffset()
{
    screenOffset = { 0.0f, 0.0f };
}

void Graphics::ScrollZoom(const int& scroll)
{
    screenZoom += static_cast<float>(scroll) * 0.25f;
}

bool Graphics::CircleOffScreen(const int& x, const int& y, const float& radius)
{
    return
        x + radius + screenOffset.x < 0 ||
        x - radius + screenOffset.x > screenWidth ||
        y + radius + screenOffset.y < 0 ||
        y - radius + screenOffset.y > screenHeight;

}

void Graphics::ClearScreen(const uint32_t& color)
{
    for (int i = 0; i < screenWidth * screenHeight; i++)
    {
        colorBuffer[i] = color;
    }

    SDL_RenderClear(renderer);
}

void Graphics::RenderFrame()
{
    SDL_UpdateTexture(colorBufferTexture, nullptr, colorBuffer, sizeof(uint32_t) * screenWidth);
    SDL_RenderCopy(renderer, colorBufferTexture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

void Graphics::DrawPixel(const int& x, const int& y, const uint32_t& color)
{
    if (x < 0 || x >= screenWidth || y < 0 || y >= screenHeight) return;

    colorBuffer[x + y * screenWidth] = color;
}

void Graphics::DrawLine(const int& x0, const int& y0, const int& x1, const int& y1, const uint32_t& color, const bool& lockToScreen)
{
    float x = x0;
    float y = y0;
    int deltaX = x1 - x0;
    int deltaY = y1 - y0;

    int steps = abs(deltaX) > abs(deltaY) ? abs(deltaX) : abs(deltaY);

    float xIncrement = deltaX / (float)steps;
    float yIncrement = deltaY / (float)steps;

    for (int i = 0; i < steps; i++)
    {
        if(lockToScreen)
            DrawPixel(static_cast<int>(x), static_cast<int>(y), color);
        else
            DrawPixel(static_cast<int>(x + screenOffset.x), static_cast<int>(y + screenOffset.y), color);
        x += xIncrement;
        y += yIncrement;
    }
}

void Graphics::DrawGrid(const uint32_t& color)
{
    for (int y = 0; y < screenHeight; y++)
    {
        for (int x = 0; x < screenWidth; x++)
        {
            if (x % PIXELS_PER_METER == 0 || y % PIXELS_PER_METER == 0)
                DrawPixel(x, y, color);
        }
    }
}

void Graphics::DrawRect(const int& x, const int& y, const int& width, const int& height, const uint32_t& color)
{
    DrawPixel(x + width / 2, y + height / 2, color);

    for (int i = x; i < x + width; i++)
        colorBuffer[i + y * screenWidth] = color;

    for (int i = y; i < y + height; i++)
        colorBuffer[x + i * screenWidth] = color;

    for (int i = y; i < y + height + 1; i++)
        colorBuffer[(x + width) + i * screenWidth] = color;

    for (int i = x; i < x + width + 1; i++)
        colorBuffer[i + (y + height) * screenWidth] = color;
}

void Graphics::DrawFillRect(const int& x, const int& y, const int& width, const int& height, const uint32_t& color)
{
    for (int i = y; i < y + height; i++)
    {
        for (int j = x; j < x + width; j++)
        {
            DrawPixel(j, i, color);
        }
    }
}

void Graphics::DrawCircle(const int& x, const int& y, const int& radius, const float& angle, const uint32_t& color, const bool& lockToScreen)
{
    if (CircleOffScreen(x, y, radius)) return;

    int x0 = 0;
    int y0 = radius;
    int p0 = 3 - (2 * radius);

    DisplayBresenhamCircle(x, y, x0, y0, color, lockToScreen);

    while (y0 >= x0)
    {
        x0++;

        if (p0 > 0)
        {
            y0--;
            p0 += 4 * (x0 - y0) + 10;
        }
        else
        {
            p0 += 4 * x0 + 6;
        }

        DisplayBresenhamCircle(x, y, x0, y0, color, lockToScreen);
    }

    //DrawLine(x, y, x + cos(angle) * radius, y + sin(angle) * radius, color, lockToScreen);
}

void Graphics::DrawFillCircle(const int& x, const int& y, const int& radius, const uint32_t& color)
{
    if (CircleOffScreen(x, y, radius)) return;
    int pRadius = radius;
    int topLeftX = x - pRadius;
    int topLeftY = y - pRadius;

    for (int i = topLeftY; i < topLeftY + pRadius * 2 + 1; i++)
    {
        for (int j = topLeftX; j < topLeftX + pRadius * 2 + 1; j++)
        {
            Vec2 point = { static_cast<float>(x - j), static_cast<float>(y - i) };

            if (point.MagnitudeSquared() < pRadius * pRadius)
                DrawPixel(j + screenOffset.x, i + screenOffset.y, color);
        }
    }
}

void Graphics::DrawPolygon(const int& x, const int& y, const std::vector<Vec2>& vertices, const uint32_t& color, const bool& lockToScreen)
{
    Vec2 current = vertices[0];
    Vec2 previous = vertices[vertices.size() - 1];

    DrawLine(previous.x, previous.y, current.x, current.y, 0xFFFFFFFF, lockToScreen);

    for (int i = 1; i < vertices.size(); i++)
    {
        previous = current;
        current = vertices[i];
        DrawLine(previous.x, previous.y, current.x, current.y, color, lockToScreen);
    }
}

void Graphics::DrawFillPolygon(const int& x, const int& y, const std::vector<Vec2>& vertices, const uint32_t& color, const bool& lockToScreen)
{
    
}

void Graphics::DrawTexture(const int& x, const int& y, const int& width, const int& height, const float& rotation, SDL_Texture* texture)
{
    SDL_Rect dstRect = {x - (width / 2), y - (height / 2), width, height};
    float rotationDeg = rotation * 57.2958;
    SDL_RenderCopyEx(renderer, texture, nullptr, &dstRect, rotationDeg, nullptr, SDL_FLIP_NONE);
}

void Graphics::DrawChar(const int& x, const int& y, const char& character, const uint32_t& color, const bool& lockToScreen)
{
    for (int j = 0; j < Font::fontHeight; j++)
    {
        for (int k = 0; k < Font::fontWidth; k++)
        {
            if (character == ' ') continue;

            if (Font::fontMap[std::tolower(character)][k + j * Font::fontWidth])
            {
                if(lockToScreen)
                    DrawPixel(x + k, y + j, color);
                else
                    DrawPixel(x + k + screenOffset.x, y + j + screenOffset.y, color);
            }
        }
    }
}

void Graphics::DrawString(const int& x, const int& y, const char* string, const uint32_t& color, const bool& lockToScreen)
{
    int i = 0;
    int xPos = x;
    int yPos = y;
    while (string[i] != '\0')
    {
        DrawChar(xPos, yPos, string[i], color, lockToScreen);
        i++;
        xPos += Font::fontWidth + Font::fontSpacing;
    }
}

void Graphics::DisplayBresenhamCircle(const int& xc, const int& yc, const int& x0, const int& y0, const uint32_t& color, const bool& lockToScreen)
{
    if (lockToScreen)
    {
        DrawPixel(xc + x0, yc + y0, color);
        DrawPixel(xc - x0, yc + y0, color);
        DrawPixel(xc + x0, yc - y0, color);
        DrawPixel(xc - x0, yc - y0, color);
        DrawPixel(xc + y0, yc + x0, color);
        DrawPixel(xc - y0, yc + x0, color);
        DrawPixel(xc + y0, yc - x0, color);
        DrawPixel(xc - y0, yc - x0, color);
    }
    else
    {
        DrawPixel(xc + x0 + screenOffset.x, yc + y0 + screenOffset.y, color);
        DrawPixel(xc - x0 + screenOffset.x, yc + y0 + screenOffset.y, color);
        DrawPixel(xc + x0 + screenOffset.x, yc - y0 + screenOffset.y, color);
        DrawPixel(xc - x0 + screenOffset.x, yc - y0 + screenOffset.y, color);
        DrawPixel(xc + y0 + screenOffset.x, yc + x0 + screenOffset.y, color);
        DrawPixel(xc - y0 + screenOffset.x, yc + x0 + screenOffset.y, color);
        DrawPixel(xc + y0 + screenOffset.x, yc - x0 + screenOffset.y, color);
        DrawPixel(xc - y0 + screenOffset.x, yc - x0 + screenOffset.y, color);
    }
}