/* Copyright (C) 2011-2017 PUC-Rio/TeleMídia Lab.

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


#ifndef IPREFENCESPAGE_H
#define IPREFENCESPAGE_H

#include <QWidget>
#include <QIcon>

namespace composer {
  namespace gui {

class IPreferencesPage : public QWidget
{

public:
  IPreferencesPage(QWidget *parent) : QWidget(parent) { }

  virtual const QString getName() { return windowTitle(); }
  virtual const QIcon getIcon() = 0;
  virtual ~IPreferencesPage() {}

public slots:
  virtual void applyValues() = 0;
  virtual void setDefaultValues() = 0;
};

}} //end namespace
#endif
