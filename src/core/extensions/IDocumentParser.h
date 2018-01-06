/* Copyright (C) 2011-2018 PUC-Rio/TeleMídia Lab.

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

#ifndef IDOCUMENTPARSER_H
#define IDOCUMENTPARSER_H

#include <QObject>

#include "model/Project.h"
using namespace cpr::core;

CPR_CORE_BEGIN_NAMESPACE

/*!
 * \ingroup core
 * \brief An abstract interface to document parsers.
 */
class COMPOSERCORESHARED_EXPORT IDocumentParser : public QObject
{
  Q_OBJECT

public:
  virtual ~IDocumentParser () {}
  /*!
   * \brief Parses the content of the document. This function must be
   *  implemented by concrete Document Parsers.
   *
   *  \return bool returns true if success and false otherwise.
   */
  virtual bool parseDocument () = 0;

  /*!
   * \brief Parses the content of the string passed as parameter. This function
   *  must be implemented by concrete Document Parsers.
   *
   *  \arg str the content to be parsed.
   *  \return bool returns true if success and false otherwise.
   */
  virtual bool parseContent (const QString &str) = 0;

  /*!
   * \brief Returns the name of the Document Parser. This name should be
   * unique.
   *  This function must be implemented by concrete Document Parsers.
   * \return QString the name of the Document Parser.
   */
  virtual QString parserName () = 0;

public slots:
  /*!
   * \brief This slot is called when there is an error in the model (commonly
   *  after a addEntity call). This function must be implemented by concrete
   *  Document Parsers.
   *
   * \param error The description of the error.
   */
  virtual void onEntityAddError (const QString &error) = 0;

  /*!
   * \brief This slot is called after the addEntity has success. This
   *  function must be implemented by concrete Document Parsers.
   *
   * \param uniqueID the unique id of the entity in the model.
   * \param entity the pointer to the entity.
   */
  virtual void onEntityAdded (const QString &uniqueID, Entity *entity) = 0;

signals:
  /*!
   * \brief Signal emitted when the parser find a new entity. This function
   *  must be implemented by concrete Document Parsers.
   *
   * \param type the type of the entity (XML tagname).
   * \param parentEntityId uniqueId of the father.
   * \param atts attributes of this entity.
   */
  void addEntity (const QString &type, const QString &parentEntityId,
                  const QMap<QString, QString> &atts);
};

CPR_CORE_END_NAMESPACE

#endif // IDOCUMENTPARSER_H
