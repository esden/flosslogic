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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QLineEdit>
#include <QDockWidget>
#include <QGridLayout>
#include <flosslogic.h>
#include "channelrenderarea.h"

extern struct flosslogic_context ctx;
extern uint8_t *sample_buffer;

namespace Ui
{
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

	void setCurrentLA(int la);
	int getCurrentLA(void);
	void setNumChannels(int ch);
	int getNumChannels(void);
	void setSampleRate(uint64_t s);
	uint64_t getSampleRate(void);
	void setNumSamples(uint64_t s);
	uint64_t getNumSamples(void);

	/* TODO: Don't hardcode number of channels. */
#define NUMCHANNELS 8

	/* FIXME */
	QLineEdit *lineEdits[NUMCHANNELS];
	ChannelRenderArea *channelRenderAreas[NUMCHANNELS];
	QDockWidget *dockWidgets[NUMCHANNELS];
	QGridLayout *gridLayouts[NUMCHANNELS];
	QWidget *widgets[NUMCHANNELS];

	void setupDockWidgets(void);

private:
	Ui::MainWindow *ui;
	int currentLA;
	int numChannels;
	uint64_t sampleRate;
	uint64_t numSamples;

private slots:
	void on_action_Get_samples_triggered();
	void on_action_Save_as_triggered();
	void on_action_Open_triggered();
	void on_actionScan_triggered();
	void on_actionPreferences_triggered();
	void on_actionAbout_Qt_triggered();
	void on_actionAbout_triggered();
};

#endif // MAINWINDOW_H
