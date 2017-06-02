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

#include "Preferences.h"

#include "ComposerSettings.h"
#include "Utilities.h"
using namespace cpr::core;

#include <QSet>
#include <QDebug>

CPR_CORE_BEGIN_NAMESPACE

Preference::Preference (const QString &name,
                        QVariant::Type type,
                        const QVariant &defaultValue,
                        const QString &category):
  _name(name), _type(type), _category(category)
{
  _value = defaultValue;
  _defaultValue = defaultValue;
}


INIT_SINGLETON(Preferences)

Preferences::Preferences()
{
}

Preferences::~Preferences()
{
  qDeleteAll( _preferences );
  _preferences.clear();
}

void Preferences::setValue(const QString &key, const QVariant &value)
{
  _preferences.value(key)->setValue(value);
}

Preference* Preferences::getValue(const QString &key) const
{
  return _preferences.value(key);
}

void Preferences::registerPreference(const QString &key, Preference *preference)
{
  if (_preferences.contains(key))
    qCWarning(CPR_CORE) <<  "Trying to register an already registered "
                         << key << "preference.  It will be ignored!";

  Q_ASSERT(!_preferences.contains(key));
  _preferences.insert(key, preference);
}

void Preferences::restore() const
{
  GlobalSettings settings;
  settings.beginGroup("preferences");
  for (const QString& key : _preferences.keys())
  {
    if (settings.contains(key))
    {// options of the preference object are hard coded and never change.
      QVariant value = settings.value(key);
      _preferences[key]->setValue(value);
    }
  }
  settings.endGroup();    //Preferences
}

void Preferences::save() const
{
  GlobalSettings settings;
  settings.beginGroup("preferences");
  for (const QString& key : _preferences.keys())
  {
    settings.setValue(key, _preferences[key]->value());
  }
  settings.endGroup();
}

QStringList Preferences::categories() const
{
  QSet<QString> categories;
  for (const QString& key : _preferences.keys())
  {
    categories << _preferences[key]->category();
  }
  categories.remove("");

  QStringList sortedCategories = categories.toList();
  std::sort(sortedCategories.begin(), sortedCategories.end());

  return sortedCategories;
}

QList<Preference *> Preferences::preferences(const QString& category) const
{
  QList<Preference*> preferences;
  for (const QString& key : _preferences.keys())
  {
    if (_preferences[key]->category() == category)
    {
      preferences << _preferences[key];
    }
  }

  return preferences;
}

CPR_CORE_END_NAMESPACE
