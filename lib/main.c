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
#include <strings.h>
#include <usb.h>
#include <flosslogic.h>

int flosslogic_hw_init(int hw, struct flosslogic_context *ctx)
{
	return flosslogic_logic_analyzers[hw].init(ctx);
}

uint8_t *flosslogic_hw_get_samples(int hw, struct flosslogic_context *ctx,
				   uint64_t numsamples, uint64_t samplerate)
{
	return flosslogic_logic_analyzers[hw].get_samples(ctx, numsamples,
							  samplerate);
}

int flosslogic_hw_shutdown(int hw, struct flosslogic_context *ctx)
{
	return flosslogic_logic_analyzers[hw].shutdown(ctx);
}

/**
 * Check if the given string is the shortname of a supported LA.
 *
 * The check is done case-insensitive.
 *
 * @param la_string The string to check.
 * @return 1 if la_string is the shortname of a supported LA, 0 otherwise.
 */
int flosslogic_is_supported_la(const char *la_string)
{
	int i = 0;
	struct logic_analyzer la;

	if (la_string == NULL)
		return 0;

	while (1) {
		la = flosslogic_logic_analyzers[i++];
		if (la.shortname == NULL)
			break;
		if (!strcasecmp(la_string, la.shortname))
			return 1;
	}

	return 0;
}
