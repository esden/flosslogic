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

#include <QtGui>
#include "configdialog.h"

ConfigDialog::ConfigDialog()
{
	menu = new QListWidget;
	menu->setViewMode(QListView::ListMode);
	menu->setMovement(QListView::Static);
	menu->setMaximumWidth(120);
	menu->setSpacing(2);

	content = new QStackedWidget;
	content->addWidget(new GeneralContent);
	content->addWidget(new UIContent);

	QListWidgetItem *generalButton = new QListWidgetItem(menu);
	generalButton->setText(tr("General"));
	generalButton->setTextAlignment(Qt::AlignHCenter);
	generalButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	QListWidgetItem *uiButton = new QListWidgetItem(menu);
	uiButton->setText(tr("User Interface"));
	uiButton->setTextAlignment(Qt::AlignHCenter);
	uiButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	connect(menu,
	  SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
	  this, SLOT(changeContent(QListWidgetItem *, QListWidgetItem *)));

	menu->setCurrentRow(0);

	QPushButton *closeButton = new QPushButton(tr("Close"));
	connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

	/* Horizontal layout for the menu and content. */
	QHBoxLayout *hLayout = new QHBoxLayout;
	hLayout->addWidget(menu);
	hLayout->addWidget(content, 1);

	/* Layout for the Close button. */
	QHBoxLayout *bLayout = new QHBoxLayout;
	bLayout->addStretch(1);
	bLayout->addWidget(closeButton);

	/* Main layout. */
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(hLayout);
	mainLayout->addStretch(1);
	mainLayout->addSpacing(12);
	mainLayout->addLayout(bLayout);
	setLayout(mainLayout);

	setWindowTitle(tr("Preferences"));
}

void ConfigDialog::changeContent(QListWidgetItem *cur, QListWidgetItem *prev)
{
	if (!cur)
		cur = prev;

	content->setCurrentIndex(menu->row(cur));
}

GeneralContent::GeneralContent(QWidget *parent)
	: QWidget(parent)
{
	/* TODO */
}

UIContent::UIContent(QWidget *parent)
	: QWidget(parent)
{
	QGroupBox *channelsGroup = new QGroupBox(tr("Channels"));

	QLabel *titleBarLabel = new QLabel(tr("Channel title bar:"));
	QComboBox *titleBarCombo = new QComboBox;
	titleBarCombo->addItem(tr("Vertical"));
	titleBarCombo->addItem(tr("Horizontal"));

	QHBoxLayout *titleBarLayout = new QHBoxLayout;
	titleBarLayout->addWidget(titleBarLabel);
	titleBarLayout->addWidget(titleBarCombo);

	QVBoxLayout *channelsLayout = new QVBoxLayout;
	channelsLayout->addLayout(titleBarLayout);
	channelsGroup->setLayout(channelsLayout);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(channelsGroup);
	mainLayout->addStretch(1);
	setLayout(mainLayout);

	/* TODO */
}
