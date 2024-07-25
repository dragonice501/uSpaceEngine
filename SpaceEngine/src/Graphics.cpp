#include "Graphics.h"
#include "Font.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_sdl2.h"
#include "../imgui/imgui_impl_sdlrenderer2.h"

#include <iostream>

int Graphics::windowWidth = 0;
int Graphics::windowHeight = 0;
int Graphics::screenWidth = 0;
int Graphics::screenHeight = 0;

uint32_t* Graphics::colorBuffer = nullptr;
SDL_Texture* Graphics::colorBufferTexture = nullptr;
SDL_Window* Graphics::window = nullptr;
SDL_Renderer* Graphics::renderer = nullptr;

float Graphics::yAspect = 0;
Vec2Double Graphics::screenOffset = { 0.0f, 0.0f };
Vec2 Graphics::mousePos = { 0.0f, 0.0f };
double Graphics::screenZoom = 1.0f;
int Graphics::screenScales[SCREEN_SCALES] = { 10, 50, 100, 500, 1000, 5000, 10000, 50000, 100000, 500000  };
size_t Graphics::scalesIndex = 2;

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

    yAspect = static_cast<float>(windowHeight) / static_cast<float>(windowWidth);

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

int Graphics::GetScreenScale()
{
    for (size_t j = SCREEN_SCALES- 1; j >= 0; j--)
    {
        if (PIXELS_PER_UNIT * screenZoom >= screenScales[j])
        {
            scalesIndex = j;
            break;
        }
    }

    int scale = PIXELS_PER_UNIT / ((PIXELS_PER_UNIT * screenZoom) / screenScales[scalesIndex]);

    return scale;
}

void Graphics::AddScreenOffset(const int x, const int y)
{
    screenOffset.x += x;
    screenOffset.y += y;
}

void Graphics::IncrementZoom(const int scroll)
{
    screenZoom -= screenZoom * scroll * 0.05f;

    if (PIXELS_PER_UNIT * screenZoom <= 10)
    {
        screenZoom = 0.1f;
    }
}

bool Graphics::CircleOffScreen(const int x, const int y, const int radius)
{
    return
        (x + radius) < 0 ||
        (x - radius) > screenWidth ||
        (y + radius) < 0 ||
        (y - radius) > screenHeight;
}

bool Graphics::PlanetOffScreen(const double x, const double y, const double radius)
{
    return
        (x + radius - screenOffset.x) * screenZoom < 0 ||
        (x - radius - screenOffset.x) * screenZoom > screenWidth ||
        (y + radius - screenOffset.y) * screenZoom < 0 ||
        (y - radius - screenOffset.y) * screenZoom > screenHeight;

}

void Graphics::ClearScreen(const uint32_t& color)
{
    for (int i = 0; i < screenWidth * screenHeight; i++)
    {
        colorBuffer[i] = color;
    }

    SDL_RenderClear(renderer);
}

void Graphics::FlipScreen()
{
    uint32_t temp;
    uint32_t size = screenWidth * screenHeight;
    size_t ySize;
    size_t yMirror;
    size_t i;

    for (size_t y = 0; y < screenHeight / 2; y++)
    {
        for (size_t x = 0; x < screenWidth; x++)
        {
            ySize = y * screenWidth;
            yMirror = size - (screenWidth - x) - ySize;

            i = x + ySize;

            temp = colorBuffer[i];
            colorBuffer[i] = colorBuffer[yMirror];
            colorBuffer[yMirror] = temp;
        }
    }
}

void Graphics::PresentFrame()
{
    ImGui::Render();

    SDL_UpdateTexture(colorBufferTexture, nullptr, colorBuffer, sizeof(uint32_t) * screenWidth);
    SDL_RenderCopy(renderer, colorBufferTexture, nullptr, nullptr);

    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);

    SDL_RenderPresent(renderer);
}

void Graphics::DrawPixel(const int& x, const int& y, const uint32_t& color)
{
    if (x < 0 || x >= screenWidth || y < 0 || y >= screenHeight) return;

    colorBuffer[x + y * screenWidth] = color;
}

