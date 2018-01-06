/* Copyright (C) 2011-2018 PUC-Rio/TeleMídia Lab.

 NCL Composer is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published
 by the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 NCL Composer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU General Lesser Public License
 along with NCL Composer.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef FILECHOOSER_H
#define FILECHOOSER_H

#include <QWidget>
#include <util/ComposerCore_global.h>

namespace Ui
{
class FileChooser;
}

CPR_GUI_BEGIN_NAMESPACE

class FileChooser : public QWidget
{
  Q_OBJECT
  Q_ENUMS (FILECHOOSER_TYPE)

public:
  enum FILECHOOSER_TYPE
  {
    OPEN_FILENAME,
    SAVE_FILENAME,
    GET_EXISTINGDIRECTORY,
    OPEN_FILENAMES
  };

  explicit FileChooser (const QString &label,
                        FileChooser::FILECHOOSER_TYPE fctype,
                        const QString &caption = "",
                        const QString &filter = "", QWidget *parent = 0);
  ~FileChooser ();

  QString getText ();
  void setText (const QString &value);

private:
  Ui::FileChooser *ui;
  FILECHOOSER_TYPE fctype;
  QString filter, caption;

private slots:
  void on_pushButton_pressed ();
};

CPR_GUI_END_NAMESPACE

#endif // FILECHOOSER_H
