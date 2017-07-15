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

#ifndef PREFERENCE_H
#define PREFERENCE_H

#include "ComposerCore_global.h"
#include "Singleton.h"

#include <QMap>
#include <QVariant>

CPR_CORE_BEGIN_NAMESPACE

/*!
 * \ingroup core
 * \brief The Preference class
 */
class COMPOSERCORESHARED_EXPORT Preference
{
private:
  QString _name;
  QString _category;
  QVariant _value, _defaultValue;

public:
  Preference (const QString &name,
              const QVariant &defaultValue = QVariant (""),
              const QString &category = "");

  void
  setValue (const QVariant &value)
  {
    this->_value = value;
  }

  QVariant
  value ()
  {
    return _value;
  }

  QString
  category ()
  {
    return _category;
  }

  QString
  key ()
  {
    return "cpr."+_category.toLower()+"."+_name;
  }
};

/*!
 * \ingroup core
 * \brief The Preferences class
 */
class COMPOSERCORESHARED_EXPORT Preferences
{
  SINGLETON (Preferences)

public:
  /*!
   * \brief gets the value of a preference. If the preference is not registered
   *        it will return a qvariant with isNull() == true
   * \param id the identifier of the preference
   * \return the value of the preference
   */
  QVariant get (const QString &id);

  void save () const;
  void restore () const;

  void registerPreference (const QString &key, Preference *preference);

  Preference *getValue (const QString &key) const;
  void setValue (const QString &key, const QVariant &value);

  QStringList categories () const;
  QList<Preference *> preferences (const QString &category) const;

private:
  Preferences ();
  ~Preferences ();

  QMap<QString, Preference *> _preferences;
  bool _enableSaveRestore;
};

CPR_CORE_END_NAMESPACE

#endif
