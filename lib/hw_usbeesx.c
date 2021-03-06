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

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <usb.h>
#include <flosslogic.h>
#include "common.h"

int hw_usbeesx_init(struct flosslogic_context *ctx)
{
	return flosslogic_usb_init(ctx, 1, 0, 2);
}

static uint8_t samplerate_config_value(uint64_t samplerate)
{
	switch (samplerate) {
	case 24000000:
		return 0x01;
		break;
	case 16000000:
		return 0x02;
		break;
	case 12000000:
		return 0x03;
		break;
	case 8000000:
		return 0x05;
		break;
	case 6000000:
		return 0x07;
		break;
	case 4000000:
		return 0x0b;
		break;
	case 3000000:
		return 0x0f;
		break;
	case 2000000:
		return 0x17;
		break;
	case 1000000:
		return 0x2f;
		break;
	}

	/* Any other value yields an error. */
	return 0xff;
}

uint8_t *hw_usbeesx_get_samples_init(struct flosslogic_context *ctx,
		uint64_t numsamples, uint64_t samplerate, int timeout)
{
	int ret, bufsize;
	char cmdbuf[2];
	uint8_t *sample_buffer;

	cmdbuf[0] = 0x01;
	/* TODO: Error handling. */
	ret = usb_interrupt_write(ctx->devhandle, 0x81, cmdbuf, 1, timeout);

	cmdbuf[0] = 0x55;
	/* TODO: Error handling. */
	ret = usb_interrupt_write(ctx->devhandle, 0x81, cmdbuf, 1, timeout);

	/* TODO */

	cmdbuf[0] = 0x01;
	cmdbuf[1] = samplerate_config_value(samplerate); /* TODO: Error h. */
	if (cmdbuf[1] == (char)0xff) {
		/* Handle hw_usbeesx_shutdown() errors. */
		hw_usbeesx_shutdown(ctx);
		/* TODO: Set error code. */
		return NULL;
	}
	/* TODO: Error handling. */
	ret = usb_interrupt_write(ctx->devhandle, 0x01, cmdbuf, 2, timeout);

	bufsize = numsamples * (ctx->la->numchannels / 8);
	sample_buffer = malloc(bufsize);
	/* TODO: Better error handling? */
	if (sample_buffer == NULL)
		return NULL;

	return sample_buffer;
}

int hw_usbeesx_get_samples_chunk(struct flosslogic_context *ctx,
		uint8_t *buf, uint64_t chunksize, uint64_t pos, int timeout)
{
	/* 0x86 == EP 6, IN direction */
	usb_block_read(ctx->devhandle, 0x86,
		       (char *)(buf + pos), chunksize, timeout);

	/* TODO: Error handling. */
	/* TODO: Guaranteed delivery of chunksize. */

	return 0;
}

int hw_usbeesx_get_samples_shutdown(struct flosslogic_context *ctx,
				    int timeout)
{
	/* TODO: Shutdown command? */

	/* QUICK HACK: Prevent compiler warnings. */
	ctx = ctx;
	timeout = timeout;

	return 0;
}

uint8_t *hw_usbeesx_get_samples(struct flosslogic_context *ctx,
		uint64_t numsamples, uint64_t samplerate, int timeout)
{
	int ret, bufsize;
	uint64_t i;
	char cmdbuf[2];
	uint8_t *sample_buffer;

	cmdbuf[0] = 0x01;
	/* TODO: Error handling. */
	ret = usb_interrupt_write(ctx->devhandle, 0x81, cmdbuf, 1, timeout);

	cmdbuf[0] = 0x55;
	/* TODO: Error handling. */
	ret = usb_interrupt_write(ctx->devhandle, 0x81, cmdbuf, 1, timeout);

	/* TODO */

	cmdbuf[0] = 0x01;
	cmdbuf[1] = samplerate_config_value(samplerate); /* TODO: Error h. */
	if (cmdbuf[1] == (char)0xff) {
		/* Handle hw_usbeesx_shutdown() errors. */
		hw_usbeesx_shutdown(ctx);
		/* TODO: Set error code. */
		return NULL;
	}
	/* TODO: Error handling. */
	ret = usb_interrupt_write(ctx->devhandle, 0x01, cmdbuf, 2, timeout);

	bufsize = numsamples * (ctx->la->numchannels / 8);
	sample_buffer = malloc(bufsize);
	/* TODO: Better error handling? */
	if (sample_buffer == NULL)
		return NULL;

	for (i = 0; (int)i < bufsize; i += 512) {
		/* 0x86 == EP 6, IN direction */
		usb_block_read(ctx->devhandle, 0x86,
			       (char *)(sample_buffer + i), 512, timeout);
	}

	/* TODO: Shutdown command? */

	return sample_buffer;
}

int hw_usbeesx_shutdown(struct flosslogic_context *ctx)
{
	/* TODO: Don't hardcode the interface number here. */
	return flosslogic_usb_shutdown(ctx, 0);
}
