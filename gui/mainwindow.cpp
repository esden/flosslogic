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
#include <QProgressDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "configdialog.h"
#include <flosslogic.h>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::MainWindow)
{
	currentLA = -1;
	numChannels = -1;

	ui->setupUi(this);

	/* FIXME */
	QMainWindow::setCentralWidget(ui->mainWidget);

	// this->setDockOptions(QMainWindow::AllowNestedDocks);
}

MainWindow::~MainWindow()
{
	int hw;

	hw = getCurrentLA();
	if (hw >= 0)
		flosslogic_hw_shutdown(hw, &ctx);

	delete ui;
}

void MainWindow::setupDockWidgets(void)
{
	int i;
	QString s;
	QColor color;

	/* TODO: Do not create new dockWidgets if we already have them. */

	/* TODO: Kill any old dockWidgets before creating new ones? */

	for (i = 0; i < getNumChannels(); i++) {
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
		channelRenderAreas[i]->setChannelNumber(i);
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

		// dockWidgets[i]->show();
#if 0
		/* If the user renames a channel, adapt the dock title. */
		QObject::connect(lineEdits[i], SIGNAL(textChanged(QString)),
				 dockWidgets[i], SLOT(setWindowTitle(QString)));
#endif
	}
}

void MainWindow::setCurrentLA(int la)
{
	currentLA = la;
}

int MainWindow::getCurrentLA(void)
{
	return currentLA;
}

void MainWindow::setNumChannels(int ch)
{
	numChannels = ch;
}

int MainWindow::getNumChannels(void)
{
	return numChannels;
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
	ConfigDialog *dialog = new ConfigDialog();
	dialog->exec();
	/* TODO */
}

void MainWindow::on_actionScan_triggered()
{
	int ret, i;
	QString s;
	struct logic_analyzer la;

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
	setNumChannels(flosslogic_logic_analyzers[ret].numchannels);

	ui->comboBoxLA->clear();
	ui->comboBoxLA->addItem(flosslogic_logic_analyzers[ret].shortname);
	ui->labelChannels->setText(s.sprintf("Channels: %d",
			flosslogic_logic_analyzers[ret].numchannels));

	i = 0;
	la = flosslogic_logic_analyzers[getCurrentLA()];
	while (la.samplerates[i].string != NULL) {
		ui->comboBoxSampleRate->addItem(la.samplerates[i].string,
			la.samplerates[i].samplerate);
		i++;
	}

	/* FIXME */
	ui->comboBoxNumSamples->addItem("3000000", 3000000);
	ui->comboBoxNumSamples->addItem("2000000", 2000000);
	ui->comboBoxNumSamples->addItem("1000000", 1000000);
	ui->comboBoxNumSamples->setEditable(true);

	ret = flosslogic_hw_init(getCurrentLA(), &ctx);
	if (ret < 0)
		statusBar()->showMessage(tr("ERROR: LA init failed."));

	if (getCurrentLA() >= 0)
		setupDockWidgets();

	/* Enable all fields now (i.e. make them non-gray). */
	ui->comboBoxSampleRate->setEnabled(true);
	ui->comboBoxNumSamples->setEnabled(true);
	ui->labelChannels->setEnabled(true);
}

void MainWindow::on_action_Open_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open sample file"), ".",
		tr("Raw sample files (*.raw *.bin);;"
		   "Gnuplot data files (*.dat);;"
		   "VCD files (*.vcd);;"
		   "All files (*)"));

	/* TODO: Error handling. */

	QFile file(fileName);
	file.open(QIODevice::ReadOnly);
	QDataStream in(&file);

	/* TODO: Implement support for loading different input formats. */

	sample_buffer = (uint8_t *)malloc(512 * 100); /* FIXME */

	in.readRawData((char *)sample_buffer, 512 * 100 /* FIXME */);
	file.close();

	setNumChannels(8); /* FIXME */

	/* FIXME. */
	setupDockWidgets();

	/* FIXME. */
	// for (int i = 0; i < 8; i++)
	// 	widgets[i]->repaint();

	// statusBar()->showMessage(fileName);
}

void MainWindow::on_action_Save_as_triggered()
{
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save sample file"), ".",
		tr("Raw sample files (*.raw *.bin);;All files (*)"));

	/* TODO: Error handling. */

	QFile file(fileName);
	file.open(QIODevice::WriteOnly);
	QDataStream out(&file);

	/* TODO: Implement support for saving to different output formats. */

	out.writeRawData((const char *)sample_buffer, 512 * 100 /* FIXME */);
	file.close();

	// statusBar()->showMessage(fileName);
}

void MainWindow::on_action_Get_samples_triggered()
{
	int ret;
	uint8_t *buf;
	uint64_t pos = 0;
	uint64_t maxValue = 512 * 10000; /* FIXME */
	uint64_t chunksize = 512;
	QString s;

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

	sample_buffer = buf;
	for (int i = 0; i < getNumChannels(); i++) {
		channelRenderAreas[i]->setSampleStart(0);
		channelRenderAreas[i]->setSampleEnd(1000);
	}

	/* FIXME */
	s.sprintf("%d", (int)channelRenderAreas[0]->getSampleStart());
	s.prepend(tr("Start sample: "));
	ui->labelSampleStart->setText(s);

	/* FIXME */
	s.sprintf("%d", (int)channelRenderAreas[0]->getSampleEnd());
	s.prepend(tr("End sample: "));
	ui->labelSampleEnd->setText(s);

	/* Enable both labels. */
	ui->labelSampleStart->setEnabled(true);
	ui->labelSampleEnd->setEnabled(true);

	flosslogic_hw_get_samples_shutdown(&ctx, 1000);
}

void MainWindow::setSampleRate(uint64_t s)
{
	sampleRate = s;
}

uint64_t MainWindow::getSampleRate(void)
{
	return sampleRate;
}

void MainWindow::setNumSamples(uint64_t s)
{
	numSamples = s;
}

uint64_t MainWindow::getNumSamples(void)
{
	return numSamples;
}
