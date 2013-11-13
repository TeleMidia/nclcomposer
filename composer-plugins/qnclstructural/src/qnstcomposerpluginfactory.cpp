/*
 * Copyright 2011-2013 TeleMidia/PUC-Rio.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

//
// ATTENTION: This code needs a refactoring.
//

#include "qnstcomposerpluginfactory.h"

QnstComposerPluginFactory::QnstComposerPluginFactory(QObject* parent)
{
  setParent(parent);
}

QnstComposerPluginFactory::~QnstComposerPluginFactory()
{

}

IPlugin* QnstComposerPluginFactory::createPluginInstance()
{
  return new QnstComposerPlugin();
}

void QnstComposerPluginFactory::releasePluginInstance(IPlugin* plugin)
{
  delete(plugin);
}

QString QnstComposerPluginFactory::id() const
{
  return "br.puc-rio.telemidia.qnst";
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Qnst, QnstComposerPluginFactory)
#endif
