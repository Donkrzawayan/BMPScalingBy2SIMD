#include "pch.h"
#include <stdint.h>
#include <string.h>

uint8_t *fun(uint8_t *Dest, uint8_t *Source, int32_t pixelSourceWidth, unsigned pixelSize) {
	//3 - one pixel, three subpixels
	int32_t subpixelDestWidth = 3 * 2 * pixelSourceWidth;
	unsigned subpixelI;
	for (subpixelI = 0; subpixelI < 3 * pixelSize; subpixelI += 3) {
		if (subpixelI % (pixelSourceWidth * 3) == 0 && subpixelI != 0)
			Dest += subpixelDestWidth;

		uint8_t *tempSource = Source + subpixelI;
		uint8_t *tempDest = Dest + 2 * subpixelI;
		uint8_t *downTempDest = tempDest + subpixelDestWidth;
		
		memcpy(tempDest, tempSource, 3 * sizeof(uint8_t));
		memcpy(tempDest + 3, tempSource, 3 * sizeof(uint8_t));
		memcpy(downTempDest, tempSource, 3 * sizeof(uint8_t));
		memcpy(downTempDest + 3, tempSource, 3 * sizeof(uint8_t));
	}

	return Dest;
}
