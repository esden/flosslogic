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

	for (int i = 0; i < NUMCHANNELS; i++)
		dockWidgets[i] = NULL;

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

		QObject::connect(channelRenderAreas[i],
				 SIGNAL(sampleStartChanged(QString)),
				 ui->labelSampleStart, SLOT(setText(QString)));
		QObject::connect(channelRenderAreas[i],
				 SIGNAL(sampleEndChanged(QString)),
				 ui->labelSampleEnd, SLOT(setText(QString)));
		QObject::connect(channelRenderAreas[i],
				 SIGNAL(zoomFactorChanged(QString)),
				 ui->labelZoomFactor, SLOT(setText(QString)));

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
		tr("<center>flosslogic-gui 0.1</center><br />\n"
		"<center>Copyright (C) 2010 "
		"Uwe Hermann &lt;uwe@hermann-uwe.de&gt;</center>\n"
		"<center>GNU GPL, version 2 or later</center><br />\n"
		"<center><a href=\"http://www.flosslogic.org\">"
		"http://www.flosslogic.org</a></center>"));
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

	/* Enable all relevant fields now (i.e. make them non-gray). */
	ui->comboBoxSampleRate->setEnabled(true);
	ui->comboBoxNumSamples->setEnabled(true);
	ui->labelChannels->setEnabled(true);
	ui->action_Get_samples->setEnabled(true);
}

void MainWindow::on_action_Open_triggered()
{
	QString s;
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open sample file"), ".",
		tr("Raw sample files (*.raw *.bin);;"
		   "Gnuplot data files (*.dat);;"
		   "VCD files (*.vcd);;"
		   "All files (*)"));

	if (fileName == NULL)
		return;

	QFile file(fileName);
	file.open(QIODevice::ReadOnly);
	QDataStream in(&file);

	/* TODO: Implement support for loading different input formats. */

	sample_buffer = (uint8_t *)malloc(file.size());
	if (sample_buffer == NULL) {
		/* TODO: Error handling. */
	}

	in.readRawData((char *)sample_buffer, file.size());

	setNumSamples(file.size());
	setNumChannels(8); /* FIXME */

	file.close();

	setupDockWidgets();

	ui->comboBoxLA->clear();
	ui->comboBoxLA->addItem(tr("File"));

	/* FIXME: Store number of channels in the file or allow user config. */
	s.sprintf("%d", getNumChannels());
	s.prepend(tr("Channels: "));
	ui->labelChannels->setText(s);
	ui->labelChannels->setEnabled(false);

	ui->comboBoxSampleRate->clear();
	ui->comboBoxSampleRate->setEnabled(false); /* FIXME */

	ui->comboBoxNumSamples->clear();
	ui->comboBoxNumSamples->addItem(s.sprintf("%d", getNumSamples()),
					getNumSamples());
	ui->comboBoxNumSamples->setEnabled(true);

	ui->labelSampleStart->setText(tr("Start sample: "));
	ui->labelSampleStart->setEnabled(true);

	ui->labelSampleEnd->setText(tr("End sample: "));
	ui->labelSampleEnd->setEnabled(true);

	ui->labelZoomFactor->setText(tr("Zoom factor: "));
	ui->labelZoomFactor->setEnabled(true);

	ui->action_Save_as->setEnabled(true);
	ui->action_Get_samples->setEnabled(false);

	for (int i = 0; i < getNumChannels(); i++) {
		channelRenderAreas[i]->setSampleStart(0);
		channelRenderAreas[i]->setSampleEnd(getNumSamples());
	}

	/* FIXME */
#if 0
	for (int i = 0; i < getNumChannels(); i++) {
		widgets[i]->repaint();
		channelRenderAreas[i]->repaint();
	}
#endif
}

void MainWindow::on_action_Save_as_triggered()
{
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save sample file"), ".",
		tr("Raw sample files (*.raw *.bin);;All files (*)"));

	if (fileName == NULL)
		return;

	QFile file(fileName);
	file.open(QIODevice::WriteOnly);
	QDataStream out(&file);

	/* TODO: Implement support for saving to different output formats. */

	out.writeRawData((const char *)sample_buffer,
			 getNumSamples() * (getNumChannels() / 8));
	file.close();
}

void MainWindow::on_action_Get_samples_triggered()
{
	int ret;
	uint8_t *buf;
	uint64_t pos = 0;
	uint64_t numSamplesLocal = ui->comboBoxNumSamples->itemData(
			ui->comboBoxNumSamples->currentIndex()).toLongLong();
	uint64_t chunksize = 512;
	QString s;

	if (getCurrentLA() < 0) {
		/* TODO */
		return;
	}

	buf = flosslogic_hw_get_samples_init(&ctx, numSamplesLocal, 1000000, 1000);
	if (buf == NULL) {
		/* TODO: Error handling. */
		return;
	}

	QProgressDialog progress("Getting samples from logic analyzer...",
				 "Abort", 0, numSamplesLocal, this);
	progress.setWindowModality(Qt::WindowModal);
	progress.setMinimumDuration(500);

	for (uint64_t i = 0; i < numSamplesLocal; i += chunksize) {
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
	progress.setValue(numSamplesLocal);

	sample_buffer = buf;
	for (int i = 0; i < getNumChannels(); i++) {
		channelRenderAreas[i]->setNumSamples(numSamplesLocal);
		channelRenderAreas[i]->setSampleStart(0);
		channelRenderAreas[i]->setSampleEnd(numSamplesLocal);
	}

#if 0
	/* FIXME */
	s.sprintf("%d", (int)channelRenderAreas[0]->getSampleStart());
	s.prepend(tr("Start sample: "));
	ui->labelSampleStart->setText(s);

	/* FIXME */
	s.sprintf("%d", (int)channelRenderAreas[0]->getSampleEnd());
	s.prepend(tr("End sample: "));
	ui->labelSampleEnd->setText(s);
#endif

	/* Enable the relevant labels/buttons. */
	ui->labelSampleStart->setEnabled(true);
	ui->labelSampleEnd->setEnabled(true);
	ui->labelZoomFactor->setEnabled(true);
	ui->action_Save_as->setEnabled(true);

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

void MainWindow::on_action_New_triggered()
{
	for (int i = 0; i < NUMCHANNELS; i++) {
		if (dockWidgets[i]) {
			/* TODO: Check if all childs are also killed. */
			delete dockWidgets[i];
			dockWidgets[i] = NULL;
		}
	}

	ui->comboBoxLA->clear();
	ui->comboBoxLA->addItem(tr("No LA detected"));

	ui->labelChannels->setText(tr("Channels: "));
	ui->labelChannels->setEnabled(false);

	ui->comboBoxSampleRate->clear();
	ui->comboBoxSampleRate->setEnabled(false);

	ui->comboBoxNumSamples->clear();
	ui->comboBoxNumSamples->setEnabled(false);

	ui->labelSampleStart->setText(tr("Start sample: "));
	ui->labelSampleStart->setEnabled(false);

	ui->labelSampleEnd->setText(tr("End sample: "));
	ui->labelSampleEnd->setEnabled(false);

	ui->labelZoomFactor->setText(tr("Zoom factor: "));
	ui->labelZoomFactor->setEnabled(false);

	ui->action_Save_as->setEnabled(false);
	ui->action_Get_samples->setEnabled(false);

	/* TODO: More cleanups. */
	/* TODO: Free sample buffer(s). */
}

uint8_t *getDemoSampleBuffer(void)
{
	/* TODO */
}
