/* Copyright (C) 2011-2018 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef NCLSTRUCTURE_H
#define NCLSTRUCTURE_H

#include "NCLLanguageProfile_global.h"

#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <QtDebug>
#include <deque>
#include <map>
using namespace std;

#include <util/AttributeReferences.h>
#include <util/Singleton.h>
using namespace cpr::core;

#define NCLSTRUCTURE_FILE ":/config/NCL_STRUCTURE"

CPR_NCLPROFILE_BEGIN_NAMESPACE

/*!
 * \brief Class that keep information about the language description.
 *
 * \todo Improve the parser error reporting.
 */
class NCLLANGUAGEPROFILESHARED_EXPORT NCLStructure
{
  SINGLETON (NCLStructure)

public:
  /*!
   * \brief
   *
   */
  void loadStructure ();
  /*!
   * \brief
   *
   */
  void clear ();

  // primitives
  /*!
   * \brief
   *
   * \param name
   * \param father
   * \param cardinality
   *
   * \todo use const for parameters
   */
  void addElement (const QString &name, const QString &father,
                   char cardinality, bool _define_scope = false);
  /*!
   * \brief
   *
   * \param element
   * \param attr
   * \param type
   * \param required
   *
   * \todo use const for parameters
   */
  void addAttribute (const QString &element, const QString &attr,
                     const QString &type, bool required);
  /*!
   * \brief
   *
   * \param element
   * \param attr
   * \param ref_element
   * \param ref_attr
   * \param scope
   *
   * \todo use const for parameters
   */
  void addReference (const QString &element, const QString &attr,
                     const QString &ref_element, const QString &ref_attr,
                     const QString &scope);

  /*!
   * \brief addDatatype
   *
   * \param datatype
   * \param regex
   *
   * \todo use const for parameters
   */
  void addDatatype (const QString &datatype, const QString &regex);

  /*!
   * \brief addDatatypeDefaultSuggestions
   *
   * \param datatype
   * \param values
   *
   * \todo use const for parameters
   */
  void addDatatypeDefaultSuggestions (const QString &datatype,
                                      const QString &values);

  /*!
   * \brief getDatatypeDefaultSuggestions
   * \param datatype
   * \return
   *
   * \todo use const for parameters
   */
  QStringList getDatatypeDefaultSuggestions (const QString &datatype);

  /*!
   * \brief getNesting
   * \return
   *
   * \todo use const to not allow user to modify the map
   */
  map<QString, map<QString, char> *> *getNesting ();

  /*!
   * \brief
   *
   * \param element
   * \return map<QString, bool>
   *
   * \todo use const for parameters
   * \todo use const to not allow user to modify the map
   */
  map<QString, bool> *getAttributes (const QString &element);

  /*!
   * \brief getAttributesOrdered
   * \param element
   * \return
   *
   * \todo use const for parameters
   * \todo use const to not allow user to modify the deque
   */
  deque<QString> *getAttributesOrdered (const QString &element);

  /*!
   * \brief getChildren
   * \param tagname
   * \return
   *
   * \todo use const for parameters
   * \todo use const to not allow user to modify the map
   */
  map<QString, char> *getChildren (const QString &tagname);

  /*!
   * \brief getElementsOrdered
   * \return
   *
   * \todo use const to not allow user to modify the deque
   */
  deque<QString> *getElementsOrdered ();

  /*!
   * \brief getReferences
   * \param element
   * \param attr
   * \return
   *
   * \todo use const for parameters
   * \todo use const to not allow user to modify the map
   */
  vector<AttributeReferences *> getReferences (const QString &element,
                                               const QString &attr);
  /*!
   * \brief getAttributeDatatype
   *
   * \param element
   * \param name
   * \return
   *
   * \todo use const for parameters
   */
  QString getAttributeDatatype (const QString &element, const QString &name);

  /*!
   * \brief defineScope
   * \param tagname
   * \return
   *
   * \todo use const for parameters
   */
  bool defineScope (const QString &tagname);

private:
  map<QString, map<QString, bool> *> *_attributes;     /**< TODO */
  map<QString, deque<QString> *> *_attributes_ordered; /**< TODO */
  map<QString, map<QString, char> *> *_nesting;        /**< TODO */
  deque<QString> *_elements_ordered;
  map<QString, QString> *_datatypes; /**< TODO */

  map<QString, QStringList> *_datatypeDefaultSuggestions;
  map<QString, map<QString, QString> *> *_attributesDatatype; /**< TODO */
  QMultiMap<QString, AttributeReferences *> *_references;     /**< TODO */
  map<QString, bool> _define_scope;

  /*!
   * \brief Constructor (it is private because this class is a singleton).
   */
  NCLStructure ();

  /*!
   * \brief Destructor
   */
  ~NCLStructure ();

  /*!
   * \brief
   *
   * \param line
   * \return vector<QString>
   */
  QStringList parseLine (const QString &line);
};

CPR_NCLPROFILE_END_NAMESPACE

#endif // NCLSTRUCTURE_H