void Graphics::DrawLine(const int x0, const int y0, const int x1, const int y1, const uint32_t color)
{
    if ((x0 < 0 || x0 > screenWidth) &&
        (y0 < 0 || y0 > screenHeight) &&
        (x1 < 0 || x1 > screenWidth) &&
        (y0 < 0 || y0 > screenHeight)) return;

    float x = x0;
    float y = y0;
    int deltaX = x1 - x0;
    int deltaY = y1 - y0;

    int steps = abs(deltaX) > abs(deltaY) ? abs(deltaX) : abs(deltaY);

    float xIncrement = deltaX / (float)steps;
    float yIncrement = deltaY / (float)steps;

    for (int i = 0; i < steps; i++)
    {
        DrawPixel(static_cast<int>(x + screenOffset.x), static_cast<int>(y + screenOffset.y), color);
        x += xIncrement;
        y += yIncrement;

        //if((x < 0 || x > screenWidth) && ( y < 0 || y > screenHeight)) break;
    }
}

void Graphics::DrawULine(float u)
{
    int x = u * screenWidth;
    for (size_t i = 0; i < Graphics::screenHeight; i++)
    {
        colorBuffer[x + i * screenWidth] = 0xFF333333;
    }
}

void Graphics::DrawXYAxis(float u, float v)
{
    // Draw X Axis
    if (!(v < 0.0f || v > 1.0f))
    {
        int y = v * (screenHeight - 1);
        for (size_t i = 0; i < Graphics::screenWidth; i++)
        {
            colorBuffer[i + y * screenWidth] = 0xFFFF0000;
        }
    }

    // Draw Y Axis
    if (!(u < 0.0f || u > 1.0f))
    {
        int x = u * (screenWidth - 1);
        for (size_t i = 0; i < Graphics::screenHeight; i++)
        {
            colorBuffer[x + i * screenWidth] = 0xFF00FF00;
        }
    }
}

void Graphics::DrawGrid(const uint32_t color)
{
    for (int y = 0; y < screenHeight; y++)
    {
        for (int x = 0; x < screenWidth; x++)
        {
            if (x % PIXELS_PER_UNIT == 0 || y % PIXELS_PER_UNIT == 0)
                DrawPixel(x, y, color);
        }
    }
}

