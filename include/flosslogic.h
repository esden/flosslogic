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

#ifndef LIBFLOSSLOGIC_FLOSSLOGIC_H
#define LIBFLOSSLOGIC_FLOSSLOGIC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <usb.h>

struct flosslogic_context {
	struct usb_device *usb_dev;
	usb_dev_handle *devhandle;
	struct logic_analyzer *la;
};

struct logic_analyzer {
	const char *shortname;
	uint16_t vid;
	uint16_t pid;
	uint8_t numchannels;
	int (*init) (struct flosslogic_context *ctx);
	/* TODO: Remove later? */
	uint8_t * (*get_samples) (struct flosslogic_context *ctx,
		uint64_t numsamples, uint64_t samplerate, int timeout);
	uint8_t * (*hw_get_samples_init) (struct flosslogic_context *ctx,
		uint64_t numsamples, uint64_t samplerate, int timeout);
	int (*hw_get_samples_chunk) (struct flosslogic_context *ctx,
		uint8_t *buf, uint64_t chunksize, uint64_t pos, int timeout);
	int (*hw_get_samples_shutdown) (struct flosslogic_context *ctx,
					int timeout);
	int (*shutdown) (struct flosslogic_context *ctx);
};

/* main.c */
int flosslogic_hw_init(int hw, struct flosslogic_context *ctx);
/* TODO: Remove later? */
uint8_t *flosslogic_hw_get_samples(int hw, struct flosslogic_context *ctx,
		uint64_t numsamples, uint64_t samplerate, int timeout);
uint8_t *flosslogic_hw_get_samples_init(struct flosslogic_context *ctx,
		uint64_t numsamples, uint64_t samplerate, int timeout);
int flosslogic_hw_get_samples_chunk(struct flosslogic_context *ctx,
		uint8_t *buf, uint64_t chunksize, uint64_t pos, int timeout);
int flosslogic_hw_get_samples_shutdown(struct flosslogic_context *ctx,
				      int timeout);
int flosslogic_hw_shutdown(int hw, struct flosslogic_context *ctx);
int flosslogic_is_supported_la(const char *la_string);

/* libusb.c */
int flosslogic_init(struct flosslogic_context *ctx);
int flosslogic_scan_for_devices(struct flosslogic_context *ctx);
int flosslogic_usb_init(struct flosslogic_context *ctx, int configuration,
			int interface, int altinterface);
int flosslogic_usb_shutdown(struct flosslogic_context *ctx, int interface);

/* hw_usbeesx.c */
int hw_usbeesx_init(struct flosslogic_context *ctx);
/* TODO: Remove later? */
uint8_t *hw_usbeesx_get_samples(struct flosslogic_context *ctx,
		uint64_t numsamples, uint64_t samplerate, int timeout);
uint8_t *hw_usbeesx_get_samples_init(struct flosslogic_context *ctx,
		uint64_t numsamples, uint64_t samplerate, int timeout);
int hw_usbeesx_get_samples_chunk(struct flosslogic_context *ctx,
		uint8_t *buf, uint64_t chunksize, uint64_t pos, int timeout);
int hw_usbeesx_get_samples_shutdown(struct flosslogic_context *ctx,
				    int timeout);
int hw_usbeesx_shutdown(struct flosslogic_context *ctx);

/* hw_lps.c */
int hw_lps_init(struct flosslogic_context *ctx);
uint8_t *hw_lps_get_samples(struct flosslogic_context *ctx,
		uint64_t numsamples, uint64_t samplerate, int timeout);
int hw_lps_shutdown(struct flosslogic_context *ctx);

static const struct logic_analyzer flosslogic_logic_analyzers[] = {
	{
		"usbeesx",
		0x08a9,
		0x0009,
		8,
		hw_usbeesx_init,
		hw_usbeesx_get_samples,
		hw_usbeesx_get_samples_init,
		hw_usbeesx_get_samples_chunk,
		hw_usbeesx_get_samples_shutdown,
		hw_usbeesx_shutdown,
	},
	{
		"lps",
		0x16d0,
		0x0498,
		16,
		hw_lps_init,
		hw_lps_get_samples,
		NULL,
		NULL,
		NULL,
		hw_lps_shutdown,
	},
	{
		"logic",
		0x0925,
		0x3881,
		8,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
	},
	{
		NULL,
		0,
		0,
		0,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
	},
};

#ifdef __cplusplus
}
#endif

#endif
