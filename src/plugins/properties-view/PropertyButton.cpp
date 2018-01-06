/*
 * Copyright (C) 2011-2018 TeleMidia/PUC-Rio.
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

#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QToolButton>

#include <QDebug>

#include <util/Utilities.h>
using namespace cpr::core;

PropertyButtons::PropertyButtons (QWidget *parent)
    : LineEditWithButton (parent, ":/images/esf-search.png")
{
  connect (_mButton, SIGNAL (pressed ()), SLOT (openfile ()),
           Qt::DirectConnection);
}

void
PropertyButtons::openfile ()
{
  QString filename = QFileDialog::getOpenFileName (
      this, tr ("Select file"), Utilities::lastFileDialogPath (), "",
      nullptr, QFileDialog::DontUseNativeDialog);

  if (!filename.isEmpty () && !filename.isNull ())
  {
    Utilities::updateLastFileDialogPath (filename);
    setText (filename);
  }
}