void Graphics::DrawGrid(int xOffset, int yOffset, int size)
{
    int mod = size;

    int xOff = xOffset % size;
    int yOff = yOffset % size;

    for (int y = 0; y < screenHeight; y++)
    {
        for (int x = 0; x < screenWidth; x++)
        {
            if ((x - xOff) % mod == 0 || (y - yOff) % mod == 0)
                DrawPixel(x, y, 0xFF333333);
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

void Graphics::DrawCircle(const int x, const int y, const int radius, const float angle, const uint32_t color)
{
    if (PlanetOffScreen(x, y, radius)) return;

    int x0 = 0;
    int y0 = radius;
    int p0 = 3 - (2 * radius);

    DisplayBresenhamCircle(x, y, x0, y0, color, false);

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

        DisplayBresenhamCircle(x, y, x0, y0, color, false);
    }

    //DrawLine(x, y, x + cos(angle) * radius, y + sin(angle) * radius, color, lockToScreen);
}

void Graphics::DrawFillCircle(const double x, const double y, const double radius, const uint32_t color)
{
    double u = x / screenZoom + screenOffset.x;
    double v = y / screenZoom + screenOffset.y;

    if (CircleOffScreen(u, v, radius)) return;

    double rSqr = radius * radius;

    double xD = 0;
    double yD = 0;

    // Top Left Quarter
    int xStart;
    int yStart = v <= 0 ? 0 : v >= screenHeight ? screenHeight : v;
    yD = abs(v - yStart);

    while (yStart < screenHeight)
    {
        xStart = u <= 0 ? 0 : u >= screenWidth ? screenWidth - 1 : u;
        if (xStart == 0) break;

        xD = abs(u - xStart);

        while (xD * xD + yD * yD < rSqr)
        {
            colorBuffer[xStart + yStart * screenWidth] = color;

            xStart--;
            xD++;
            if (xStart < 0) break;
        }

        yStart++;
        yD++;
    }

    // Bottom Left Quarter
    yStart = v <= 0 ? 0 : v >= screenHeight ? screenHeight - 1 : v;
    yD = abs(v - yStart);

    while (yStart > 0)
    {
        xStart = u <= 0 ? 0 : u >= screenWidth ? screenWidth - 1 : u;
        if (xStart == 0) break;

        xD = abs(u - xStart);

        while (xD * xD + yD * yD < rSqr)
        {
            colorBuffer[xStart + yStart * screenWidth] = color;

            xStart--;
            xD++;
            if (xStart < 0) break;
        }

        yStart--;
        yD++;
    }

    // Top Right Quarter
    yStart = v <= 0 ? 0 : v >= screenHeight ? screenHeight : v;
    yD = abs(v - yStart);

    while (yStart < screenHeight)
    {
        xStart = u <= 0 ? 0 : u >= screenWidth ? screenWidth : u;
        if (xStart == screenWidth) break;

        xD = abs(u - xStart);

        while (xD * xD + yD * yD < rSqr)
        {
            colorBuffer[xStart + yStart * screenWidth] = color;

            xStart++;
            xD++;
            if (xStart >= screenWidth) break;
        }

        yStart++;
        yD++;
    }

    // Bottom Right Quarter
    yStart = v <= 0 ? 0 : v >= screenHeight ? screenHeight - 1 : v;
    yD = abs(v - yStart);

    while (yStart > 0)
    {
        xStart = u <= 0 ? 0 : u >= screenWidth ? screenWidth : u;
        if (xStart == screenWidth) break;

        xD = abs(u - xStart);

        while (xD * xD + yD * yD < rSqr)
        {
            colorBuffer[xStart + yStart * screenWidth] = color;

            xStart++;
            xD++;
            if (xStart >= screenWidth) break;
        }

        yStart--;
        yD++;
    }
}

void Graphics::DrawPlanet(const double x, const double y, const double radius, const uint32_t color)
{
    double u = x / screenZoom + screenOffset.x;
    double v = y / screenZoom + screenOffset.y;
    double r = radius / screenZoom;

    if (PlanetOffScreen(u, v, r)) return;

    double rSqr = r * r;

    double xD = 0;
    double yD = 0;

    // Top Left Quarter
    int xStart;
    int yStart = v <= 0 ? 0 : v >= screenHeight ? screenHeight : v;
    yD = abs(v - yStart);

    while (yStart < screenHeight)
    {
        xStart = u <= 0 ? 0 : u >= screenWidth ? screenWidth - 1 : u;
        if (xStart == 0) break;

        xD = abs(u - xStart);

        while (xD * xD + yD * yD < rSqr)
        {
            colorBuffer[xStart + yStart * screenWidth] = color;

            xStart--;
            xD++;
            if (xStart < 0) break;
        }

        yStart++;
        yD++;
    }

    // Bottom Left Quarter
    yStart = v <= 0 ? 0 : v >= screenHeight ? screenHeight - 1 : v;
    yD = abs(v - yStart);

    while (yStart > 0)
    {
        xStart = u <= 0 ? 0 : u >= screenWidth ? screenWidth - 1 : u;
        if (xStart == 0) break;

        xD = abs(u - xStart);

        while (xD * xD + yD * yD < rSqr)
        {
            colorBuffer[xStart + yStart * screenWidth] = color;

            xStart--;
            xD++;
            if (xStart < 0) break;
        }

        yStart--;
        yD++;
    }

    // Top Right Quarter
    yStart = v <= 0 ? 0 : v >= screenHeight ? screenHeight : v;
    yD = abs(v - yStart);

    while (yStart < screenHeight)
    {
        xStart = u <= 0 ? 0 : u >= screenWidth ? screenWidth : u;
        if (xStart == screenWidth) break;

        xD = abs(u - xStart);

        while (xD * xD + yD * yD < rSqr)
        {
            colorBuffer[xStart + yStart * screenWidth] = color;

            xStart++;
            xD++;
            if (xStart >= screenWidth) break;
        }

        yStart++;
        yD++;
    }

    // Bottom Right Quarter
    yStart = v <= 0 ? 0 : v >= screenHeight ? screenHeight - 1 : v;
    yD = abs(v - yStart);

    while (yStart > 0)
    {
        xStart = u <= 0 ? 0 : u >= screenWidth ? screenWidth : u;
        if (xStart == screenWidth) break;

        xD = abs(u - xStart);

        while (xD * xD + yD * yD < rSqr)
        {
            colorBuffer[xStart + yStart * screenWidth] = color;

            xStart++;
            xD++;
            if (xStart >= screenWidth) break;
        }

        yStart--;
        yD++;
    }
}

void Graphics::DrawPolygon(const int& x, const int& y, const std::vector<Vec2>& vertices, const uint32_t& color, const bool& lockToScreen)
{
    Vec2 current = vertices[0];
    Vec2 previous = vertices[vertices.size() - 1];

    DrawLine(previous.x, previous.y, current.x, current.y, 0xFFFFFFFF);

    for (int i = 1; i < vertices.size(); i++)
    {
        previous = current;
        current = vertices[i];
        DrawLine(previous.x, previous.y, current.x, current.y, color);
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