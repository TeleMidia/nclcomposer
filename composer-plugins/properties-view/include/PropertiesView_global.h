/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef PROPERTIESVIEW_GLOBAL_H
#define PROPERTIESVIEW_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PROPERTIESVIEW_LIBRARY)
#  define PROPERTIESVIEWSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PROPERTIESVIEWSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PROPERTIESVIEW_GLOBAL_H
