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
#include <flosslogic.h>
#include "common.h"

/**
 * Initialize libflosslogic.
 *
 * @param ctx A pointer to a flosslogic_context.
 * @return 0 if the init was successful, negative values upon errors.
 */
int flosslogic_init(struct flosslogic_context *ctx)
{
	if (ctx == NULL)
		return -1;

	ctx->num_devices_found = 0;
	/* TODO */

	/* TODO: Error checking. */
	usb_init();
	usb_find_busses();
	usb_find_devices();

	return 0;
}
 
/**
 * Check if the given USB device is a supported logic analyzer.
 *
 * @param dev The USB device to check.
 * @return The index of the supported logic analyzer that was found, or
 *         -1 in case none is found.
 */
static int is_supported_device(struct usb_device *dev)
{
	int i = 0;
	struct logic_analyzer la;

	while (1) {
		la = flosslogic_logic_analyzers[i++];
		if (la.shortname == NULL)
			break;
		if (dev->descriptor.idVendor != la.vid
		    || dev->descriptor.idProduct != la.pid)
			continue;

		return (i - 1);
	}

	return -1;
}

/**
 * Scan for supported USB-based logic analyzers.
 *
 * @param ctx A flosslogic_context struct.
 * @return TODO
 */
int flosslogic_scan_for_devices(struct flosslogic_context *ctx)
{
	struct usb_bus *bus;
	struct usb_device *dev;
	int ret;

	if (ctx == NULL)
		return -1;

	for (bus = usb_get_busses(); bus; bus = bus->next) {
		for (dev = bus->devices; dev; dev = dev->next) {
			if ((ret = is_supported_device(dev)) >= 0) {
				/* TODO: Support for multiple devices. */
				ctx->usb_dev = dev;
				return ret;
			}
		}
	}

	return -2;
}

/* FIXME */
int usb_block_read(usb_dev_handle *devhandle, int endpoint, char *buf,
		   size_t nbytes)
{
	ssize_t rv;
	size_t chunk_size = nbytes;
	size_t left = nbytes;
	size_t timeout = 3000; /* FIXME */

	while (left) {
		size_t block_size = (left > chunk_size) ? chunk_size : left;

		rv = usb_bulk_read(devhandle, endpoint, buf, block_size,
				   timeout);
		if (rv < 0)
			return -1;

		left -= rv;

		if ((size_t)rv < block_size) {
			if (rv == 0)
				return -1;
		}
	}

	return 0; /* FIXME */
}
