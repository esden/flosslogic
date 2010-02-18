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
#include <sys/types.h>
#include <flosslogic.h>
#include "common.h"

/**
 * Initialize libflosslogic.
 *
 * @param ctx A pointer to a flosslogic_context which is to be initialized.
 * @return 0 if the init was successful, negative values upon errors.
 */
int flosslogic_init(struct flosslogic_context *ctx)
{
	int num_bus_changes, num_device_changes;

	if (ctx == NULL)
		return -1;

	ctx->usb_dev = NULL;
	ctx->devhandle = NULL;
	ctx->la = NULL;

	usb_init();

	/* Return code unused so far in this place. */
	num_bus_changes = usb_find_busses();

	/* Return code unused so far in this place. */
	num_device_changes = usb_find_devices();

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
				ctx->la = (struct logic_analyzer *)&flosslogic_logic_analyzers[ret];
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

int flosslogic_usb_init(struct flosslogic_context *ctx, int configuration,
			int interface, int altinterface)
{
	int ret;

	ctx->devhandle = usb_open(ctx->usb_dev);
	if (ctx->devhandle == NULL)
		return -1;

	ret = usb_set_configuration(ctx->devhandle, configuration);
	if (ret < 0) {
		usb_close(ctx->devhandle); /* TODO? */
		return -2;
	}

	ret = usb_claim_interface(ctx->devhandle, interface);
	if (ret < 0) {
		usb_close(ctx->devhandle); /* TODO? */
		return -3;
	}

	ret = usb_set_altinterface(ctx->devhandle, altinterface);
	if (ret < 0) {
		usb_close(ctx->devhandle); /* TODO? */
		return -4;
	}

	return 0;
}

int flosslogic_usb_shutdown(struct flosslogic_context *ctx, int interface)
{
	if (ctx == NULL)
		return 0;

	if (ctx->devhandle != NULL) {
		if (usb_release_interface(ctx->devhandle, interface) < 0)
			return -1;
	}

	/* TODO: What about ctx->usb_dev? */

	if (ctx->devhandle != NULL) {
		if (usb_close(ctx->devhandle) < 0)
			return -1;
	}

	return 0;
}
