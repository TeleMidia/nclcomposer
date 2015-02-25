/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef CREATIONFAILED_H
#define CREATIONFAILED_H

#include <QString>

#include <exception>
using namespace std;

namespace composer{
    namespace core {
        namespace util {

/*!
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

} } } // end namespace

#endif // CREATIONFAILED_H
