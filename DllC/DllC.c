#include "pch.h"
#include <stdint.h> /* int32_t, uint8_t */
#include <string.h> /* memcpy */

uint8_t *fun(uint8_t *Dest, uint8_t *Source, int32_t sourceWidth, unsigned size) {
	int32_t destWidth = 2 * sourceWidth;
	unsigned i;
	for (i = 0; i < size; i += 3) {
		if (!(i % sourceWidth) && i)
			Dest += destWidth;

		uint8_t *tempSource = Source + i;
		uint8_t *tempDest = Dest + 2 * i;
		uint8_t *downTempDest = tempDest + destWidth;
		
		/* 3 - one pixel, three subpixels */
		memcpy(tempDest, tempSource, 3 * sizeof(uint8_t));
		memcpy(tempDest + 3, tempSource, 3 * sizeof(uint8_t));
		memcpy(downTempDest, tempSource, 3 * sizeof(uint8_t));
		memcpy(downTempDest + 3, tempSource, 3 * sizeof(uint8_t));
	}

	return Dest;
}
