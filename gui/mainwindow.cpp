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
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
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
		tr("Open sample file"), "/", tr("Raw sample files (*.bin)"));
}
