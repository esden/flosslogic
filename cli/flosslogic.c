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
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <flosslogic.h>
#include "common.h"

/* Command line options */
int verbose = 0;
uint64_t samplerate = 0;
uint64_t numsamples = 0;
char *devicestring;
char *outfile;
char *outformat = "binary";

int main(int argc, char *argv[])
{
	int ret, hw;
	struct flosslogic_context ctx;
	uint8_t *sample_buffer;

	handle_cmdline_options(argc, argv);

	if ((ret = flosslogic_init(&ctx)) < 0) {
		fprintf(stderr, "Error initializing flosslogic (%d).\n", ret);
		return EXIT_FAILURE;
	}

	if ((ret = flosslogic_scan_for_devices(&ctx)) < 0) {
		fprintf(stderr, "Error scanning for devices (%d).\n", ret);
		return EXIT_FAILURE;
	}

	hw = ret;

	if (verbose)
		printf("Found supported logic analyzer #%d.\n", hw);

	if ((ret = flosslogic_hw_init(hw, &ctx)) < 0) {
		fprintf(stderr, "Error initializing device %d (%d).\n",
				hw, ret);
		return EXIT_FAILURE;
	}

	if (verbose)
		printf("Logic analyzer initialized successfully.\n");

	sample_buffer = flosslogic_hw_get_samples(hw, &ctx, numsamples,
					          samplerate, 3000);
	if (sample_buffer == NULL) {
		fprintf(stderr, "Error getting samples from device %d.\n", hw);
		return EXIT_FAILURE;
	}

	if (verbose)
		printf("Logic analyzer samples acquired successfully.\n");

	if (!strcmp(outformat, "binary")) {
		if (outfile == NULL)
			outfile = "flosslogic_binary.dat";
		/* TODO: Error handling. */
		output_binary(sample_buffer, numsamples, outfile, &ctx);
	} else if (!strcmp(outformat, "gnuplot")) {
		if (outfile == NULL)
			outfile = "flosslogic_gnuplot.dat";
		output_gnuplot(sample_buffer, numsamples, &ctx);
	} else if (!strcmp(outformat, "vcd")) {
		if (outfile == NULL)
			outfile = "flosslogic.vcd";
		output_vcd(sample_buffer, numsamples, outfile, &ctx);
	}


	flosslogic_hw_shutdown(hw, &ctx);

	if (verbose)
		printf("Logic analyzer shutdown completed successfully.\n");

	return EXIT_SUCCESS;
}
