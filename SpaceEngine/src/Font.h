#pragma once

#include <map>
#include <array>

struct Font
{
	static int fontWidth;
	static int fontHeight;
	static int fontSpacing;

	static int GetStringFontLength(const char* string);

	static std::map<char, std::array<bool, 15>> fontMap;
};