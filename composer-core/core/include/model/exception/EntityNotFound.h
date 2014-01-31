/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef ENTITYNOTFOUND_H
#define ENTITYNOTFOUND_H

#include <QString>

#include <exception>
using namespace std;

namespace composer{
    namespace core {
        namespace util {

/*!
 \brief Exception to inform that the desired entity wasn't find (e.g. in the
            model).

*/
class EntityNotFound : public exception
{
public:
  /*!
   \brief Contructor.

   \param _element
   \param _id
  */
  EntityNotFound(const QString &element, const QString &id) :
    _element(element), _id(id)
  {

  }

  /*!
      \brief Destructor.
  */
  ~EntityNotFound() throw() {}

  /*!
    \brief return a description of what the exception represents.
  */
  const char* what() const throw()
  {
    QString ret = "Entity (" + _element + ") with id (" + _id + ") not found!";
    return ret.toStdString().c_str();
  }

private:
  QString _element; /*!< TODO */
  QString _id; /*!< TODO */
};

}}}
#endif // ENTITYNOTFOUND_H
