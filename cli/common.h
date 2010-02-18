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

#ifndef FLOSSLOGIC_COMMON_H
#define FLOSSLOGIC_COMMON_H

#include <stdint.h>

#define PROGRAM_NAME	"flosslogic"
#define PROGRAM_VERSION	"0.1"

#define USAGE "Usage: %s [-d] [-n] [-s] [-V] [-v] [-h]\n\n\
  -d | --device          Device to use (usbeesx, lps, logic)\n\
  -n | --numsamples      Number of samples to acquire\n\
  -s | --samplerate      Sample rate to use (in sps)\n\
  -o | --outfile         Output filename or '-' for stdout\n\
  -V | --verbose         Verbose mode\n\
  -v | --version         Show the program version\n\
  -h | --help            Show a short help text\n\n"

/* Command line options */
extern int verbose;
extern uint64_t samplerate;
extern uint64_t numsamples;
extern char *devicestring;
extern char *outfile;
extern char *outformat;

/* cmdline.c */
void handle_cmdline_options(int argc, char *argv[]);

/* output.c */
void output_gnuplot(uint8_t *buf, uint64_t local_numsamples,
		    struct flosslogic_context *ctx);
int output_binary(uint8_t *buf, uint64_t local_numsamples,
		  const char *filename, struct flosslogic_context *ctx);

#endif
