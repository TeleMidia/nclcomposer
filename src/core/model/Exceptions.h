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
  CreationFailed (QString _element, QString _id) : element (_element), id (_id)
  {
  }
  ~CreationFailed () throw () {}

  const char *
  what () const throw ()
  {
    QString ret = "Entity (" + element + ") with id (" + id + ") could "
                  + "not be created!";

    return ret.toStdString ().c_str ();
  }

private:
  QString element; /*!< TODO */
  QString id;      /*!< TODO */
};

/*!
 * \ingroup core
 * \brief Exception to inform that the desired entity wasn't find (e.g. in the
 * model).
 */
class EntityNotFound : public exception
{
public:
  /*!
   \brief Contructor.

   \param _element
   \param _id
  */
  EntityNotFound (const QString &element, const QString &id)
      : _element (element), _id (id)
  {
  }

  /*!
      \brief Destructor.
  */
  ~EntityNotFound () throw () {}

  /*!
    \brief return a description of what the exception represents.
  */
  const char *
  what () const throw ()
  {
    QString ret = "Entity (" + _element + ") with id (" + _id + ") not found!";
    return ret.toStdString ().c_str ();
  }

private:
  QString _element; /*!< TODO */
  QString _id;      /*!< TODO */
};

/*!
 * \ingroup core
 * \brief Exception to inform that the entity doesn't have a parent.
 */
class ParentNotFound : public exception
{
public:
  /*!
   * \brief Constructor
   *
   * \param element
   * \param parent
   * \param id
   */
  ParentNotFound (const QString &element, const QString &parent,
                  const QString &id)
      : _element (element), _parent (parent), _id (id)
  {
  }

  /*!
   * \brief Descriptor
   */
  ~ParentNotFound () throw () {}

  /*!
   * \brief return a description of what the exception represents.
   */
  const char *
  what () const throw ()
  {
    QString ret = "Entity (" + _parent + ") parent of " + _element + "(" + _id
                  + ") could not be found!";
    return ret.toStdString ().c_str ();
  }

private:
  QString _element; /*!< TODO */
  QString _parent;  /*!< TODO */
  QString _id;      /*!< TODO */
};

CPR_CORE_END_NAMESPACE

#endif // CREATIONFAILED_H
