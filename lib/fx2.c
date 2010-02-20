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

#include <usb.h>
#include <stdint.h>
#include <flosslogic.h>
#include "common.h"

int flosslogic_fx2_write_ram(usb_dev_handle *devhandle, int ram_addr,
			     uint8_t *buf, int numbytes, int timeout)
{
	/* TODO */
	/* Avoid compiler warnings. */
	devhandle = devhandle;
	ram_addr = ram_addr;
	buf = buf;
	numbytes = numbytes;
	timeout = timeout;

	return 0;
}

int flosslogic_fx2_read_ram(usb_dev_handle *devhandle, int ram_addr,
			    uint8_t *buf, int numbytes, int timeout)
{
	/* TODO */
	/* Avoid compiler warnings. */
	devhandle = devhandle;
	ram_addr = ram_addr;
	buf = buf;
	numbytes = numbytes;
	timeout = timeout;

	return 0;
}

int flosslogic_fx2_reset(usb_dev_handle *devhandle, int timeout)
{
	uint8_t b = 1;
	return flosslogic_fx2_write_ram(devhandle, 0xe600, &b, 1, timeout);
}

int flosslogic_fx2_run(usb_dev_handle *devhandle, int timeout)
{
	uint8_t b = 0;
	return flosslogic_fx2_write_ram(devhandle, 0xe600, &b, 1, timeout);
}

int flosslogic_fx2_upload_firmware(usb_dev_handle *devhandle, int ram_addr,
				   uint8_t *buf, int numbytes, int timeout)
{
	/* TODO */
	/* Avoid compiler warnings. */
	devhandle = devhandle;
	ram_addr = ram_addr;
	buf = buf;
	numbytes = numbytes;
	timeout = timeout;

	return 0;
}
