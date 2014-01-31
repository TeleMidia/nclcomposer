/*
 * Copyright 2011 TeleMidia/PUC-Rio.
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

#include "FileChooser.h"
#include "ui_FileChooser.h"

#include <QFileDialog>

FileChooser::FileChooser( const QString &label,
                          FileChooser::FILECHOOSER_TYPE fctype,
                          const QString &caption,
                          const QString &filter,
                          QWidget *parent ) :
  QWidget(parent),
  ui(new Ui::FileChooser),
  fctype(fctype),
  filter(filter),
  caption(caption)
{
  ui->setupUi(this);
  ui->label->setText(label);
}

FileChooser::~FileChooser()
{
  delete ui;
}

void FileChooser::on_pushButton_pressed()
{
  QString location;

  switch(fctype)
  {
    case OPEN_FILENAME:
      location = QFileDialog::getOpenFileName(NULL, caption, "", filter);
      break;
    case SAVE_FILENAME:
      location = QFileDialog::getSaveFileName(NULL, caption, "", filter);
      break;
    case GET_EXISTINGDIRECTORY:
      location = QFileDialog::getExistingDirectory(NULL, caption, "");
      break;
    //case OPEN_FILENAMES:
    //  location = QFileDialog::getOpenFileNames(NULL, caption, "", filter);
    //  break;
    default:
      location = QFileDialog::getOpenFileName(NULL, caption, "", filter);
      break;
  }

  ui->lineEdit->setText(location);
}

QString FileChooser::getText()
{
  return ui->lineEdit->text();
}

void FileChooser::setText(const QString &value)
{
  ui->lineEdit->setText(value);
}
