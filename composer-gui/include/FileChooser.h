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

#ifndef FILECHOOSER_H
#define FILECHOOSER_H

#include <QWidget>

namespace Ui {
  class FileChooser;
}

class FileChooser : public QWidget
{
  Q_OBJECT
  Q_ENUMS(FILECHOOSER_TYPE)

public:
  enum  FILECHOOSER_TYPE
  {
    OPEN_FILENAME,
    SAVE_FILENAME,
    GET_EXISTINGDIRECTORY,
    OPEN_FILENAMES
  };

  explicit FileChooser(const QString &label,
                       FileChooser::FILECHOOSER_TYPE fctype,
                       const QString &caption = "",
                       const QString &filter = "",
                       QWidget *parent = 0);
  ~FileChooser();

  QString getText();
  void setText(const QString &value);
  
private:
  FILECHOOSER_TYPE fctype;
  QString filter, caption;
  Ui::FileChooser *ui;

private slots:
  void on_pushButton_pressed();
};

#endif // FILECHOOSER_H
