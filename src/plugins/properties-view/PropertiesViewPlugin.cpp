/*
 * Copyright 2011 TeleMidia/PUC-Rio.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include "PropertiesViewPlugin.h"
#include "modules/LanguageControl.h"

#include <QDockWidget>

PropertiesViewPlugin::PropertiesViewPlugin ()
{
  _window = new PropertiesEditor (0);
  project = nullptr;
  _currentEntity = nullptr;

  connect (_window, SIGNAL (propertyChanged (QString, QString)),
           this, SLOT (updateCurrentEntityAttr (QString, QString)));
}

PropertiesViewPlugin::~PropertiesViewPlugin ()
{
  /*!  \todo Review PropertiesViewPlugin::~PropertiesViewPlugin().
             Why not delete 'window'? Memory leak? */
  // delete window;
}

QWidget *
PropertiesViewPlugin::getWidget ()
{
  return _window;
}

void
PropertiesViewPlugin::onEntityAdded (const QString &pluginID, Entity *entity)
{
  Q_UNUSED (pluginID)
  Q_UNUSED (entity)
}

void
PropertiesViewPlugin::errorMessage (const QString &error)
{
  qDebug () << "PropertiesViewPlugin::onEntityAddError(" << error << ")";
}

void
PropertiesViewPlugin::onEntityChanged (const QString &pluginID, Entity *entity)
{
  Q_UNUSED (pluginID)

  if (pluginID == this->getPluginInstanceID())
    return;

  if (entity != nullptr && _currentEntity != nullptr)
  {
    if (entity->getUniqueId () == _currentEntity->getUniqueId ())
      updateCurrentEntity ();
  }
}

void
PropertiesViewPlugin::onEntityRemoved (const QString &pluginID,
                                       const QString &entityID)
{
  Q_UNUSED (pluginID)

  if (entityID == _currentEntityId)
  {
    _currentEntity = nullptr;
    _window->setTagname ("", "");
    _currentEntityId = "";
  }
}

bool
PropertiesViewPlugin::saveSubsession ()
{
  /*!< TODO: Implement PropertiesViewPlugin::saveSubsession() */
  return true;
}

void
PropertiesViewPlugin::init ()
{
  /*!< TODO: Implement PropertiesViewPlugin::init() */
}

void
PropertiesViewPlugin::changeSelectedEntity (QString pluginID, void *param)
{
  Q_UNUSED (pluginID)

  QString *id = (QString *)param;
  if (id != nullptr && *id != "")
  {
    _currentEntity = project->getEntityById (*id);
    _currentEntityId = *id;
  }

  if (_currentEntity != nullptr)
  {
    updateCurrentEntity ();
  }

  emit sendBroadcastMessage ("askAllValidationMessages", nullptr);
}

void
PropertiesViewPlugin::updateCurrentEntity (QString errorMessage)
{
  QString tagname = _currentEntity->getType ();
  QString name;
  if (_currentEntity->hasAttribute ("id"))
    name = _currentEntity->getAttribute ("id");
  else if (_currentEntity->hasAttribute ("name"))
    name = _currentEntity->getAttribute ("name");
  else
    name = "Unknown";

  _window->setTagname (tagname,
                       name,
                       getAttributes (tagname),
                       getAttributesDatatype (tagname),
                       getAttributeSuggestions (tagname));

  _window->setErrorMessage (errorMessage);

  QMap<QString, QString> attrs = _currentEntity->getAttributes ();
  foreach (const QString &key, attrs.keys())
  {
    _window->setAttributeValue (key, attrs[key]);
  }
}

void
PropertiesViewPlugin::updateCurrentEntityAttr (QString attr, QString value)
{
  if (_currentEntity != nullptr)
  {
    if (_currentEntity->hasAttribute (attr)
        && _currentEntity->getAttribute (attr) == value)
    {
      // do nothing
      return;
    }
    else
    {
      QMap<QString, QString> attrs = _currentEntity->getAttributes ();
      attrs[attr] = value;

      if (value.isNull() || value.isEmpty() || value == "")
      {
        attrs.remove(attr);
      }
      else
      {
        try
        {
          if (NCLStructure::getInstance ()->getAttributeDatatype (
                _currentEntity->getType (), attr) == "URI")
          {
            attrs[attr] = Utilities::relativePath (project->getLocation (),
                                                 value, true);
          }
        }
        catch(...)
        {
          qDebug () << "Do not changing to a relative path";
        }
      }

      emit setAttributes (_currentEntity, attrs);
    }
  }
}

