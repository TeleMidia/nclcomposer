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

#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "util/ComposerCore_global.h"

#include <QObject>
#include <QList>
#include <QStack>

#include "model/Entity.h"
#include "model/exception/ParentNotFound.h"
#include "model/exception/EntityNotFound.h"
#include "util/Utilities.h"
using namespace cpr::core;

CPR_CORE_BEGIN_NAMESPACE

class PluginControl;
class MessageControl;
class ProjectControl;
class ProjectReader;

class EditCommand;
class AddCommand;
class DeleteCommand;

/*!
 * \brief A Project is a data structure that will keep all Entities and all
 *      plugin data in a same place.
 */
class COMPOSERCORESHARED_EXPORT Project : public Entity
{
  Q_OBJECT

  // The following classes are "reliable" and can acess the
  // private and protected members of Document.
  friend class cpr::core::PluginControl;
  friend class cpr::core::MessageControl;
  friend class cpr::core::ProjectControl;
  friend class cpr::core::ProjectReader;

  friend class cpr::core::EditCommand;
  friend class cpr::core::AddCommand;
  friend class cpr::core::RemoveCommand;

public:
  /*!
   * \brief Returns the Entity with the unique id __id.
   *
   * \param _id The Unique Identifier of the required entity.
   */
  Entity* getEntityById(const QString &id);
  /*!
   * \brief Returns a list of the entities which are of the type __type.
   * \param _type The entity type required.
   */
  QList<Entity*> getEntitiesbyType(const QString &type);

  /*!
   * \brief Returns the location of the project.
   * \todo This should be a URL.
   */
  QString getLocation();

  /*!
   * \brief Returns the Identifier of this project. A project type says for what
   *  language this project is. Currently, there are suport only for NCL
   *  projects.
   */
  QString getProjectId();

  /*!
   * \brief Returns the type of the project.
   */
  LanguageType getProjectType();
  /*!
   * \brief Returns the specific data of a plugin.
   *
   * \param pluginId
   */
  QByteArray getPluginData(const QString &pluginId);

  /*!
   * \brief Generate a descritor NCLID that are not present in the core.
   */
  QString generateUniqueNCLId(const QString &tagname);
  QList<Entity*> getEntityByAttrId(const QString &id);

  bool isDirty();

signals:
  void dirtyProject(bool isDirty);

protected:
  /*!
   * \brief Constructor.
   * \param parent
   */
  explicit Project(QObject *parent = 0);

  /*!
   * \brief Constructor.
   *
   * \param atts
   * \param parent
   */
  Project(const QMap<QString,QString> &atts, QObject *parent = 0);

  /*!
   * \brief Constructor.
   *
   * \param uniqueId
   * \param atts
   * \param parent
   */
  Project(const QString &uniqueId, const QMap<QString,QString> &atts, QObject *parent = 0);

  /*!
   * \brief init
   */
  void init();

  /*!
   * \brief Destructor;
   */
  ~Project();

  /*!
   * \brief Set the location of the project.
   * \todo The locations should be an URL.
   * \param location the location of the project.
   */
  void setLocation(const QString &location);
  /*!
   * \brief Set the type of the project.s
   * \param type The type of the project. Currently, there are support just for
   *  NCL projects.
   */
  void setProjectType(LanguageType type);


  QString model; /*!< Keeps the model as an string (this will be useful for
                        serialize the model. */
  /*!
   * \brief Replaces the current string that represents the model.
   *
   * \param model The new model as a string.
   */
  void setModelString(const QString &model) {this->model = model;}
  /*!
   * \brief Returns the current model as a string.
   *
   * \return QString the current model as a string.
   */
  QString getModelString() {return this->model;}

  /*!
   * \brief This method is used to add an Entity in the map and as child of
   *        the parentId.
   *
   * \param entity - A Entity to be added
   * \param parentId - A QString identifying the parent to be added
   *                   as a child.
   *
   * \return true if success and false otherwise.
   */
  bool addEntity(Entity* entity, const QString &parentId)
    throw (EntityNotFound, ParentNotFound);

  bool addEntity(Entity *entity, const QString &parentId, Entity *entity_before)
    throw (EntityNotFound, ParentNotFound);

  /*!
   * \brief Sets specific plugin data to project file.
   *
   * \param pluginId the plugin identifier that we want to set
   * \param data
   */
  bool setPluginData(const QString &pluginId, const QByteArray &data);

  /*!
   * \brief This method is used to remove and delete an Entity from the map.
   *
   * \param entity - A Entity to be removed
   * \param appendChild - If true the children of Entity will be
   *                    appended in the parentEntity. If false all the
   *                    children are also deleted.
   * \return an boolean depending on the success
   */
  bool removeEntity(Entity* entity, bool appendChild) throw (EntityNotFound);

  /*!
   * \brief Transforms the current project to a string. This string will contain
   *        the internal model and the specific plugins data.
   */
  QString toString();

  /*!
   * \brief This slot must be called when there are some data (on the plugin
   *        data or on the model) that are not synchronized with the saved file.
   */
  void setDirty(bool isDirty);

private:
  QMutex *lockEntities; /*!< TODO */
  QMutex lockLocation; /*!< TODO */
  QMap<QString, Entity*> entities; /*!< TODO */
  QMap<QString, QByteArray> pluginData; /*!< TODO */

  QString projectLocation; /*!< TODO */
  QString projectName; /*!< TODO */
  LanguageType projectType; /*!< TODO */

  bool dirty;
};

CPR_CORE_END_NAMESPACE

#endif // DOCUMENT_H
