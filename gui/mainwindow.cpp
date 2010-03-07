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

#include <QMessageBox>
#include <QFileDialog>
#include <QLineEdit>
#include <QDockWidget>
#include <QGridLayout>
#include <QProgressDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "channelrenderarea.h"

#include <flosslogic.h>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::MainWindow)
{
/* TODO: Don't hardcode number of channels. */
#define NUMCHANNELS 8

	int i;
	QLineEdit *lineEdits[NUMCHANNELS];
	ChannelRenderArea *channelRenderAreas[NUMCHANNELS];
	QDockWidget *dockWidgets[NUMCHANNELS];
	QGridLayout *gridLayouts[NUMCHANNELS];
	QWidget *widgets[NUMCHANNELS];
	QString s;
	QColor color;

	setCurrentLA(-1);

	ui->setupUi(this);

	/* FIXME */
	QMainWindow::setCentralWidget(ui->infoLabel);

	// this->setDockOptions(QMainWindow::AllowNestedDocks);

	for (i = 0; i < NUMCHANNELS; i++) {
		widgets[i] = new QWidget(this);
		gridLayouts[i] = new QGridLayout(widgets[i]);

		lineEdits[i] = new QLineEdit(this);
		lineEdits[i]->setMaximumWidth(150);
		lineEdits[i]->setText(s.sprintf("Channel %d", i));
		/* Use random colors for the channel names for now. */
		QPalette p = QPalette(QApplication::palette());
		color = QColor(2 + qrand() * 16);
		p.setColor(QPalette::Base, color);
		lineEdits[i]->setPalette(p);
		gridLayouts[i]->addWidget(lineEdits[i], i, 1);

		channelRenderAreas[i] = new ChannelRenderArea(this);
		channelRenderAreas[i]->setSizePolicy(QSizePolicy::Minimum,
					QSizePolicy::MinimumExpanding);
		channelRenderAreas[i]->setChannelColor(color);
		gridLayouts[i]->addWidget(channelRenderAreas[i], i, 2);

		dockWidgets[i] = new QDockWidget(this);
		dockWidgets[i]->setAllowedAreas(Qt::RightDockWidgetArea);
		dockWidgets[i]->setFeatures(
			QDockWidget::DockWidgetClosable |
			QDockWidget::DockWidgetMovable |
			QDockWidget::DockWidgetFloatable |
			QDockWidget::DockWidgetVerticalTitleBar);
		dockWidgets[i]->setWidget(widgets[i]);
		addDockWidget(Qt::RightDockWidgetArea, dockWidgets[i]);

#if 0
		/* If the user renames a channel, adapt the dock title. */
		QObject::connect(lineEdits[i], SIGNAL(textChanged(QString)),
				 dockWidgets[i], SLOT(setWindowTitle(QString)));
#endif
	}
}

MainWindow::~MainWindow()
{
	int hw;

	hw = getCurrentLA();
	if (hw >= 0)
		flosslogic_hw_shutdown(hw, &ctx);

	delete ui;
}

void MainWindow::setCurrentLA(int la)
{
	currentLA = la;
}

int MainWindow::getCurrentLA(void)
{
	return currentLA;
}

void MainWindow::on_actionAbout_triggered()
{
	QMessageBox::about(this, tr("About"),
			   tr("<center>flosslogic-gui 0.1</center>\n\n"
			   "<center>Copyright (C) 2010 "
			   "Uwe Hermann &lt;uwe@hermann-uwe.de&gt;</center>\n"
			   "<center>GNU GPL, version 2 or later</center>\n"
			   "<center>http://www.flosslogic.org</center>"));
}

void MainWindow::on_actionAbout_Qt_triggered()
{
	QMessageBox::aboutQt(this, tr("About Qt"));
}

void MainWindow::on_actionPreferences_triggered()
{
	QMessageBox::about(this, tr("Preferences"), tr("Not yet implemented."));
}

void MainWindow::on_actionScan_triggered()
{
	int ret;
	QString s;

	statusBar()->showMessage(tr("Scanning for logic analyzers..."));

	ret = flosslogic_scan_for_devices(&ctx);
	if (ret < 0) {
		s = tr("No supported logic analyzer found.");
		statusBar()->showMessage(s);
		return;
	} else {
		s = tr("Found supported logic analyzer: ");
		s.append(flosslogic_logic_analyzers[ret].shortname);
		statusBar()->showMessage(s);
	}

	setCurrentLA(ret);

	ret = flosslogic_hw_init(getCurrentLA(), &ctx);
	if (ret < 0)
		statusBar()->showMessage(tr("ERROR: LA init failed."));
}

void MainWindow::on_action_Open_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open sample file"), ".",
		tr("Raw sample files (*.raw *.bin);;"
		   "Gnuplot data files (*.dat);;"
		   "VCD files (*.vcd);;"
		   "All files (*)"));

	statusBar()->showMessage(fileName);
}

void MainWindow::on_action_Save_as_triggered()
{
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save sample file"), ".",
		tr("Raw sample files (*.raw *.bin);;All files (*)"));

	statusBar()->showMessage(fileName);
}

void MainWindow::on_action_Get_samples_triggered()
{
	int ret;
	uint8_t *buf;
	uint64_t pos = 0;
	uint64_t maxValue = 512 * 10000; /* FIXME */
	uint64_t chunksize = 512;

	if (getCurrentLA() < 0) {
		/* TODO */
		return;
	}

	buf = flosslogic_hw_get_samples_init(&ctx, maxValue, 1000000, 1000);
	if (buf == NULL) {
		/* TODO: Error handling. */
		return;
	}

	QProgressDialog progress("Getting samples from logic analyzer...",
				 "Abort", 0, maxValue, this);
	progress.setWindowModality(Qt::WindowModal);
	progress.setMinimumDuration(500);

	for (uint64_t i = 0; i < maxValue; i += chunksize) {
		progress.setValue(i);

		/* TODO: Properly handle an abort. */
		if (progress.wasCanceled())
			break;

		/* Get a small chunk of samples. */
		ret = flosslogic_hw_get_samples_chunk(&ctx, buf, chunksize,
						      pos, 1000);
		pos += chunksize;
		/* TODO: Error handling. */
	}
	progress.setValue(maxValue);

	flosslogic_hw_get_samples_shutdown(&ctx, 1000);
}
