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
#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>
#include <deque>
#include <map>
#include <QStringList>
using namespace std;

#include <util/AttributeReferences.h>
using namespace composer::util;
#include <core/util/Singleton.h>

#define NCLSTRUCTURE_FILE ":/config/NCL_STRUCTURE"

namespace composer {
    namespace language{
/*!
 * \brief Class that keep information about the language description.
 *
 */
class NCLLANGUAGEPROFILESHARED_EXPORT NCLStructure
{
    SINGLETON(NCLStructure)

private:
    map <QString, map <QString, bool> *> *attributes; /**< TODO */
    map <QString, deque <QString> *>      *attributes_ordered; /**< TODO */
    map <QString, map <QString, char> *> *nesting; /**< TODO */
    map <QString, QString > *dataTypes; /**< TODO */

    map <QString, QStringList> *dataTypeDefaultSuggestions;
    map <QString, map <QString, QString> *> *attributesDatatype; /**< TODO */
    QMultiMap <QString, AttributeReferences* > *references; /**< TODO */
    map <QString, bool> define_scope;

    /*!
     * \brief Constructor
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
     */
    void addElement (QString name, QString father, char cardinality,
                     bool define_scope = false);
    /*!
     * \brief
     *
     * \param element
     * \param attr
     * \param type
     * \param required
     */
    void addAttribute (QString element,
                       QString attr,
                       QString type,
                       bool required);
    /*!
     * \brief
     *
     * \param element
     * \param attr
     * \param ref_element
     * \param ref_attr
     * \param scope
     */
    void addReference (QString element,
                       QString attr,
                       QString ref_element,
                       QString ref_attr,
                       QString scope);

    /*!
     *
     */
    void addDatatype( QString datatype,
                      QString regex);

    void addDatatypeDefaultSuggestions(QString datatype, QString values);

    QStringList getDatatypeDefaultSuggestions(QString datatype);
    /*!
     * \brief
     *
     * \return map<QString, map<QString, char> *>
     */
    map <QString, map <QString, char> *> *getNesting();
    /*!
     * \brief
     *
     * \param element
     * \return map<QString, bool>
     */
    map <QString, bool> *getAttributes (QString element);
    /*!
     * \brief
     *
     * \param element
     * \return deque<QString>
     */
    deque <QString> *getAttributesOrdered (QString element);
    /*!
     * \brief
     *
     * \param tagname
     * \return map<QString, char>
     */
    map <QString, char> *getChildren (QString tagname);
    /*!
     * \brief
     *
     * \param element
     * \param attr
     * \return vector<AttributeReferences *>
     */
    vector <AttributeReferences *> getReferences (QString element, QString attr);

    /*!
     * \brief
     *
     */
    QString getAttributeDatatype(QString element, QString name);

    /*!
     * \brief
     */
    bool defineScope(QString tagname);
};

}} //end namespace

#endif // NCLSTRUCTURE_H
