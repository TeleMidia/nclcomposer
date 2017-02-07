/* Copyright (C) 2011-2017 PUC-Rio/TeleMídia Lab.

 NCL Composer is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published
 by the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 NCL Composer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU General Lesser Public
 License along with Foobar.  If not, see <http://www.gnu.org/licenses/>. */

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
