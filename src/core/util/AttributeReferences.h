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

#ifndef ATTRIBUTEREFERENCES_H
#define ATTRIBUTEREFERENCES_H
#include "ComposerCore_global.h"

#include <QMap>
#include <QString>

CPR_CORE_BEGIN_NAMESPACE

/*!
 * \ingroup core
 * \brief Keep information about which pair (element, attribute) can refer a
 *    value of another pair (element, attribute).
 *
 * This class is mainly used to described the language profile.
 */
class COMPOSERCORESHARED_EXPORT AttributeReferences
{
public:
  enum REFERENCE_SCOPE
  {
    ANY_SCOPE = 0,
    SAME_SCOPE,
    USERDEFINED_SCOPE
  };

public:
  /*!
   * \brief Constructor.
   *
   * \param element The source Element.
   * \param attr The source Attribute (should be an element's attribute).
   * \param ref_element The target element.
   * \param ref_attr The target attribute (should be an ref_element's
   *    attribute).
   * \param scope Which scope.
   */
  AttributeReferences (const QString &elt, const QString &attr,
                       const QString &ref_elt, const QString &ref_attr,
                       REFERENCE_SCOPE scope = ANY_SCOPE);
  /*!
   * \brief Constructor.
   *
   * \param element The source Element.
   * \param attr The source Attribute (should be an element's attribute).
   * \param ref_element The target element.
   * \param ref_attr The target attribute (should be an ref_element's
   *                    attribute).
   * \param scope Which scope (as a string).
   */
  AttributeReferences (const QString &elt, const QString &attr,
                       const QString &ref_elt, const QString &ref_attr,
                       const QString &scope);
  /*!
   * \brief Get the source element.
   *
   * \return QString the name of the source element.
   */
  QString element ();
  /*!
   * \brief Get the source attribute.
   *
   * \return QString the name of the source attribute.
   */
  QString attribute ();
  /*!
   * \brief Get the name of referred element.
   *
   * \return QString The name of the referred element.
   */
  QString refElement ();
  /*!
   * \brief Get the name of the referred attribute (an ref_element's
   * attribute).
   *
   * \return QString The name of the referred attribute.
   */
  QString refAttribute ();
  /*!
   * \brief Sets the scope of the object as a string.
   *
   * \return QString the name of the scope.
   */
  void setScope (const QString &_scope);
  /*!
   * \brief Gets the scope of the object as a REFERENCE_SCOPE.
   *
   * \return REFERENCE_SCOPE the type of the scope.
   */
  REFERENCE_SCOPE getScope ();
  /*!
   * \brief Gets the string where the user define the scope.
   *
   * \return QString the user defined scope.
   */
  QString userDefinedScope ();

private:
  QString _elt, _attr, _ref_elt, _ref_attr; /*!< TODO */

  REFERENCE_SCOPE _scope;
  QString _userDefinedScope;
  QMap<QString, REFERENCE_SCOPE> _str2scope;

private:
  void initStr2Scope ();
};

CPR_CORE_END_NAMESPACE

#endif
