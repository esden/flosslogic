/*
 * This file is part of the flosslogic project.
 *
 * Copyright (C) 2010 Uwe Hermann <uwe@hermann-uwe.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

#include <stdio.h>
#include <stdint.h>

/* TODO: Don't hardcode number of channels to 8. */
void output_gnuplot(uint8_t *buf, uint64_t numsamples)
{
	uint64_t i, j, count = 1;

	for (i = 0; i < numsamples; i++) {
		/* The first column is a counter (needed for gnuplot). */
		printf("%lld\t\t", count++);

		/* The next 8 columns are the values of all 8 channels. */
		for (j = 0; j < 8; j++)
			printf("%d ", ((buf[i] & (1 << j))) >> j);
		printf("\n");
	}
}
