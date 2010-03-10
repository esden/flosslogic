#!/usr/bin/env lua5.1
--
-- This file is part of the flosslogic project.
--
-- Copyright (C) 2010 Uwe Hermann <uwe@hermann-uwe.de>
--
-- This program is free software; you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation; either version 2 of the License, or
-- (at your option) any later version.
--
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with this program; if not, write to the Free Software
-- Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
--

--
-- Decode SPI.
--
-- @param buf The input buffer containing the samples in binary form.
-- @param numchannels The number of channels of this logic analyzer.
-- @param numsamples The number of samples in the buffer.
-- @param samplerate The samplerate used to get the samples.
-- @param spi_cs Index of the channel corresponding to the SPI CS line.
-- @param spi_clk Index of the channel corresponding to the SPI CLK line.
-- @param spi_miso Index of the channel corresponding to the SPI MISO line.
-- @param spi_moѕi Index of the channel corresponding to the SPI MOSI line.
-- @param spi_bitwidth The SPI bit width.
-- @param spi_mode The SPI mode (0-3).
-- @return A buffer containing the SPI decode result. TODO: log.
function bin2spi(buf, numchannels, numsamples, samplerate, spi_cs, spi_clk,
		 spi_miso, spi_mosi, spi_bitwidth, spi_mode)
	-- TODO
end

