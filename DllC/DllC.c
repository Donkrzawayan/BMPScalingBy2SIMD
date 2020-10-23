#include "pch.h"
#include <stdint.h> /* int32_t, uint8_t */
#include <string.h> /* memcpy */
#include <math.h>

void fun(uint8_t *dest, uint8_t *src, int32_t sourceWidth, unsigned size) {
	int32_t destWidth = 2 * sourceWidth;
	uint8_t *tempSrc, *tempDest;
	for (tempSrc = src, tempDest = dest; tempSrc < src + size; tempDest += destWidth) {
		do {
			/* 3 - one pixel, three subpixels */
			memcpy(tempDest, tempSrc, 3);
			memcpy(tempDest + 3, tempSrc, 3);
			tempSrc += 3;
			tempDest += 2 * 3;
		} while ((tempSrc - src) % sourceWidth);

		memcpy(tempDest, tempDest - destWidth, destWidth);
	}
}
