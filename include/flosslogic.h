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

#ifndef FLOSSLOGIC_FLOSSLOGIC_H
#define FLOSSLOGIC_FLOSSLOGIC_H

#include <usb.h>

struct flosslogic_context {
	int num_devices_found;
};

struct logic_analyzer {
	const char *shortname;
	uint16_t vid;
	uint16_t pid;
};

static const struct logic_analyzer flosslogic_logic_analyzers[] = {
	{"usbeesx",	0x08a9, 0x0009},
	{"lps",		0x16d0, 0x0498},
	{"logic",	0x0925, 0x3881},
	{NULL,		0,	0},
};

/* main.c */
/* TODO */

/* libusb.c */
int flosslogic_init(struct flosslogic_context *ctx);
int flosslogic_scan_for_devices(struct flosslogic_context *ctx);

#endif
