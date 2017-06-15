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

#ifndef ENTITY_H
#define ENTITY_H

#include "util/ComposerCore_global.h"

#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include <QMap>
#include <QUuid>
#include <QDebug>
#include <QDomElement>

CPR_CORE_BEGIN_NAMESPACE

class MessageControl;
class ProjectReader;

class Project;

class EditCommand;
class AddCommand;
class RemoveCommand;

/*!
 * \brief An Entity is the main class of the internal Composer core.
 *
 * The internal model Composer model is a tree of Entities.
 */
class COMPOSERCORESHARED_EXPORT Entity: public QObject
{
  Q_OBJECT

  friend class Project;
  friend class MessageControl;
  friend class ProjectReader;

  friend class EditCommand;
  friend class AddCommand;
  friend class RemoveCommand;

public:
  /*!
   * \brief This method is used to get an specific attribute of the element.
   *
   * \param name - The name of the attribute been requested
   * \return A string with the requested attribute.
   */
  QString getAttribute(const QString &name) const;

  /*!
   * \brief Gets the attributes of the entity.
   *
   * \return a QMap with the attributes of the entity
   */
  QMap<QString, QString> getAttributes() const;

  /*!
   * \brief This method is used to verify if this element has certain
   *      attribute.
   *
   * \param name - The name of the attribute to be verified.
   * \return an boolean depending of the existence of the attribute.
  */
  bool hasAttribute(const QString &name) const;

  QString getUniqueId() const;

  QString getType() const;

  Entity* getParent() const;

  QString getParentUniqueId() const;

  /*!
   * \brief Tell if the children should be deleted when this entity is deleted
   *        through destructor.
   *
   * \param mustDelete tell if the children also must be deleted.
   */
  void setDeleteChildren(bool mustDelete);

  /*!
   * \brief
   * \return
   */
  QVector <Entity *> getChildren() const;

  /*!
   * \brief Converts the current Entity to an XML String.
   *
   * \param ntabs the number of tabs to be inserted before the current entity.
   */
  QString toString(int ntabs, bool writeuid = true);

  /*!
   * \brief Creates a clone of the current entity.
   *
   * All the content of the entity will be cloned, including its uniqueId.
   */
  Entity *cloneEntity();

protected:
  /*!
   * \brief Constructor.
   *
   * \param parent The QObject parent.
   */
  explicit Entity(QDomDocument &doc, Entity *parent=0);

  /*!
   * \brief Constructor.
   *
   * \param atts attributes to be set to this Entity.
   * \param parent the QObject parent.
   */
  explicit Entity(const QMap<QString,QString> &atts,
                  QDomDocument &doc,
                  Entity *parent=0);

  /*!
   * \brief Contructor.
   *
   * \param uniqueId the uniqueId of this Entiy.
   * \param type the type of the Entity.
   * \param atts attributes to be set to this Entity.
   * \param parent the QObject parent.
   */
  explicit Entity(const QString &uniqueId,
                  const QString &_type,
                  const QMap<QString,QString> &atts,
                  QDomDocument &doc,
                  Entity *parent=0);

  /*!
   * \brief Destructor.
   */
  virtual ~Entity();

  /*!
   * \brief This method is used to set an specific attribute of the element
   *
   * \param name - The name of the attribute to be set
   * \param value - The value this attribute is going to be set
   */
  void setAttribute(const QString &name, const QString &value);

  /*!
   * \brief Set the attributes of the current entity to the value passed as
   *      parameter.
   *
   * \param newAtts the new value of entity's attributes.
   */
  void setAtrributes(const QMap<QString,QString> &newatts);

  /*!
   * \brief Set the entity's type to the value passed as parameter.
   *
   * \param type The type of the entity.
   */
  void setType(const QString &_type);

  /*!
   *  \brief Set the unique identifier of the entity to the value passed as
   *      parameter.
   *
   * \param _id The new uniqueId of the entity.
   */
  void setUniqueID(const QString &uniqueId);

  /*!
   * \brief Set the parent of the Entity.
   *
   * \param parent The new parent of this Entity.
   */
  void setParent(Entity *parent);

  // OBS: This method updates the parent reference
  bool addChild(Entity *entity, int pos = -1);

  /*!
   * \brief This call deletes the child and his children in a recursive way.
   *
   * \param entity The Entity child to be removed.
   */
  bool deleteChild(Entity *entity);
  /*!
   * \brief
   */
  void print();

private:
  QDomDocument _doc;
  QDomElement _element;
  QString _id;
  Entity* parent;
  bool deleteChildren = true;

  /*!
   * \brief children is a list of Entity that is children than this Entity.
   */
  QVector <Entity*> children;
};

CPR_CORE_END_NAMESPACE

#endif // ENTITY_H
