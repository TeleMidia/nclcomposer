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

#ifndef ENTITY_H
#define ENTITY_H

#include "util/ComposerCore_global.h"

#include "model/Node.h"

CPR_CORE_BEGIN_NAMESPACE

/*!
 * \ingroup core
 * \brief An Entity is the main class of the internal Composer core.
 *
 * The internal model Composer model is a tree of Entities.
 */
class COMPOSERCORESHARED_EXPORT Entity : public Node
{
  Q_OBJECT

  friend class Project;
  friend class MessageControl;
  friend class ProjectReader;

  friend class EditEntityCmd;
  friend class AddEntityCmd;
  friend class RemoveEntityCmd;

public:
  /*!
   * \brief This method is used to get an specific attribute of the element.
   *
   * \param name - The name of the attribute been requested
   * \return A string with the requested attribute.
   */
  QString attr (const QString &name) const;

  /*!
   * \brief Gets the attributes of the entity.
   *
   * \return a QMap with the attributes of the entity
   */
  QMap<QString, QString> attrs () const;

  /*!
   * \brief This method is used to verify if this element has certain
   *      attribute.
   *
   * \param name - The name of the attribute to be verified.
   * \return an boolean depending of the existence of the attribute.
  */
  bool hasAttr (const QString &name) const;

  QString type () const;

  /*!
   * \brief Converts the current Entity to an XML String.
   *
   * \param ntabs the number of tabs to be inserted before the current entity.
   */
  virtual QString toString (int ntabs, bool writeuid = true) override;

  /*!
   * \brief Creates a clone of the current entity.
   *
   * All the content of the entity will be cloned, including its uniqueId.
   */
  Entity *clone ();

protected:
  /*!
   * \brief Constructor.
   *
   * \param parent The QObject parent.
   */
  explicit Entity (QDomDocument &doc, Entity *parent = 0);

  /*!
   * \brief Constructor.
   *
   * \param atts attributes to be set to this Entity.
   * \param parent the QObject parent.
   */
  explicit Entity (const QMap<QString, QString> &atts, QDomDocument &doc,
                   Entity *parent = 0);

  /*!
   * \brief Contructor.
   *
   * \param uniqueId the uniqueId of this Entiy.
   * \param type the type of the Entity.
   * \param atts attributes to be set to this Entity.
   * \param parent the QObject parent.
   */
  explicit Entity (const QString &uid, const QString &_type,
                   const QMap<QString, QString> &atts, QDomDocument &doc,
                   Entity *parent = 0);

  /*!
   * \brief Destructor.
   */
  virtual ~Entity ();

  /*!
   * \brief This method is used to set an specific attribute of the element
   *
   * \param name - The name of the attribute to be set
   * \param value - The value this attribute is going to be set
   */
  void setAttr (const QString &name, const QString &value);

  /*!
   * \brief Set the attributes of the current entity to the value passed as
   *      parameter.
   *
   * \param newAtts the new value of entity's attributes.
   */
  void setAtrrs (const QMap<QString, QString> &newatts);

  /*!
   * \brief Set the entity's type to the value passed as parameter.
   *
   * \param type The type of the entity.
   */
  void setType (const QString &type);

  /*!
   * \brief
   */
  //  void print ();
};

CPR_CORE_END_NAMESPACE

#endif // ENTITY_H
