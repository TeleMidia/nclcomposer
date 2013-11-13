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
#include "DebugConsoleFactory.h"

DebugConsoleFactory::DebugConsoleFactory()
{

}

DebugConsoleFactory::~DebugConsoleFactory()
{

}

IPlugin* DebugConsoleFactory::createPluginInstance()
{
  return new DebugConsolePlugin();
}

void DebugConsoleFactory::releasePluginInstance(IPlugin *plugin)
{
  DebugConsolePlugin *debug = qobject_cast<DebugConsolePlugin*>(plugin);

  if (debug)
    {
      delete debug;
      debug = NULL;
    }

}

QList<LanguageType> DebugConsoleFactory::getSupportedLanguages()
{
  QList<LanguageType> lTypes;
  lTypes.append(NCL);
  return lTypes;
}

QString DebugConsoleFactory::id() const
{
  return "br.puc-rio.telemidia.DebugConsole";
}

QIcon DebugConsoleFactory::icon()  const
{
  return QIcon(":/images/icon.png");
}

QWidget* DebugConsoleFactory::getPreferencePageWidget()
{
  return NULL;
}

void DebugConsoleFactory::setDefaultValues()
{

}

void DebugConsoleFactory::applyValues()
{

}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(DebugConsole,DebugConsoleFactory)
#endif
