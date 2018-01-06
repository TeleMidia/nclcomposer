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

#ifndef __SINGLETON_HPP_
#define __SINGLETON_HPP_

#include <QObject>
#include <assert.h>

#define SINGLETON(T)                                                          \
private:                                                                      \
  static T *m_pInstance;                                                      \
                                                                              \
public:                                                                       \
  static T *instance ()                                                       \
  {                                                                           \
    if (m_pInstance == nullptr)                                               \
      return m_pInstance = new T ();                                          \
    return m_pInstance;                                                       \
  }                                                                           \
  static void release ()                                                      \
  {                                                                           \
    if (m_pInstance != nullptr)                                               \
      delete m_pInstance;                                                     \
    m_pInstance = nullptr;                                                    \
  }

#define INIT_SINGLETON(T) T *T::m_pInstance = nullptr;

#endif
