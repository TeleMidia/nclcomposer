/*
 * Copyright 2011-2012 TeleMidia/PUC-Rio.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include "PropertyButton.h"

#include <QFileDialog>
#include <QToolButton>
#include <QApplication>

#include <QDebug>

#include <core/util/Utilities.h>
using namespace composer::core::util;

PropertyButtons::PropertyButtons(QString propName, QWidget *parent)
  : key(propName), LineEditWithButton(parent, ":/images/esf-search.png")
{
  connect(mButton, SIGNAL(pressed()), SLOT(openfile()));

  connect(this, SIGNAL(lostFocus()), SLOT(emitNewValue()));

  qDebug() << "PropertyButtons" << propName;
}

void PropertyButtons::openfile()
{
  disconnect(this, SIGNAL(lostFocus()), this, SLOT(emitNewValue()));
  QFileDialog dialog;
  dialog.setFileMode(QFileDialog::AnyFile);

  QString filename = dialog.getOpenFileName(NULL, tr("Select file"),
                                            Utilities::getLastFileDialogPath());

  if(!filename.isEmpty() && !filename.isNull())
  {
    Utilities::updateLastFileDialogPath(filename);
    value = filename;
    emitNewValue(value);
  }

  connect(this, SIGNAL(lostFocus()), SLOT(emitNewValue()));
}

void PropertyButtons::emitNewValue()
{
  emit newValue(key, text());
}

void PropertyButtons::emitNewValue(QString text)
{
  emit newValue(key, text);
}

