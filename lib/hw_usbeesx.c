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
#include <usb.h>
#include <flosslogic.h>
#include "common.h"

int hw_usbeesx_init(struct flosslogic_context *ctx)
{
	int ret;

	ctx->devhandle = usb_open(ctx->usb_dev);
	if (ctx->devhandle == NULL)
		return -1;

	ret = usb_set_configuration(ctx->devhandle, 1);
	if (ret < 0) {
		usb_close(ctx->devhandle); /* TODO? */
		return -2;
	}

	ret = usb_claim_interface(ctx->devhandle, 0);
	if (ret < 0) {
		usb_close(ctx->devhandle); /* TODO? */
		return -3;
	}

	ret = usb_set_altinterface(ctx->devhandle, 2);
	if (ret < 0) {
		usb_close(ctx->devhandle); /* TODO? */
		return -4;
	}

	return 0;
}

uint8_t *hw_usbeesx_get_samples(struct flosslogic_context *ctx,
				uint64_t numsamples, uint64_t samplerate)
{
	int ret;
	uint64_t i;
	char cmdbuf[2];
	uint8_t *sample_buffer;

	/* TODO: Don't hardcode timeout. */

	cmdbuf[0] = 0x01;
	/* TODO: Error handling. */
	ret = usb_interrupt_write(ctx->devhandle, 0x81, cmdbuf, 1, 3000);

	cmdbuf[0] = 0x55;
	/* TODO: Error handling. */
	ret = usb_interrupt_write(ctx->devhandle, 0x81, cmdbuf, 1, 3000);

	/* TODO */

	cmdbuf[0] = 0x01;
	cmdbuf[1] = samplerate; /* FIXME */
	cmdbuf[1] = 0x0b; /* TODO: Don't hardcode sample rate. */
	/* TODO: Error handling. */
	ret = usb_interrupt_write(ctx->devhandle, 0x01, cmdbuf, 2, 3000);

	/* TODO: Error handling. */
	sample_buffer = malloc(numsamples * (ctx->la->numchannels / 8));

	for (i = 0; i < numsamples * (ctx->la->numchannels / 8); i += 512) {
		/* 0x86 == EP 6, IN direction */
		usb_block_read(ctx->devhandle, 0x86,
			       (char *)(sample_buffer + i), 512);
	}

	/* TODO: Shutdown command? */

	return sample_buffer;
}

void hw_usbeesx_shutdown(struct flosslogic_context *ctx)
{
	/* TODO */

	/* Hack to prevent compiler warning. */
	ctx = ctx;
}
