/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef PARENTNOTFOUND_H
#define PARENTNOTFOUND_H

#include <QString>

#include <exception>
using namespace std;

namespace composer{
    namespace core {
        namespace util {

/*!
 \brief Exception to inform that the entity doesn't have a parent.

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
  ParentNotFound( const QString &element, const QString &parent,
                  const QString &id ) :
    _element(element), _parent(parent), _id(id) {}

  /*!
   * \brief Descriptor
   */
  ~ParentNotFound() throw() {}

  /*!
   * \brief return a description of what the exception represents.
   */
  const char* what() const throw()
  {
    QString ret = "Entity (" + _parent + ") parent of " + _element + "(" + _id
        + ") could not be found!";
    return ret.toStdString().c_str();
  }

private:
  QString _element; /*!< TODO */
  QString _parent; /*!< TODO */
  QString _id; /*!< TODO */
};

} } }
#endif // PARENTNOTFOUND_H
