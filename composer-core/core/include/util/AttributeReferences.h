/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef ATTRIBUTEREFERENCES_H
#define ATTRIBUTEREFERENCES_H
#include "ComposerCoreControl_global.h"

#include <QString>
#include <QMap>

namespace composer {
  namespace util {
/*!
 * \brief Keep information about which pair (element, attribute) can refer a
 *    value of another pair (element, attribute).
 *
 * This class is mainly used to described the language profile.
 */
class COMPOSERCORESHARED_EXPORT AttributeReferences
{
public:
  enum REFERENCE_SCOPE {
    ANY_SCOPE = 0,
    SAME_SCOPE,
    USERDEFINED_SCOPE
  };

private:
  QString element, attr, /*!< TODO */
          ref_element, ref_attr; /*!< TODO */

  REFERENCE_SCOPE scope;
  QString userDefinedScope;
  QMap <QString, REFERENCE_SCOPE> stringToScope;

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
  AttributeReferences( QString element,
                       QString attr,
                       QString ref_element,
                       QString ref_attr,
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
  AttributeReferences( QString element,
                       QString attr,
                       QString ref_element,
                       QString ref_attr,
                       QString scope);
  /*!
   * \brief Get the source element.
   *
   * \return QString the name of the source element.
   */
  QString getElement();
  /*!
   * \brief Get the source attribute.
   *
   * \return QString the name of the source attribute.
   */
  QString getAttribute();
  /*!
   * \brief Get the name of referred element.
   *
   * \return QString The name of the referred element.
   */
  QString getRefElement();
  /*!
   * \brief Get the name of the referred attribute (an ref_element's attribute).
   *
   * \return QString The name of the referred attribute.
   */
  QString getRefAttribute();
  /*!
   * \brief Sets the scope of the object as a string.
   *
   * \return QString the name of the scope.
   */
  void setScope(QString scope);
  /*!
   * \brief Gets the scope of the object as a REFERENCE_SCOPE.
   *
   * \return REFERENCE_SCOPE the type of the scope.
   */
  REFERENCE_SCOPE getScope();
  /*!
   * \brief Gets the string where the user define the scope.
   *
   * \return QString the user defined scope.
   */
  QString getUserDefinedScope();

private:
  void initializeStringToScope();
};

}} //end namespace
#endif
