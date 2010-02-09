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
// #include <string.h>
// #include <getopt.h>
#include <flosslogic.h>
#include "common.h"

/* Command line options */
int verbose = 0;
uint8_t samplerate = 0;
uint64_t numsamples = 0;
char *devicestring;

int main(int argc, char *argv[])
{
	int ret;
	struct flosslogic_context ctx;

	handle_cmdline_options(argc, argv);

	if ((ret = flosslogic_init(&ctx)) < 0) {
		fprintf(stderr, "Error initializing flosslogic (%d).\n", ret);
		return EXIT_FAILURE;
	}

	if ((ret = flosslogic_scan_for_devices(&ctx)) < 0) {
		fprintf(stderr, "Error scanning for devices (%d).\n", ret);
		return EXIT_FAILURE;
	}

	printf("Found %d supported logic analyzers.\n", ret);

	return EXIT_SUCCESS;
}