void
PropertiesViewPlugin::validationError (QString pluginID, void *param)
{
  Q_UNUSED (pluginID)

  if (param)
  {
    pair<QString, QString> *p = (pair<QString, QString> *)param;

    QString uid = p->first;

    if (_currentEntity == project->getEntityById (uid))
      updateCurrentEntity (p->second);
  }
}

QStringList
PropertiesViewPlugin::getAttributes (const QString &tagname)
{
  QStringList attrs_list;
  NCLStructure *structure = NCLStructure::getInstance ();
  deque<QString> *attrs = structure->getAttributesOrdered (tagname);

  if (attrs != nullptr)
  {
    for (auto it = attrs->begin (); it != attrs->end (); ++it)
    {
      attrs_list << *(it);
    }
  }

  return attrs_list;
}

QStringList
PropertiesViewPlugin::getAttributesDatatype (const QString &tagname)
{
  QStringList datatypes;

  NCLStructure *structure = NCLStructure::getInstance ();
  foreach (const QString &attr, getAttributes (tagname))
  {
    datatypes << structure->getAttributeDatatype (tagname, attr);
  }

  return datatypes;
}

QList<QStringList>
PropertiesViewPlugin::getAttributeSuggestions (const QString &tagname)
{
  QList<QStringList> suggestions;
  NCLStructure *structure = NCLStructure::getInstance ();

  // \todo References
  foreach (const QString &attr, getAttributes (tagname))
  {
    // first, we get the default datatype suggestions
    QString datatype = structure->getAttributeDatatype (tagname, attr);
    QStringList attr_suggestions =
        structure->getDatatypeDefaultSuggestions (datatype);

    // then, we get the values that are references to other elements in the
    // document
    vector <AttributeReferences *> references
        = structure->getReferences(tagname, attr);

    for (AttributeReferences *ref : references)
    {
       QString ref_tagname = ref->getRefElement();
       QString ref_attr = ref->getRefAttribute();
       AttributeReferences::REFERENCE_SCOPE scope = ref->getScope ();

       // Find the scope we should search the entity
       Entity *entityTargetScope = nullptr;
       if (scope == AttributeReferences::ANY_SCOPE)
       {
         // do nothing
       }
       else if (scope == AttributeReferences::SAME_SCOPE)
       {
         // Get the scope of the current entity
         entityTargetScope = dynamic_cast<Entity *>(_currentEntity->getParent());
         while (entityTargetScope->getParent()
                && !structure->defineScope (entityTargetScope->getType()))
         {
           entityTargetScope = dynamic_cast <Entity *> (entityTargetScope->getParent());
         }
       }
       else if (scope == AttributeReferences::USERDEFINED_SCOPE)
       {
         QString attr_scope;
         QString userDefinedScope = ref->getUserDefinedScope ();

         QString idEntityScope = "";
         if (userDefinedScope.startsWith ("$THIS"))
         {
           attr_scope = userDefinedScope.mid (6);
           idEntityScope = _currentEntity->getAttribute (attr_scope);
         }
         else if (userDefinedScope.startsWith ("$PARENT"))
         {
           attr_scope = userDefinedScope.mid (8);
           idEntityScope = static_cast <Entity *> (_currentEntity->getParent())->getAttribute(attr_scope);
         }
         else if (userDefinedScope.startsWith ("$GRANDPARENT"))
         {
           attr_scope = userDefinedScope.mid (13);
           idEntityScope = static_cast <Entity *> (_currentEntity->getParent()->getParent())->getAttribute(attr_scope);
         }

         QList <Entity *> ents = project->getEntityByAttrId (idEntityScope);
         if (ents.size())
          entityTargetScope = ents.at(0);
       }

       // Now, search for the entities in the required scope
       QList<Entity *> entities = this->project->getEntitiesbyType (ref_tagname);
       foreach (Entity *ent, entities)
       {
         if (ent != _currentEntity)
         {
           Entity *parent_scope = dynamic_cast <Entity *> (ent->getParent());
           while (parent_scope->getParent()
                  && !structure->defineScope (parent_scope->getType()))
           {
             parent_scope = dynamic_cast <Entity *> (parent_scope->getParent());
           }

           if (ent->hasAttribute(ref_attr))
           {
             if ((scope == AttributeReferences::ANY_SCOPE) // ANY_SCOPE
                 || (parent_scope != nullptr
                     && parent_scope == entityTargetScope) )

               attr_suggestions << ent->getAttribute(ref_attr);
           }
         }
       }
    }

    suggestions << attr_suggestions;
  }

  return suggestions;
}
