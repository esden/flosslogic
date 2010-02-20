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
	return flosslogic_usb_init(ctx, 1, 0, 2);
}

static uint16_t samplerate_config_value(uint64_t samplerate)
{
	switch (samplerate) {
	case 24000000:
		return 0x01 | 0xee;
		break;
	case 16000000:
		return 0x02 | 0xee;
		break;
	case 15000000:
		return 0x01 | 0xae; /* AE */
		break;
	case 12000000:
		return 0x03 | 0xee;
		break;
	case 10000000:
		return 0x02 | 0xae; /* AE */
		break;
	case 8000000:
		return 0x05 | 0xee;
		break;
	case 6000000:
		return 0x07 | 0xee;
		break;
	case 5000000:
		return 0x05 | 0xae; /* AE */
		break;
	case 4000000:
		return 0x0b | 0xee;
		break;
	case 3000000:
		return 0x0f | 0xee;
		break;
	case 2500000:
		return 0x0b | 0xae; /* AE */
		break;
	case 2000000:
		return 0x17 | 0xee;
		break;
	case 1500000:
		return 0x1f | 0xee;
		break;
	case 1000000:
		return 0x2f | 0xee;
		break;
	case 800000:
		return 0x3b | 0xee;
		break;
	case 750000:
		return 0x3f | 0xee;
		break;
	case 600000:
		return 0x4f | 0xee;
		break;
	case 500000:
		return 0x5f | 0xee;
		break;
	case 400000:
		return 0x77 | 0xee;
		break;
	case 300000:
		return 0x9f | 0xee;
		break;
	case 250000:
		return 0xbf | 0xee;
		break;
	case 200000:
		return 0xef | 0xee;
		break;
	case 150000:
		return 0xc7 | 0xae; /* AE */
		break;
	case 120000:
		return 0xf9 | 0xae; /* AE */
		break;
	}

	/* Any other value yields an error. */
	return 0xff | 0xff;
}

uint8_t *hw_lps_get_samples(struct flosslogic_context *ctx,
			    uint64_t numsamples, uint64_t samplerate,
			    int timeout)
{
	int ret, bufsize;
	uint64_t i;
	char cmdbuf[5];
	uint8_t *sample_buffer;

	/* TODO */
	samplerate = samplerate;

	/* FIXME: Don't hardcode sample rate. */
	cmdbuf[0] = 0x02;
	cmdbuf[1] = 0xa0;
	cmdbuf[2] = 0xc0;
	cmdbuf[3] = 0xff;
	cmdbuf[4] = 0x0c;
	ret = usb_interrupt_write(ctx->devhandle, 0x01, cmdbuf, 5, timeout);

	bufsize = numsamples * (ctx->la->numchannels / 8);
	sample_buffer = malloc(bufsize);
	/* TODO: Better error handling? */
	if (sample_buffer == NULL)
		return NULL;

	for (i = 0; (int)i < bufsize; i += 512) {
		/* 0x86 == EP 6, IN direction */
		ret = usb_block_read(ctx->devhandle, 0x86,
			       (char *)(sample_buffer + i), 512, timeout);
	}

	/* TODO: Shutdown command? */

	return sample_buffer;
}

int hw_lps_shutdown(struct flosslogic_context *ctx)
{
	/* TODO: Don't hardcode the interface number here. */
	return flosslogic_usb_shutdown(ctx, 0);
}
