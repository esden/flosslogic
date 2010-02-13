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

int hw_lps_init(struct flosslogic_context *ctx)
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

uint8_t *hw_lps_get_samples(struct flosslogic_context *ctx,
			    uint64_t numsamples, uint64_t samplerate)
{
	/* Ugly hack. */
	ctx = ctx;
	numsamples = numsamples;
	samplerate = samplerate;

	/* TODO */
	return NULL;
}

int hw_lps_shutdown(struct flosslogic_context *ctx)
{
	if (ctx == NULL)
		return 0;

	/* TODO: Don't hardcode the interface number here. */
	if (ctx->devhandle != NULL) {
		if (usb_release_interface(ctx->devhandle, 0) < 0)
			return -1;
	}

	/* TODO: What about ctx->usb_dev? */

	if (ctx->devhandle != NULL) {
		if (usb_close(ctx->devhandle) < 0)
			return -1;
	}

	return 0;
}
