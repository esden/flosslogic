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

#include <QPainter>
#include <QPen>
#include <flosslogic.h>
#include "channelrenderarea.h"
#include "mainwindow.h"

extern uint8_t *sample_buffer;

ChannelRenderArea::ChannelRenderArea(QWidget *parent) : QWidget(parent)
{
	channelColor = Qt::black;
}

QSize ChannelRenderArea::minimumSizeHint() const
{
	return QSize(300, 10);
	// return QSize(Qt::PreferredSize, Qt::PreferredSize);
}

QSize ChannelRenderArea::sizeHint() const
{
	return QSize(300, 10);
	// return QSize(Qt::PreferredSize, Qt::PreferredSize);
}

void ChannelRenderArea::paintEvent(QPaintEvent *event)
{
	int bit, low = this->height() - 2, high = 2, current_x, current_y;
	uint64_t i;
	QPainter painter(this);

	sample_start = 0;
	sample_stop = 512 * 100;

	if (sample_buffer == NULL)
		return;

	/* TODO: Use Qt::black etc. */
	QPen pen(this->getChannelColor(), 1, Qt::SolidLine, Qt::SquareCap,
		 Qt::BevelJoin);
	painter.setPen(pen);

	current_x = 0;
	current_y = low; /* FIXME? */

	QPainterPath path;
	path.moveTo(current_x, current_y);

	for (i = sample_start; i < sample_stop; i++) {
		current_x += 10;
		path.lineTo(current_x, current_y);
		bit = getbit(sample_buffer, i, getChannelNumber());
		if (bit != 0)
			current_y = high;
		else
			current_y = low;
		path.lineTo(current_x, current_y);
	}

	painter.drawPath(path);
}

void ChannelRenderArea::setChannelColor(QColor color)
{
	channelColor = color;
}

QColor ChannelRenderArea::getChannelColor(void)
{
	return channelColor;
}

void ChannelRenderArea::setChannelNumber(int ch)
{
	channelNumber = ch;
}

int ChannelRenderArea::getChannelNumber(void)
{
	return channelNumber;
}
