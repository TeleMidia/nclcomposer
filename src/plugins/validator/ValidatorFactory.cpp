/*
 * Copyright 2011-2013 Laws/UFMA.
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
 * License along with this library. If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "ValidatorFactory.h"


ValidatorFactory::ValidatorFactory(QWidget *parent)
  : QObject(parent)
{
}

ValidatorFactory::~ValidatorFactory()
{
}

IPlugin * ValidatorFactory::createPluginInstance()
{
  return new ValidatorPlugin();
}

void ValidatorFactory::releasePluginInstance(IPlugin *plugin)
{
  ValidatorPlugin *validator = qobject_cast <ValidatorPlugin *> (plugin);

  if (validator)
  {
    delete validator;
    validator = NULL;
  }
}


QList <LanguageType> ValidatorFactory::getSupportedLanguages()
{
  QList <LanguageType> list;
  list.append(NCL);
  return list;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(ValidatorFactory, ValidatorFactory)
#endif
