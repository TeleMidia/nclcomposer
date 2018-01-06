/* Copyright (C) 2011-2018 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef NCLLANGUAGEPROFILE_GLOBAL_H
#define NCLLANGUAGEPROFILE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(NCLLANGUAGEPROFILE_MAKE_DLL)
#define NCLLANGUAGEPROFILESHARED_EXPORT Q_DECL_EXPORT
#else
#define NCLLANGUAGEPROFILESHARED_EXPORT Q_DECL_IMPORT
#endif

#define CPR_NCLPROFILE_BEGIN_NAMESPACE _CPR_BEGIN (nclprofile)
#define CPR_NCLPROFILE_END_NAMESPACE _CPR_END

#endif // NCLLANGUAGEPROFILE_GLOBAL_H
