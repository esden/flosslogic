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
#include <getopt.h>
#include <flosslogic.h>

#define PROGRAM_NAME	"flosslogic"
#define PROGRAM_VERSION	"0.1"

#define USAGE "Usage: %s [-n] [-s] [-V] [-v] [-h]\n\n\
  -n | --numsamples      Number of samples to acquire\n\
  -s | --samplerate      Sample rate to use (in Msps)\n\
  -V | --verbose         Verbose mode\n\
  -v | --version         Show the program version\n\
  -h | --help            Show a short help text\n\n"

/* Command line options */
int verbose = 0;
uint8_t samplerate_value = 0;
uint64_t num_samples = 0;

static void handle_cmdline_options(int argc, char *argv[])
{
	int opt, option_index;

	static const struct option long_options[] = {
		{"numsamples",	required_argument,	NULL,	'n'},
		{"samplerate",	required_argument,	NULL,	's'},
		{"verbose",	no_argument,		NULL,	'V'},
		{"version",	no_argument,		NULL,	'v'},
		{"help",	no_argument,		NULL,	'h'},
		{0, 0, 0, 0}
	};

	while ((opt = getopt_long(argc, argv, "n:s:Vvh",
				  long_options, &option_index)) >= 0) {
		switch (opt) {
		case 'n':
			/* TODO */
			break;
		case 's':
			/* TODO */
			break;
		case 'V':
			verbose = 1;
			break;
		case 'v':
			printf("%s %s\n", PROGRAM_NAME, PROGRAM_VERSION);
			exit(EXIT_SUCCESS);
			break;
		case 'h':
			printf(USAGE, PROGRAM_NAME);
			exit(EXIT_SUCCESS);
			break;
		default:
			/* Unknown option. */
			exit(EXIT_FAILURE);
			break;
		}
	}
}

int main(int argc, char *argv[])
{
	handle_cmdline_options(argc, argv);

	return EXIT_SUCCESS;
}
