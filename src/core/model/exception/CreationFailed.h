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

#ifndef CREATIONFAILED_H
#define CREATIONFAILED_H

#include "util/ComposerCore_global.h"

#include <QString>

#include <exception>
using namespace std;

CPR_CORE_BEGIN_NAMESPACE

/*!
 * \ingroup core
 * \brief Exception to inform that the creation of an element failed.
 */
class CreationFailed : public exception
{
public:
  /*!
   * \brief
   *
   * \param _element
   * \param _id
   */
  CreationFailed(QString _element, QString _id) : element(_element), id(_id) { }
  ~CreationFailed() throw() { }

  const char* what() const throw()
  {
    QString ret = "Entity (" + element + ") with id (" + id + ") could "
        + "not be created!";

    return ret.toStdString().c_str();
  }

private:
  QString element; /*!< TODO */
  QString id; /*!< TODO */
};

CPR_CORE_END_NAMESPACE

#endif // CREATIONFAILED_H
