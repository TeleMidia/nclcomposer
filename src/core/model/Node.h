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

#ifndef NODE_H
#define NODE_H

#include "util/ComposerCore_global.h"

#include <QDebug>
#include <QDomElement>
#include <QMap>
#include <QMutex>
#include <QMutexLocker>
#include <QObject>
#include <QUuid>

CPR_CORE_BEGIN_NAMESPACE

class MessageControl;
class ProjectReader;

class Project;
class Entity;
class EditEntityCmd;
class AddEntityCmd;
class RemoveEntityCmd;

/*!
 * \ingroup core
 * \brief A Node is a common root for all the core model classes.
 *
 * The internal model Composer model is a tree of Entities.
 */
class COMPOSERCORESHARED_EXPORT Node : public QObject
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
   * \brief uniqueId
   * \return the unique id of the node.
   */
  QString uid () const;

  /*!
   * \brief getParent
   * \return the node parent of the node.  Returns nullptr if the node has no
   *  parent.
   */
  Node *parent () const;

  /*!
   * \brief getParentUniqueId
   * \return the parent unique id.
   */
  QString parentUid () const;

  /*!
   * \brief Tell if the children should be deleted when this entity is deleted
   *        through destructor.
   *
   * \param mustDelete tell if the children also must be deleted.
   */
  void setDeleteChildren (bool mustDelete);

  /*!
   * \brief
   * \return
   */
  QList<Node *> children () const;

  /*!
   * \brief getEntityChildren
   * \return only the list of entity children (ignoring the comments).
   */
  QList<Entity *> entityChildren () const;

  /*!
   * \brief Converts the current Entity to an XML String.
   *
   * \param ntabs the number of tabs to be inserted before the current entity.
   */
  virtual QString toString (int ntabs = 0, bool writeuid = false) = 0;

protected:
  QDomDocument _doc;
  QDomNode _domNode;
  QString _uid;
  Node *_parent;
  bool _deleteChildren = true;

  /*!
   * \brief children is a list of Entity that is children than this Entity.
   */
  QList<Node *> _children;

  /*!
   * \brief Constructor.
   *
   * \param parent The QObject parent.
   */
  explicit Node (QDomDocument &doc, Node *parent = 0);

  /*!
   * \brief Contructor.
   *
   * \param uniqueId the uniqueId of this Entiy.
   * \param type the type of the Entity.
   * \param atts attributes to be set to this Entity.
   * \param parent the QObject parent.
   */
  explicit Node (const QString &uid, QDomDocument &doc, Node *parent = 0);

  /*!
   * \brief Destructor.
   */
  virtual ~Node ();

  /*!
   *  \brief Set the unique identifier of the entity to the value passed as
   *      parameter.
   *
   * \param _id The new uniqueId of the entity.
   */
  void setUid (const QString &uid);

  /*!
   * \brief Set the parent of the Entity.
   *
   * \param parent The new parent of this Entity.
   */
  void setParent (Node *_parent);

  // OBS: This method updates the parent reference
  bool addChild (Node *node, int pos = -1);

  /*!
   * \brief This call deletes the child and his children in a recursive way.
   *
   * \param entity The Entity child to be removed.
   */
  bool deleteChild (Node *node);
  /*!
   * \brief
   */
  void print ();
};

CPR_CORE_END_NAMESPACE

#endif // NODE_H
