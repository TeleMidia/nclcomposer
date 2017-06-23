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

#ifndef CPR_CORE_GLOBAL_H
#define CPR_CORE_GLOBAL_H

#include <QtCore/qglobal.h>

/*!
 * \defgroup core NCL Composer Core
 */

#if defined(COMPOSERCORE_MAKE_DLL)
#  define COMPOSERCORESHARED_EXPORT Q_DECL_EXPORT
#else
#  define COMPOSERCORESHARED_EXPORT Q_DECL_IMPORT
#endif

// Namespaces.
#define _CPR_NS_BEGIN(t)     namespace t {/*}*/
#define _CPR_NS_END                    /*{*/}
#define _CPR_BEGIN(t)                  _CPR_NS_BEGIN (cpr) _CPR_NS_BEGIN (t)
#define _CPR_END                       _CPR_NS_END _CPR_NS_END

#define CPR_CORE_BEGIN_NAMESPACE       _CPR_BEGIN (core)
#define CPR_CORE_END_NAMESPACE         _CPR_END

#define CPR_GUI_BEGIN_NAMESPACE       _CPR_BEGIN (gui)
#define CPR_GUI_END_NAMESPACE         _CPR_END

#define CPR_PLUGIN_BEGIN_NAMESPACE(t)  _CPR_NS_BEGIN (plugin) _CPR_NS_BEGIN (t)
#define CPR_PLUGIN_END_NAMESPACE(t)    _CPR_END _CPR_END

#endif // CPR_GLOBAL_H
