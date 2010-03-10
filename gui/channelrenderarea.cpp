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
#include <QColor>
#include <flosslogic.h>
#include "channelrenderarea.h"
#include "mainwindow.h"

/* WHEEL_DELTA was introduced in Qt 4.6, earlier versions don't have it. */
#ifndef WHEEL_DELTA
#define WHEEL_DELTA 120
#endif

extern uint8_t *sample_buffer;

ChannelRenderArea::ChannelRenderArea(QWidget *parent) : QWidget(parent)
{
	channelNumber = -1;
	channelColor = Qt::black;
	numSamples = 0;
	sampleStart = 0;
	sampleEnd = 0;
	zoomFactor = 1.0;
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

	if (sample_buffer == NULL)
		return;

	/* TODO: Use Qt::black etc. */
	QPen pen(this->getChannelColor(), 1, Qt::SolidLine, Qt::SquareCap,
		 Qt::BevelJoin);
	painter.setPen(pen);

	// painter.fillRect(0, 0, this->width(), this->height(), QColor(Qt::gray));

	current_x = 0;
	current_y = low; /* FIXME? */

	QPainterPath path;
	path.moveTo(current_x, current_y);

	for (i = getSampleStart(); i < getSampleEnd(); i++) {
		current_x += 2;
		path.lineTo(current_x, current_y);
		bit = getbit(sample_buffer, i, getChannelNumber());
		if (bit != 0)
			current_y = high;
		else
			current_y = low;
		path.lineTo(current_x, current_y);
	}

	painter.drawPath(path);
	// painter.scale(getZoomFactor(), getZoomFactor());
}

void ChannelRenderArea::wheelEvent(QWheelEvent *event)
{
	uint64_t sampleStartNew, sampleEndNew;
	float zoomFactorNew;

	/* FIXME: Make this constant user-configurable. */
	zoomFactorNew = getZoomFactor()
			+ 0.01 * (event->delta() / WHEEL_DELTA);
	if (zoomFactorNew < 0)
		zoomFactorNew = 0;
	if (zoomFactorNew > 2)
		zoomFactorNew = 2; /* FIXME: Don't hardcode. */
	setZoomFactor(zoomFactorNew);

	sampleStartNew = 0; /* FIXME */
	sampleEndNew = getNumSamples() * zoomFactorNew;
	if (sampleEndNew > getNumSamples())
		sampleEndNew = getNumSamples();

	setSampleStart(sampleStartNew);
	setSampleEnd(sampleEndNew);

#if 0
	uint64_t sampleStartNew, sampleEndNew;

	sampleStartNew = getSampleStart() + event->delta() / WHEEL_DELTA;
	sampleEndNew = getSampleEnd() + event->delta() / WHEEL_DELTA;

	/* TODO: More checks. */

#if 1
	if (sampleStartNew < 0 || sampleEndNew < 0)
		return;
	if (sampleStartNew > 512 * 1000 || sampleEndNew > 512 * 1000 /* FIXME */)
		return;
#endif

	setSampleStart(sampleStartNew);
	setSampleEnd(sampleEndNew); /* FIXME: Use len? */
#endif

	repaint();
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

void ChannelRenderArea::setNumSamples(uint64_t s)
{
	numSamples = s;
}

uint64_t ChannelRenderArea::getNumSamples(void)
{
	return numSamples;
}

void ChannelRenderArea::setSampleStart(uint64_t s)
{
	QString str;

	sampleStart = s;

	emit(sampleStartChanged(sampleStart));
	emit(sampleStartChanged(str.sprintf("%ld", sampleStart)));
}

uint64_t ChannelRenderArea::getSampleStart(void)
{
	return sampleStart;
}

void ChannelRenderArea::setSampleEnd(uint64_t e)
{
	QString str;

	sampleEnd = e;

	emit(sampleEndChanged(sampleEnd));
	emit(sampleEndChanged(str.sprintf("%ld", sampleEnd)));
}

uint64_t ChannelRenderArea::getSampleEnd(void)
{
	return sampleEnd;
}

void ChannelRenderArea::setZoomFactor(float z)
{
	QString str;

	zoomFactor = z;

	emit(zoomFactorChanged(zoomFactor));
	emit(zoomFactorChanged(str.sprintf("%f", zoomFactor)));
}

float ChannelRenderArea::getZoomFactor(void)
{
	return zoomFactor;
}
