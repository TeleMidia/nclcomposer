/* Copyright (c) 2011 Telemidia/PUC-Rio.
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

#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <deque>
#include <map>
#include <QStringList>
using namespace std;

#include <util/AttributeReferences.h>
using namespace composer::util;
#include <util/Singleton.h>

#define NCLSTRUCTURE_FILE ":/config/NCL_STRUCTURE"

namespace composer {
    namespace language {
/*!
 * \brief Class that keep information about the language description.
 *
 */
class NCLLANGUAGEPROFILESHARED_EXPORT NCLStructure
{
  SINGLETON(NCLStructure)

public:
  /*!
   * \brief
   *
   */
  void loadStructure();
  /*!
   * \brief
   *
   */
  void clear();

  //primitives
  /*!
   * \brief
   *
   * \param name
   * \param father
   * \param cardinality
   *
   * \todo use const for parameters
   */
  void addElement ( QString name,
                    QString father,
                    char cardinality,
                    bool define_scope = false );
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
  void addAttribute ( QString element,
                      QString attr,
                      QString type,
                      bool required );
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
  void addReference ( QString element,
                      QString attr,
                      QString ref_element,
                      QString ref_attr,
                      QString scope );

  /*!
   * \brief addDatatype
   *
   * \param datatype
   * \param regex
   *
   * \todo use const for parameters
   */
  void addDatatype( QString datatype, QString regex );

  /*!
   * \brief addDatatypeDefaultSuggestions
   *
   * \param datatype
   * \param values
   *
   * \todo use const for parameters
   */
  void addDatatypeDefaultSuggestions(QString datatype, QString values);

  /*!
   * \brief getDatatypeDefaultSuggestions
   * \param datatype
   * \return
   *
   * \todo use const for parameters
   */
  QStringList getDatatypeDefaultSuggestions(QString datatype);

  /*!
   * \brief getNesting
   * \return
   *
   * \todo use const to not allow user to modify the map
   */
  map <QString, map <QString, char> *> *getNesting();

  /*!
   * \brief
   *
   * \param element
   * \return map<QString, bool>
   *
   * \todo use const for parameters
   * \todo use const to not allow user to modify the map
   */
  map <QString, bool> *getAttributes (QString element);

  /*!
   * \brief getAttributesOrdered
   * \param element
   * \return
   *
   * \todo use const for parameters
   * \todo use const to not allow user to modify the deque
   */
  deque <QString> *getAttributesOrdered (QString element);

  /*!
   * \brief getChildren
   * \param tagname
   * \return
   *
   * \todo use const for parameters
   * \todo use const to not allow user to modify the map
   */
  map <QString, char> *getChildren (QString tagname);

  /*!
   * \brief getElementsOrdered
   * \return
   *
   * \todo use const to not allow user to modify the deque
   */
  deque <QString> *getElementsOrdered();

  /*!
   * \brief getReferences
   * \param element
   * \param attr
   * \return
   *
   * \todo use const for parameters
   * \todo use const to not allow user to modify the map
   */
  vector <AttributeReferences *> getReferences (QString element, QString attr);
  /*!
   * \brief getAttributeDatatype
   *
   * \param element
   * \param name
   * \return
   *
   * \todo use const for parameters
   */
  QString getAttributeDatatype(QString element, QString name);

  /*!
   * \brief defineScope
   * \param tagname
   * \return
   *
   * \todo use const for parameters
   */
  bool defineScope(QString tagname);

private:
  map <QString, map <QString, bool> *>  *attributes; /**< TODO */
  map <QString, deque <QString> *>      *attributes_ordered; /**< TODO */
  map <QString, map <QString, char> *>  *nesting; /**< TODO */
  deque <QString>                       *elements_ordered;
  map <QString, QString >               *dataTypes; /**< TODO */

  map <QString, QStringList>                  *dataTypeDefaultSuggestions;
  map <QString, map <QString, QString> *>     *attributesDatatype; /**< TODO */
  QMultiMap <QString, AttributeReferences* >  *references; /**< TODO */
  map <QString, bool>                         define_scope;

  /*!
   * \brief Constructor (it is private because this class is a singleton).
   */
  NCLStructure();

  /*!
   * \brief Destructor
   */
  ~NCLStructure();

  /*!
   * \brief
   *
   * \param line
   * \return vector<QString>
   */
  vector <QString> parseLine(QString line);
};

}} //end namespace

#endif // NCLSTRUCTURE_H
