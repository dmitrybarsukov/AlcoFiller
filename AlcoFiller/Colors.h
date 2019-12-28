#ifndef COLORS_H
#define COLORS_H

#include <Arduino.h>

namespace Color
{
	uint32_t RED = 0xFF0000;
	uint32_t ORANGE = 0xFF3300;
	uint32_t YELLOW = 0xFF7700;
	uint32_t LIME = 0xFFFF00;
	uint32_t GREEN = 0x00FF00;
	uint32_t AQUA = 0x00FFFF;
	uint32_t BLUE = 0x0000FF;
	uint32_t PINK = 0xFF00FF;
	uint32_t BLACK = 0x000000;
	uint32_t WHITE = 0xFFFFFF;

	uint32_t Interpolate(uint32_t color1, uint32_t color2, uint8_t cft)
	{
		uint32_t r1 = ((color1 >> 16) & 0xFF) * cft / 255;
		uint32_t g1 = ((color1 >> 8) & 0xFF) * cft / 255;
		uint32_t b1 = ((color1 >> 0) & 0xFF) * cft / 255;
		uint32_t r2 = ((color2 >> 16) & 0xFF) * (255 - cft) / 255;
		uint32_t g2 = ((color2 >> 8) & 0xFF) * (255 - cft) / 255;
		uint32_t b2 = ((color2 >> 0) & 0xFF) * (255 - cft) / 255;
		return ((r1 + r2) << 16) | ((g1 + g2) << 8) | ((b1 + b2) << 0);
	}
}

#endif // COLORS_H
