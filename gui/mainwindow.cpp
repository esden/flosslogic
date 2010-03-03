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
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "channelrenderarea.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::MainWindow)
{
	int i;
	QLineEdit *lineEdits[8];
	ChannelRenderArea *channelRenderAreas[8];
	QDockWidget *dockWidgets[8];
	QGridLayout *gridLayouts[8];
	QWidget *widgets[8];
	QString s;
	QColor color;

	ui->setupUi(this);

	/* FIXME */
	ui->tabWidget->setMaximumSize(400, 800);
	QMainWindow::setCentralWidget(ui->tabWidget);

	// this->setDockOptions(QMainWindow::AllowNestedDocks);

	/* TODO: Don't hardcode number of channels. */
	for (i = 0; i < 8; i++) {
		widgets[i] = new QWidget(this);
		gridLayouts[i] = new QGridLayout(widgets[i]);

		lineEdits[i] = new QLineEdit(this);
		lineEdits[i]->setText(s.sprintf("Channel %d", i));
		/* Use random colors for the channel names for now. */
		QPalette p = QPalette(QApplication::palette());
		// p.setColor(QPalette::Base, QColor(2 + qrand() * 16));
		color = QColor(2 + qrand() * 16);
		p.setColor(QPalette::Base, color);
		lineEdits[i]->setPalette(p);
		gridLayouts[i]->addWidget(lineEdits[i], i, 1);

		channelRenderAreas[i] = new ChannelRenderArea(this);
		channelRenderAreas[i]->setSizePolicy(QSizePolicy::Minimum,
					QSizePolicy::MinimumExpanding);
		channelRenderAreas[i]->setChannelColor(color);
		gridLayouts[i]->addWidget(channelRenderAreas[i], i, 2);

		dockWidgets[i] = new QDockWidget(s.sprintf("Channel %d", i),
						 this);
		dockWidgets[i]->setAllowedAreas(Qt::RightDockWidgetArea);
		dockWidgets[i]->setWidget(widgets[i]);
		addDockWidget(Qt::RightDockWidgetArea, dockWidgets[i]);
	}
}

MainWindow::~MainWindow()
{
	delete ui;
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
	statusBar()->showMessage(tr("Scanning for supported logic analyzers..."));
}

void MainWindow::on_action_Open_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open sample file"), ".", tr("Raw sample files (*.bin)"));
}
