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

#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QListWidgetItem>
#include <QStackedWidget>

class ConfigDialog : public QDialog
{
	Q_OBJECT;

public:
	ConfigDialog();

public slots:
	void changeContent(QListWidgetItem *cur, QListWidgetItem *prev);

private:
	QListWidget *menu;
	QStackedWidget *content;
};

class GeneralContent : public QWidget
{
public:
	GeneralContent(QWidget *parent = 0);
};

class UIContent : public QWidget
{
public:
	UIContent(QWidget *parent = 0);
};

#endif // CONFIGDIALOG_H
