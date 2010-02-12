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
#include <string.h>
#include <stdint.h>
#include <flosslogic.h>

/* TODO: Don't hardcode number of channels to 8. */
void output_gnuplot(uint8_t *buf, uint64_t numsamples,
		    struct flosslogic_context *ctx)
{
	uint64_t i, j, count = 1;

	for (i = 0; i < numsamples; i++) {
		/* The first column is a counter (needed for gnuplot). */
		printf("%lld\t\t", count++);

		/* The next columns are the values of all channels. */
		for (j = 0; j < ctx->la->numchannels; j++) {
			if (j < 8) {
				printf("%d ", ((buf[i] & (1 << j))) >> j);
			} else if (j >= 8 && j < 16) {
				printf("%d ", ((buf[i + 1]
				       & (1 << (j - 8)))) >> (j - 8));
			}
		}
		printf("\n");
	}
}

int output_binary(uint8_t *buf, uint64_t numsamples, const char *filename,
		  struct flosslogic_context *ctx)
{
	FILE *f;

	if (buf == NULL)
		return -1;
	if (filename == NULL)
		return -2;
	if (ctx == NULL)
		return -3;
	if (ctx->la == NULL)
		return -4;

	/* If filename is "-", output to stdout. */
	if (strlen(filename) == 1 && !strncmp(filename, "-", 1)) {
		f = stdout;
	} else {
		/* TODO: Error handling. */
		f = fopen(filename, "wb");
	}

	/* TODO: Error handling. */
	fwrite(buf, numsamples * (ctx->la->numchannels / 8), 1, f);

	/* TODO: Error handling. */
	fclose(f);

	return 0;
}
