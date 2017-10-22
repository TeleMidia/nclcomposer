#include "StructuralView.h"

#include <QDebug>
#include <QMessageBox>
#include <assert.h>

#include <QClipboard>
#include <QFileDialog>
#include <QMimeData>
#include <QUuid>

#include "StructuralUtil.h"
#include "util/Utilities.h"

StructuralView::StructuralView (QWidget *parent) : QGraphicsView (parent),
  _minimap (new StructuralMinimap (this)),
  _menu (new StructuralMenu (this)),
  _dialog (new StructuralLinkDialog (this)),
  _scene (new StructuralScene (_menu, this))
{
  _mode = Structural::Pointing;
  _zoom = ZOOM_ORIGINAL;

  _linking = false;
  _linkingTail = nullptr;
  _linkingHead = nullptr;

  _selected = "";

  _clipboard = nullptr;
  _tool = nullptr;

  setAttribute (Qt::WA_TranslucentBackground);
  setAcceptDrops (true);
  // setDragMode(ScrollHandDrag);

  // Initialize minimap...
  _minimap->init (this);
  _minimap->setMinimumSize (STR_DEFAULT_MINIMAP_W, STR_DEFAULT_MINIMAP_H);
  _minimap->setMaximumSize (STR_DEFAULT_MINIMAP_W * 2,
                            STR_DEFAULT_MINIMAP_H * 2);
  _minimap->show ();

  // Setting...
  setScene (_scene);
  centerOn (0, 0);
}

StructuralView::~StructuralView ()
{
  // TODO
}

StructuralScene *
StructuralView::getScene ()
{
  return _scene;
}

StructuralEntity *
StructuralView::getBody ()
{
  StructuralEntity *entity = nullptr;

  foreach (StructuralEntity *e, _entities.values ())
  {
    if (e->getType () == Structural::Body)
    {
      entity = e;
      break;
    }
  }

  return entity;
}

StructuralLinkDialog *
StructuralView::getDialog ()
{
  return _dialog;
}

bool
StructuralView::hasEntity (const QString &uid)
{
  return _entities.contains (uid);
}

StructuralEntity *
StructuralView::getEntity (const QString &uid)
{
  return _entities.value (uid, nullptr);
}

QMap<QString, StructuralEntity *>
StructuralView::getEntities ()
{
  return _entities;
}

StructuralMode
StructuralView::getMode () const
{
  return _mode;
}

void
StructuralView::setMode (StructuralMode mode)
{
  _mode = mode;
}

void
StructuralView::setError (const QString &uid, const QString &error)
{
  if (_entities.contains (uid))
  {
    StructuralEntity *entity = _entities.value (uid);
    entity->setError (error);
    entity->adjust (true);
  }
}

void
StructuralView::cleanErrors ()
{
  foreach (StructuralEntity *e, _entities.values ())
    e->setError ("");
}

void
StructuralView::load (const QString &data)
{
  QDomDocument *dom = new QDomDocument ();
  dom->setContent (data);

  QDomElement root = dom->firstChildElement ();
  QDomNodeList rootChildren = root.childNodes ();

  // Cheking if exist a 'body' entity when 'body'
  // is enabled in view. If don't, adds one.
  if (STR_DEFAULT_WITH_BODY)
  {
    bool hasBody = false;

    for (int i = 0; i < rootChildren.length (); i++)
    {
      if (rootChildren.item (i).isElement ())
      {
        QDomElement element = rootChildren.item (i).toElement ();

        if (element.nodeName () == "entity")
        {
          if (element.attributeNode ("type").nodeValue ()
              == StructuralUtil::typeToString (Structural::Body))
          {
            hasBody = true;
            break;
          }
        }
      }
    }

    if (!hasBody)
    {
      // Creating
      createEntity (Structural::Body);

      // Setting
      root.setAttribute ("uid", _entities.firstKey ());
    }
  }

  for (int i = 0; i < rootChildren.length (); i++)
  {
    if (rootChildren.item (i).isElement ())
    {
      QDomElement element = rootChildren.item (i).toElement ();

      if (element.nodeName () == "entity")
      {
        load (element, root);
      }
      else if (element.nodeName () == "reference")
      {
        _references[element.attributeNode ("uid").nodeValue ()]
            = element.attributeNode ("refer").nodeValue ();
      }
    }
  }

  foreach (const QString &key, _entities.keys ())
  {
    if (_entities.contains (key))
    {
      StructuralEntity *e = _entities.value (key);

      if (e->getCategory () == Structural::Edge
          || e->getType () == Structural::Port || e->isReference ())
      {

        adjustReferences (e);
        e->adjust (true);
      }
    }
  }
}

void
StructuralView::load (QDomElement entity, QDomElement parent)
{
  if (!entity.attributeNode ("uid").nodeValue ().isEmpty ())
  {
    QString entityUid;
    QString parentUid;

    entityUid = entity.attributeNode ("uid").nodeValue ();

    if (!parent.attributeNode ("uid").nodeValue ().isEmpty ())
      parentUid = parent.attributeNode ("uid").nodeValue ();
    else
      parentUid = "";

    QDomNodeList entityChildren = entity.childNodes ();

    QMap<QString, QString> properties;

    for (int i = 0; i < entityChildren.length (); i++)
    {
      if (entityChildren.item (i).isElement ())
      {
        QDomElement child = entityChildren.item (i).toElement ();

        if (child.nodeName () == "property")
          properties.insert (child.attributeNode ("name").nodeValue (),
                             child.attributeNode ("value").nodeValue ());
      }
    }

    QMap<QString, QString> settings
        = StructuralUtil::createSettings (false, false);
    settings[STR_SETTING_ADJUST_REFERENCE] = STR_VALUE_FALSE;

    insert (entityUid, parentUid, properties, settings);

    for (int i = 0; i < entityChildren.length (); i++)
    {
      if (entityChildren.item (i).isElement ())
      {
        QDomElement child = entityChildren.item (i).toElement ();

        if (child.nodeName () == "entity")
          load (child, entity);
      }
    }
  }
}

QString
StructuralView::save ()
{
  QDomDocument *document = new QDomDocument ();

  QDomElement root = document->createElement ("structural");

  foreach (const QString &key, _references.keys ())
  {
    if (_entities.contains (key)
        && _entities.contains (_references.value (key)))
    {

      QDomElement reference = document->createElement ("reference");
      reference.setAttribute ("uid", key);
      reference.setAttribute ("refer", _references.value (key));

      root.appendChild (reference);
    }
  }

  if (STR_DEFAULT_WITH_BODY)
  {
    foreach (StructuralEntity *e, _entities.values ())
      if (e->getType () == Structural::Body)
        createDocument (e, document, root);
  }
  else
  {
    foreach (StructuralEntity *e, _entities.values ())
      if (e->getParent () == nullptr)
        createDocument (e, document, root);
  }

  document->appendChild (root);

  return document->toString (4);
}

void
StructuralView::resizeEvent (QResizeEvent *event)
{
  _minimap->move (event->size ().width () - _minimap->width (),
                  event->size ().height () - _minimap->height ());

  QGraphicsView::resizeEvent (event);
}

void
StructuralView::zoomIn ()
{
  if (_zoom + ZOOM_STEP <= ZOOM_MAX)
  {
    _zoom += ZOOM_STEP;

    resetMatrix ();
    scale ((double)_zoom / 100.0, (double)_zoom / 100.0);

    emit zoomChanged (_zoom);
  }
}

void
StructuralView::zoomOut ()
{
  if (_zoom - ZOOM_STEP >= ZOOM_MIN)
  {
    _zoom -= ZOOM_STEP;

    resetMatrix ();
    scale ((double)_zoom / 100.0, (double)_zoom / 100.0);

    emit zoomChanged (_zoom);
  }
}

void
StructuralView::zoomOriginal ()
{
  _zoom = ZOOM_ORIGINAL;

  resetMatrix ();
  scale ((double)_zoom / 100.0, (double)_zoom / 100.0);

  emit zoomChanged (_zoom);
}

void
StructuralView::performPointer ()
{
  setMode (Structural::Pointing);
}

void
StructuralView::performLink ()
{
  setMode (Structural::Linking);
}

void
StructuralView::createDocument (StructuralEntity *entity,
                                QDomDocument *document, QDomElement parent)
{
  if (entity->getType () != Structural::Reference)
  {
    QDomElement element = document->createElement ("entity");
    element.setAttribute ("uid", entity->getUid ());
    element.setAttribute ("type", entity->getType ());

    foreach (const QString &key, entity->getProperties ().keys ())
    {
      QDomElement property = document->createElement ("property");
      property.setAttribute ("name", key);
      property.setAttribute ("value", entity->getProperty (key));

      element.appendChild (property);
    }

    foreach (StructuralEntity *e, entity->getChildren ())
      createDocument (e, document, element);

    parent.appendChild (element);
  }
}

void
StructuralView::insert (QString uid, QString parent,
                        QMap<QString, QString> properties,
                        QMap<QString, QString> settings)
{
  if (!_entities.contains (uid))
  {
    //
    // Initializing....
    //
    StructuralEntity *e = nullptr;
    StructuralEntity *p = _entities.value (parent, nullptr);

    //
    // Creating...
    //
    StructuralType type = StructuralUtil::stringToType (
        properties[STR_PROPERTY_ENTITY_TYPE]);

    if (p != nullptr || !STR_DEFAULT_WITH_BODY)
    {
      switch (type)
      {
        case Structural::Media:
          // Creating...
          e = new StructuralContent ();

          // Connecting...
          // nothing
          break;

        case Structural::Context:
        case Structural::Switch:
          // Creating...
          e = new StructuralComposition ();
          e->setType (type);

          // Connecting...
          // nothing
          break;

        case Structural::Link:
        {
          // Creating...
          e = new StructuralLink ();
          e->setType (type);

          // Connecting...
          connect ((StructuralLink *)e, &StructuralLink::performedEdit,
                   this, &StructuralView::performLinkDialog);

          break;
        }

        case Structural::Area:
        case Structural::Property:
        case Structural::Port:
        case Structural::SwitchPort:
        {
          // Creating...
          e = new StructuralInterface ();
          e->setType (type);

          // Connecting...
          // nothing
          break;
        }

        case Structural::Reference:
        case Structural::Mapping:
        {
          // Creating...
          e = new StructuralEdge ();
          e->setType (type);

          // Setting...
          // nothing

          // Connecting...
          // Nothing
          break;
        }

        case Structural::Bind:
        {
          // Creating...
          e = new StructuralBind ();
          e->setType (type);

          // Setting...
          // nothing

          // Connecting...
          connect ((StructuralBind *) e, &StructuralBind::performedEdit,
                   this, &StructuralView::performBindDialog);

          break;
        }

        default:
        {
          break;
        }
      }
    }
    else
    {
      switch (type)
      {
        case Structural::Body:
        {
          if (getBody () == nullptr)
          {
            // Creating...
            e = new StructuralComposition ();
            e->setType (type);

            // Setting...
            e->setWidth (STR_DEFAULT_BODY_W);
            e->setHeight (STR_DEFAULT_BODY_H);

            // Connecting...
            // nothing

            // Notifing...
            switchedBody (false);
          }
          break;
        }

        default:
        {
          break;
        }
      }
    }

    if (e != nullptr)
    {

      //
      // Setting...
      //

      // Setting 'uid'...
      e->setUid (uid);

      // Setting 'id'...
      if (!properties.contains (STR_PROPERTY_ENTITY_ID))
        e->setId (getNewId (e));

      // Setting 'parent'...
      if (p != nullptr)
        e->setParent (p);
      else
        _scene->addItem (e);

      // Setting 'top'...
      if (!properties.contains (STR_PROPERTY_ENTITY_TOP))
      {
        if (p != nullptr)
          e->setTop (p->getHeight () / 2 - e->getHeight () / 2);
        else
        {
          auto rect = this->mapToScene (this->viewport ()->rect ().center ());
          e->setTop (rect.y () - e->getHeight () / 2);
        }
      }

      // Setting 'left'...
      if (!properties.contains (STR_PROPERTY_ENTITY_LEFT))
      {
        if (p != nullptr)
          e->setLeft (p->getWidth () / 2 - e->getWidth () / 2);
        else
        {
          auto rect = this->mapToScene (this->viewport ()->rect ().center ());
          e->setLeft (rect.x () - e->getWidth () / 2);
        }
      }


      e->setMenu (_menu);

      // Setting 'others'...
      e->setProperties (properties);

      //
      _entities[uid] = e;

      //
      // Adjusting...
      //

      // Adjust 'compositions'
      if (p != nullptr)
      {
        if (!p->isUncollapsed ())
        {
          if (p->getType () == Structural::Body
              || p->getType () == Structural::Switch
              || p->getType () == Structural::Context)
          {
            ((StructuralComposition *)p)->collapse ();
            ((StructuralComposition *)p)->collapse ();
          }
        }
      }

      // Adjust 'references'
      if (settings[STR_SETTING_ADJUST_REFERENCE] != STR_VALUE_FALSE)
      {
        adjustReferences (e);
      }

      // Adjust 'angles'
      if (e->getType () == Structural::Bind)
        if (!properties.contains (STR_PROPERTY_EDGE_ANGLE))
          ((StructuralBind *)e)->setAngle (getNewAngle ((StructuralBind *)e));

      // Adjust 'others'
      if (!properties.contains (STR_PROPERTY_ENTITY_TOP)
          || !properties.contains (STR_PROPERTY_ENTITY_LEFT))
        e->adjust (false);
      else
        e->adjust (true);

      //
      if (settings[STR_SETTING_ADJUST_REFERENCE] != STR_VALUE_FALSE)
      {
        if (e->getCategory () == Structural::Interface)
        {
          if (p != nullptr)
          {
            if (p->getType () == Structural::Media)
            {
              if (p->isReference ())
                adjustReferences (p);
              else
                foreach (const QString &key, _references.keys ())
                {
                  if (_references.contains (key)
                      && _references.value (key) == p->getUid ()
                      && _entities.contains (key))
                  {
                        adjustReferences (_entities.value (key));
                  }
                }
            }
          }
        }
      }

      //
      // Connecting...
      //
      connect (e, &StructuralEntity::inserted, this, &StructuralView::insert);
      connect (e, &StructuralEntity::removed, this, &StructuralView::remove);
      connect (e, &StructuralEntity::changed, this, &StructuralView::change);
      connect (e, &StructuralEntity::selected, this, &StructuralView::select);
      connect (e, &StructuralEntity::moved, this, &StructuralView::move);

      //
      // Saving...
      //
      if (settings[STR_SETTING_UNDO] == STR_VALUE_TRUE)
      {
        Insert *cmd = new Insert (uid, parent, e->getProperties (), settings);
        cmd->setText (settings[STR_SETTING_CODE]);

        connect (cmd, &Insert::insert, this, &StructuralView::insert);
        connect (cmd, &Insert::remove, this, &StructuralView::remove);
        connect (cmd, &Insert::change, this, &StructuralView::change);
        connect (cmd, &Insert::select, this, &StructuralView::select);

        _commands.push (cmd);
        emit switchedUndo (true);
      }

      //
      // Notifing...
      //

      if (settings[STR_SETTING_NOTIFY] == STR_VALUE_TRUE)
      {
        emit inserted (uid, parent, e->getProperties (), settings);
      }

      if (!STR_DEFAULT_WITH_INTERFACES)
      {
        // Since interfaces are hidden, no update related with that entities
        // are notified. However, the structural view create a new id when this
        // property
        // is empty, then the change must be explicit notified for the hidden
        // entities
        // (interface in this case).
        emit changed (uid, e->getProperties (),
                      e->getProperties (), // this param is not used
                                           // in this case
                      StructuralUtil::createSettings (false, false));
      }
    }
  }
}

qreal
StructuralView::getNewAngle (StructuralBind *bind)
{
  int min = 100000;
  int max = -100000;

  qreal angle = 0;

  if (bind->getTail () != nullptr && bind->getHead () != nullptr)
  {
    foreach (StructuralEntity *e, _entities.values ())
    {
      if (e->getType () == Structural::Bind)
      {
        if (e != bind)
        {
          StructuralBind *b = (StructuralBind *)e;

          if (b->getTail () != nullptr && b->getHead () != nullptr)
          {
            if ((bind->getTail () == b->getTail ()
                 || bind->getTail () == b->getHead ())
                && (bind->getHead () == b->getHead ()
                    || bind->getHead () == b->getTail ()))
            {

              int current = b->getAngle ();

              if (b->getHead ()->getType () != Structural::Link)
                current = -current;

              if (max < current)
                max = current;

              if (min > current)
                min = current;
            }
          }
        }
      }
    }

    if (min != 100000 || max != -100000)
    {
      if (max <= abs (min))
        angle = max + 60;
      else if (max > abs (min))
        angle = min - 60;
    }

    if (bind->getHead ()->getType () != Structural::Link)
      return -angle;
  }

  return angle;
}

void
StructuralView::remove (QString uid, QMap<QString, QString> settings)
{
  CPR_ASSERT (_entities.contains (uid));

  if (_entities.contains (uid))
  {
    //
    // Initializing...
    //
    StructuralEntity *e = _entities.value (uid);
    StructuralEntity *p = e->getParent ();
    StructuralType type = e->getType ();

    //
    // Saving...
    //
    if (settings[STR_SETTING_UNDO] == STR_VALUE_TRUE
        && (!e->isReference ()
            || e->getCategory () != Structural::Interface))
    {
      QString parent = "";

      if (_entities[uid]->getParent () != nullptr)
        parent = _entities[uid]->getParent ()->getUid ();

      Remove *cmd = new Remove (_entities[uid]->getUid (), parent,
                                _entities[uid]->getProperties (), settings);
      cmd->setText (settings[STR_SETTING_CODE]);

      connect (cmd, &Remove::insert, this, &StructuralView::insert);
      connect (cmd, &Remove::remove, this, &StructuralView::remove);
      connect (cmd, &Remove::change, this, &StructuralView::change);
      connect (cmd, &Remove::select, this, &StructuralView::select);

      _commands.push (cmd);
      emit switchedUndo (true);
      return;
    }

    //
    // Removing...
    //

    // Removing 'references'...
    if (e->getCategory () == Structural::Interface)
    {
      foreach (const QString &key, _references.keys (e->getUid ()))
        remove (key, StructuralUtil::createSettings (
                         STR_VALUE_FALSE, STR_VALUE_FALSE,
                         settings.value (STR_SETTING_CODE)));

      // Only remove interface -> interface references. Keeping
      // Media -> Media references to enable undo. All "trash" references
      // are ignore when saving the project.
      _references.remove (e->getUid ());
    }
    else if (e->getType () == Structural::Media)
    {
      foreach (const QString &key, _references.keys (e->getUid ()))
      {
        if (_entities.contains (key))
        {
          _entities.value (key)->setReference (false);
          _entities.value (key)->adjust (true);
        }
      }
    }

    // Removing 'children'...
    if (settings.value (STR_SETTING_UNDO_TRACE) != STR_VALUE_FALSE)
      settings[STR_SETTING_UNDO] = STR_VALUE_TRUE;

    while (!e->getChildren ().isEmpty ())
      remove (e->getChildren ().first ()->getUid (),
              settings);

    // Removing 'edges'...
    if (e->getCategory () != Structural::Edge)
    {
      QVector<StructuralEntity *> relatives;
      relatives += StructuralUtil::getNeighbors (e);
      relatives += StructuralUtil::getUpNeighbors (e);

      foreach (StructuralEntity *entity, relatives)
      {
        if (entity->getCategory () == Structural::Edge)
        {
          StructuralEdge *edge = (StructuralEdge *)entity;

          if (edge->getTail () == e || edge->getHead () == e)
          {
            if (edge->getType () != Structural::Reference)
            {
              remove (edge->getUid (),
                      StructuralUtil::createSettings (
                          STR_VALUE_TRUE, settings.value (STR_SETTING_NOTIFY),
                          settings.value (STR_SETTING_CODE)));
            }
            else
            {
              if (STR_DEFAULT_WITH_INTERFACES)
              {
                // In case of 'Structural::Reference' edge, just change
                // 'Structural::Port' properties.
                //
                // Note:
                // The 'tail' of a 'Structural::Reference' edge
                // is always a 'Structural::Port' entity.
                QMap<QString, QString> previous
                    = edge->getTail ()->getProperties ();
                QMap<QString, QString> properties
                    = edge->getTail ()->getProperties ();

                properties[STR_PROPERTY_REFERENCE_COMPONENT_ID] = "";
                properties[STR_PROPERTY_REFERENCE_COMPONENT_UID] = "";
                properties[STR_PROPERTY_REFERENCE_INTERFACE_ID] = "";
                properties[STR_PROPERTY_REFERENCE_INTERFACE_UID] = "";

                change (edge->getTail ()->getUid (), properties,
                        previous,
                        StructuralUtil::createSettings (
                            STR_VALUE_TRUE, settings.value (STR_SETTING_NOTIFY),
                            settings.value (STR_SETTING_CODE)));
              }
            }
          }
        }
      }
    }

    // Removing 'params'...
    if (type == Structural::Link || type == Structural::Bind)
    {
      foreach (const QString &key, e->getProperties ().keys ())
      {
        if (key.contains (STR_PROPERTY_LINKPARAM_NAME)
            || key.contains (STR_PROPERTY_BINDPARAM_NAME))
        {
          if (settings[STR_SETTING_NOTIFY] == STR_VALUE_TRUE)
          {
            emit removed (
                key.right (key.length () - key.lastIndexOf (':') - 1),
                settings);
          }
        }
      }
    }

    if (!STR_DEFAULT_WITH_BODY && !STR_DEFAULT_WITH_FLOATING_INTERFACES)
    {
      if (e->getType () == Structural::Port)
      {
        StructuralEntity *target = nullptr;

        if (_entities.contains (e->getProperty (
                STR_PROPERTY_REFERENCE_COMPONENT_UID)))
        {
          target = _entities.value (
              e->getProperty (STR_PROPERTY_REFERENCE_COMPONENT_UID));

          if (_entities.contains (e->getProperty (
                  STR_PROPERTY_REFERENCE_INTERFACE_UID)))
          {
            target = _entities.value (e->getProperty (
                STR_PROPERTY_REFERENCE_INTERFACE_UID));
          }
        }

        if (target != nullptr)
          target->setProperty (STR_PROPERTY_ENTITY_AUTOSTART,
                                         STR_VALUE_FALSE);
      }
    }

    // Removing 'others'...
    if (p != nullptr)
      p->removeChild (e);
    else
      _scene->removeItem (e);

    _entities.remove (uid);
    delete e;
    e = nullptr;

    //
    // Notifing...
    //
    if (uid == _selected)
    {
      emit select ("", settings);
    }

    if (type == Structural::Body)
    {
      emit switchedBody (true);
    }

    if (settings[STR_SETTING_NOTIFY] == STR_VALUE_TRUE)
    {
      emit removed (uid, settings);
    }
  }
}

void
StructuralView::change (QString uid, QMap<QString, QString> properties,
                        QMap<QString, QString> previous,
                        QMap<QString, QString> settings)
{
  if (_entities.contains (uid))
  {
    //
    // Saving...
    //
    if (settings[STR_SETTING_UNDO] == STR_VALUE_TRUE)
    {
      Change *cmd = new Change (uid, properties, previous, settings);
      cmd->setText (settings[STR_SETTING_CODE]);

      connect (cmd, &Change::insert, this, &StructuralView::insert);
      connect (cmd, &Change::remove, this, &StructuralView::remove);
      connect (cmd, &Change::change, this, &StructuralView::change);
      connect (cmd, &Change::select, this, &StructuralView::select);

      _commands.push (cmd);
      emit switchedUndo (true);
      return;
    }

    //
    // Initializing...
    //
    StructuralEntity *entity = _entities[uid];

    //
    // Setting...
    //
    if (entity->isUncollapsed ())
      if (properties.value (STR_PROPERTY_ENTITY_UNCOLLAPSED)
          == STR_VALUE_FALSE)
        ((StructuralComposition *)entity)->collapse ();

    if (!entity->isUncollapsed ())
      if (properties.value (STR_PROPERTY_ENTITY_UNCOLLAPSED) == STR_VALUE_TRUE)
        ((StructuralComposition *)entity)->collapse ();

    if (!STR_DEFAULT_WITH_BODY && !STR_DEFAULT_WITH_FLOATING_INTERFACES)
    {
      if (entity->getType () == Structural::Port)
      {
        StructuralEntity *lasttarget = nullptr;

        if (_entities.contains (
                previous.value (STR_PROPERTY_REFERENCE_COMPONENT_UID)))
        {
          lasttarget = _entities.value (
              previous.value (STR_PROPERTY_REFERENCE_COMPONENT_UID));

          if (_entities.contains (
                  previous.value (STR_PROPERTY_REFERENCE_INTERFACE_UID)))
          {
            lasttarget = _entities.value (
                previous.value (STR_PROPERTY_REFERENCE_INTERFACE_UID));
          }
        }

        if (lasttarget != nullptr)
          lasttarget->setProperty (STR_PROPERTY_ENTITY_AUTOSTART,
                                             STR_VALUE_FALSE);
      }
    }

    // Setting 'others'...
    entity->setProperties (properties);

    //
    // Adjusting...
    //

    // Adjust 'references'
    adjustReferences (entity);

    // Adjust 'others'
    entity->adjust (true);

    //
    // Notifing...
    //

    if (settings[STR_SETTING_NOTIFY] == STR_VALUE_TRUE)
    {
      emit changed (uid, properties, previous, settings);
    }
  }
}

void
StructuralView::adjustReferences (StructuralEntity *entity)
{
  CPR_ASSERT (entity != nullptr);

  if (entity != nullptr)
  {
    StructuralType type = entity->getType ();

    switch (type)
    {
      case Structural::Media:
      {

        // Ajusting others media entities that refer
        // the current one
        bool hasChange = false;

        if (!entity->getProperty (STR_PROPERTY_REFERENCE_REFER_ID)
                 .isEmpty ())
        {
          if (_entities.contains (entity->getProperty (
                  STR_PROPERTY_REFERENCE_REFER_UID)))
          {
            StructuralEntity *refer
                = _entities.value (entity->getProperty (
                    STR_PROPERTY_REFERENCE_REFER_UID));

            if (entity != refer && !refer->isReference ())
            {
              if (refer->getType () == Structural::Media)
              {
                QString instance = "new";

                if (!entity
                         ->getProperty (
                             STR_PROPERTY_CONTENT_INSTANCE)
                         .isEmpty ())
                  instance = entity->getProperty (
                      STR_PROPERTY_CONTENT_INSTANCE);

                foreach (StructuralEntity *e, entity->getChildren ())
                  if (e->isReference ())
                    if (_entities.contains (e->getProperty (
                            STR_PROPERTY_REFERENCE_REFER_UID)))
                      if (_entities
                              .value (e->getProperty (
                                  STR_PROPERTY_REFERENCE_REFER_UID))
                              ->getParent ()
                          != refer)
                        remove (e->getUid (),
                                StructuralUtil::createSettings (false, false));

                QVector<QString> entities;

                QMap<QString, QString> mapReferToChild;
                QMap<QString, QString> mapChildToRefer;

                foreach (StructuralEntity *e, entity->getChildren ())
                {
                  if (e->isReference ())
                  {
                    mapChildToRefer[e->getUid ()]
                        = e->getProperty (
                            STR_PROPERTY_REFERENCE_REFER_UID);
                    mapReferToChild[e->getProperty (
                        STR_PROPERTY_REFERENCE_REFER_UID)]
                        = e->getUid ();
                  }

                  entities.append (e->getUid ());
                }

                foreach (StructuralEntity *e, refer->getChildren ())
                {
                  if (e->getCategory () == Structural::Interface)
                  {
                    if (!mapReferToChild.contains (e->getUid ()))
                    {
                      if (!entities.contains (
                              _references.value (e->getUid ())))
                      {
                        QString uid = StructuralUtil::createUid ();

                        QMap<QString, QString> properties
                            = e->getProperties ();
                        properties[STR_PROPERTY_ENTITY_UID] = uid;
                        properties[STR_PROPERTY_ENTITY_REFERENCE]
                            = STR_VALUE_TRUE;

                        properties[STR_PROPERTY_REFERENCE_REFER_ID]
                            = e->getId ();
                        properties[STR_PROPERTY_REFERENCE_REFER_UID]
                            = e->getUid ();

                        properties.remove (STR_PROPERTY_ENTITY_TOP);
                        properties.remove (STR_PROPERTY_ENTITY_LEFT);

                        mapChildToRefer[uid] = e->getUid ();
                        mapChildToRefer[e->getUid ()] = uid;

                        _references[uid] = e->getUid ();

                        QMap<QString, QString> settings
                            = StructuralUtil::createSettings (false, false);
                        settings[STR_SETTING_ADJUST_REFERENCE]
                            = STR_VALUE_FALSE;

                        insert (uid, entity->getUid (), properties,
                                settings);
                      }
                    }
                  }
                }

                if (instance == "new")
                {
                  foreach (StructuralEntity *e,
                           entity->getChildren ())
                    if (!e->isReference ())
                      foreach (const QString &key,
                               _references.keys (e->getUid ()))
                        remove (key,
                                StructuralUtil::createSettings (false, false));
                }
                else if (instance == "instSame" || instance == "gradSame")
                {
                  foreach (StructuralEntity *e,
                           entity->getChildren ())
                    if (!e->isReference ())
                      foreach (const QString &key,
                               _references.keys (e->getUid ()))
                        if (_entities.contains (key))
                          if (_entities.value (key)->getParent ()
                              != refer)
                            remove (key, StructuralUtil::createSettings (
                                             false, false));

                  bool hasNewEntity = false;

                  foreach (StructuralEntity *e,
                           entity->getChildren ())
                  {
                    if (e->getCategory () == Structural::Interface)
                    {
                      if (!mapChildToRefer.contains (e->getUid ()))
                      {
                        if (!_references.values ().contains (
                                e->getUid ()))
                        {
                          QString uid = StructuralUtil::createUid ();

                          QMap<QString, QString> properties
                              = e->getProperties ();
                          properties[STR_PROPERTY_ENTITY_UID] = uid;
                          properties[STR_PROPERTY_ENTITY_REFERENCE]
                              = STR_VALUE_TRUE;

                          properties.remove (STR_PROPERTY_ENTITY_TOP);
                          properties.remove (STR_PROPERTY_ENTITY_LEFT);

                          properties[STR_PROPERTY_REFERENCE_REFER_ID]
                              = e->getId ();
                          properties[STR_PROPERTY_REFERENCE_REFER_UID]
                              = e->getUid ();

                          _references[uid] = e->getUid ();

                          QMap<QString, QString> settings
                              = StructuralUtil::createSettings (false, false);
                          settings[STR_SETTING_ADJUST_REFERENCE]
                              = STR_VALUE_FALSE;

                          insert (uid, refer->getUid (), properties,
                                  settings);

                          hasNewEntity = true;
                        }
                      }
                    }
                  }

                  if (hasNewEntity)
                    foreach (const QString &key,
                             _references.keys (refer->getUid ()))
                      if (_entities.contains (key))
                        if (key != entity->getUid ())
                          adjustReferences (_entities.value (key));
                }

                _references[entity->getUid ()]
                    = refer->getUid ();
                entity->setReference (true);
                hasChange = true;
              }
            }
          }
        }

        if (_references.contains (entity->getUid ()))
        {
          if (!hasChange)
          {
            foreach (StructuralEntity *e, entity->getChildren ())
            {
              if (e->isReference ())
              {
                remove (e->getUid (),
                        StructuralUtil::createSettings (false, false));
              }
              else
              {
                foreach (const QString &key,
                         _references.keys (e->getUid ()))
                  remove (key, StructuralUtil::createSettings (false, false));
              }
            }

            entity->setReference (false);
          }
        }

        // Adjusting values of relatives entities
        QVector<StructuralEntity *> relatives;
        relatives += StructuralUtil::getNeighbors (entity);
        relatives += StructuralUtil::getUpNeighbors (entity);

        foreach (StructuralEntity *relative, relatives)
        {
          // Adjusting relative edges
          if (relative->getCategory () == Structural::Edge)
          {
            StructuralEdge *edge = (StructuralEdge *) relative;

            // Adjusting edge if it already has
            // an tail and head defined.
            if (edge->getTail () == entity ||
                edge->getHead () == entity)
            {
              adjustReferences (edge);
            }

            // Adjusting edge if it is a bind and a component value
            // was defined
            else if (edge->getType () == Structural::Bind)
            {
              StructuralEdge *bind = (StructuralEdge *) edge;

              if (!bind->getProperty (
                    STR_PROPERTY_REFERENCE_COMPONENT_ID).isEmpty () &&
                  bind->getProperty (
                    STR_PROPERTY_REFERENCE_INTERFACE_ID).isEmpty ())
              {
                if (entity->getId () ==
                    bind->getProperty (STR_PROPERTY_REFERENCE_COMPONENT_ID))
                {
                  if (bind->getHead () != nullptr &&
                      bind->getHead ()->getType () == Structural::Link)
                  {
                    bind->setTail (entity);

                    bind->setProperty (
                          STR_PROPERTY_EDGE_TAIL,
                          entity->getUid());

                    bind->setProperty (
                          STR_PROPERTY_REFERENCE_COMPONENT_UID,
                          entity->getUid());
                  }
                  else if (bind->getTail () != nullptr &&
                           bind->getTail ()->getType () == Structural::Link)
                  {
                    bind->setHead (entity);

                    bind->setProperty (
                          STR_PROPERTY_EDGE_HEAD,
                          entity->getUid());

                    bind->setProperty (
                          STR_PROPERTY_REFERENCE_COMPONENT_UID,
                          entity->getUid());
                  }

                  adjustReferences (bind);
                }
              }
            }
          }

          // Adjusting relative ports
          else if (relative->getCategory () == Structural::Interface)
          {
            StructuralInterface *interface = (StructuralInterface *) relative;

            if (!STR_DEFAULT_WITH_INTERFACES)
            {
              if (interface->getType() == Structural::Port)
              {
                if (interface->getProperty (
                      STR_PROPERTY_REFERENCE_COMPONENT_ID) == entity->getId())
                {
                  entity->setProperty (
                        STR_PROPERTY_ENTITY_AUTOSTART, STR_VALUE_TRUE);
                }
              }
            }
          }
        }

        break;
      }

      case Structural::Reference:
      case Structural::Mapping:
      case Structural::Bind:
      {
        if (_entities.contains (
                entity->getProperty (STR_PROPERTY_EDGE_TAIL)))
          ((StructuralEdge *)entity)
              ->setTail (_entities.value (
                  entity->getProperty (STR_PROPERTY_EDGE_TAIL)));

        if (_entities.contains (
                entity->getProperty (STR_PROPERTY_EDGE_HEAD)))
          ((StructuralEdge *)entity)
              ->setHead (_entities.value (
                  entity->getProperty (STR_PROPERTY_EDGE_HEAD)));

        bool isVisible = true;

        StructuralEntity *tail = ((StructuralEdge *)entity)->getTail ();
        StructuralEntity *head = ((StructuralEdge *)entity)->getHead ();

        if (tail != nullptr && head != nullptr)
        {
          if (!STR_DEFAULT_WITH_INTERFACES)
          {
            if (tail->getCategory () == Structural::Interface)
            {
              ((StructuralEdge *)entity)->setTail (tail->getParent ());
            }

            if (head->getCategory () == Structural::Interface)
            {
              ((StructuralEdge *)entity)->setHead (head->getParent ());
            }
          }

          if (entity->getParent () != nullptr)
            if (!entity->getParent ()->isUncollapsed ())
              isVisible = false;

          StructuralEntity *component = nullptr;
          StructuralEntity *interface = nullptr;

          if (!entity
                   ->getProperty (
                       STR_PROPERTY_REFERENCE_INTERFACE_ID)
                   .isEmpty ())
          {
            if (head->getId ()
                    != entity->getProperty (
                           STR_PROPERTY_REFERENCE_INTERFACE_ID)
                && tail->getId ()
                       != entity->getProperty (
                              STR_PROPERTY_REFERENCE_INTERFACE_ID))
              isVisible = false;

            if (_entities.contains (entity->getProperty (
                    STR_PROPERTY_REFERENCE_INTERFACE_UID)))
              interface = _entities.value (entity->getProperty (
                  STR_PROPERTY_REFERENCE_INTERFACE_UID));
            else
              isVisible = false;
          }

          if (!entity
                   ->getProperty (
                       STR_PROPERTY_REFERENCE_COMPONENT_ID)
                   .isEmpty ())
          {
            if (entity
                    ->getProperty (
                        STR_PROPERTY_REFERENCE_INTERFACE_ID)
                    .isEmpty ())
              if (head->getId ()
                      != entity->getProperty (
                             STR_PROPERTY_REFERENCE_COMPONENT_ID)
                  && tail->getId ()
                         != entity->getProperty (
                                STR_PROPERTY_REFERENCE_COMPONENT_ID))
                isVisible = false;

            if (_entities.contains (entity->getProperty (
                    STR_PROPERTY_REFERENCE_COMPONENT_UID)))
              component = _entities.value (entity->getProperty (
                  STR_PROPERTY_REFERENCE_COMPONENT_UID));
            else
              isVisible = false;
          }
          else
          {
            isVisible = false;
          }

          if (component != nullptr)
          {
            if (interface != nullptr)
              if (interface->getParent () != component)
                isVisible = false;

            if (entity->getParent ()
                != component->getParent ())
              isVisible = false;
          }

          //          if (!STR_DEFAULT_WITH_BODY &&
          //              !STR_DEFAULT_WITH_FLOATING_INTERFACES)
          //          {
          //            if (tail->isHidden())
          //              isVisible = false;
          //          }
        }
        else
        {
          isVisible = false;
        }

        entity->setHidden (!isVisible);

        break;
      }

      case Structural::Area:
      case Structural::Property:
      case Structural::Port:
      case Structural::Switch:
      {
        QVector<StructuralEntity *> relatives;
        relatives += StructuralUtil::getNeighbors (entity);
        relatives += StructuralUtil::getUpNeighbors (entity);

        foreach (StructuralEntity *relative, relatives)
        {
          if (relative->getCategory () == Structural::Edge)
          {
            StructuralEdge *edge = (StructuralEdge *) relative;

            if (edge->getTail () == entity ||
                edge->getHead () == entity)
            {
              adjustReferences (edge);
            }
            else if (edge->getType () == Structural::Bind)
            {
              StructuralEdge *bind = (StructuralEdge *) edge;

              if (!bind->getProperty (
                    STR_PROPERTY_REFERENCE_INTERFACE_ID).isEmpty())
              {
                if (entity->getId () ==
                    bind->getProperty (STR_PROPERTY_REFERENCE_INTERFACE_ID))
                {
                  if (bind->getHead () != nullptr &&
                      bind->getHead ()->getType () == Structural::Link)
                  {
                    bind->setTail (entity);

                    bind->setProperty (
                          STR_PROPERTY_EDGE_TAIL,
                          entity->getUid());
                  }
                  else if (bind->getTail () != nullptr &&
                           bind->getTail ()->getType () == Structural::Link)
                  {
                    bind->setHead (entity);

                    bind->setProperty (
                          STR_PROPERTY_EDGE_HEAD,
                          entity->getUid());
                  }

                  bind->setProperty (
                        STR_PROPERTY_REFERENCE_INTERFACE_UID,
                        entity->getUid());

                  bind->setProperty (
                        STR_PROPERTY_REFERENCE_COMPONENT_ID,
                        entity->getParent()->getId());

                  bind->setProperty (
                        STR_PROPERTY_REFERENCE_COMPONENT_UID,
                        entity->getParent()->getUid());

                  adjustReferences (bind);
                }
              }
            }
          }
        }

        if (type == Structural::Port)
        {
          if (STR_DEFAULT_WITH_INTERFACES)
          {
            foreach (StructuralEntity *e, _entities.values ())
              if (e->getType () == Structural::Reference)
                if (e->getProperty (STR_PROPERTY_EDGE_TAIL)
                    == entity->getUid ())
                  remove (e->getUid (),
                          StructuralUtil::createSettings (false, false));

            StructuralEntity *component = nullptr;
            StructuralEntity *interface = nullptr;

            if (_entities.contains (entity->getProperty (
                    STR_PROPERTY_REFERENCE_INTERFACE_UID)))
              interface = _entities.value (entity->getProperty (
                  STR_PROPERTY_REFERENCE_INTERFACE_UID));

            if (_entities.contains (entity->getProperty (
                    STR_PROPERTY_REFERENCE_COMPONENT_UID)))
              component = _entities.value (entity->getProperty (
                  STR_PROPERTY_REFERENCE_COMPONENT_UID));

            StructuralEntity *head = nullptr;

            if (component != nullptr)
            {
              head = component;

              if (interface != nullptr)
                if (interface->getParent () == component)
                  head = interface;

              if (head != nullptr)
              {
                QMap<QString, QString> properties;
                properties[STR_PROPERTY_ENTITY_TYPE]
                    = StructuralUtil::typeToString (
                        Structural::Reference);
                properties[STR_PROPERTY_EDGE_TAIL] = entity->getUid ();
                properties[STR_PROPERTY_EDGE_HEAD] = head->getUid ();

                properties[STR_PROPERTY_REFERENCE_COMPONENT_ID]
                    = entity->getProperty (
                        STR_PROPERTY_REFERENCE_COMPONENT_ID);
                properties[STR_PROPERTY_REFERENCE_COMPONENT_UID]
                    = entity->getProperty (
                        STR_PROPERTY_REFERENCE_COMPONENT_UID);
                properties[STR_PROPERTY_REFERENCE_INTERFACE_ID]
                    = entity->getProperty (
                        STR_PROPERTY_REFERENCE_INTERFACE_ID);
                properties[STR_PROPERTY_REFERENCE_INTERFACE_UID]
                    = entity->getProperty (
                        STR_PROPERTY_REFERENCE_INTERFACE_UID);

                QString parentUid;

                if (entity->getParent () != nullptr)
                  parentUid = entity->getParent ()->getUid ();
                else
                  parentUid = "";

                if (!STR_DEFAULT_WITH_BODY
                    && !STR_DEFAULT_WITH_FLOATING_INTERFACES)
                {
                  if (entity->getParent () == nullptr)
                  {
                    entity->setHidden (true);

                    head->setProperty (STR_PROPERTY_ENTITY_AUTOSTART,
                                                 STR_VALUE_TRUE);

                    properties[STR_PROPERTY_ENTITY_HIDDEN] = STR_VALUE_TRUE;
                  }
                }

                insert (StructuralUtil::createUid (), parentUid, properties,
                        StructuralUtil::createSettings (false, false));
              }
            }
          }
          else if (!STR_DEFAULT_WITH_FLOATING_INTERFACES)
          {
            StructuralEntity* component = nullptr;
            StructuralEntity* interface = nullptr;

            if (_entities.contains(
                  entity->getProperty (STR_PROPERTY_REFERENCE_COMPONENT_UID)))
            {
              component = _entities[
                  entity->getProperty (STR_PROPERTY_REFERENCE_COMPONENT_UID)];
            }

            if (_entities.contains(
                  entity->getProperty (STR_PROPERTY_REFERENCE_INTERFACE_UID)))
            {
              interface = _entities[
                  entity->getProperty (STR_PROPERTY_REFERENCE_INTERFACE_UID)];
            }

            QMap<QString, QString> properties;

            QMap<QString, QString> settings
                = StructuralUtil::createSettings (true, true);

            if (interface != nullptr)
            {
              if (component != nullptr &&
                  interface->getParent () == component)
              {
                properties = interface->getProperties();
                properties[STR_PROPERTY_ENTITY_AUTOSTART] = STR_VALUE_TRUE;

                change (interface->getUid (),
                        properties,
                        entity->getProperties (),
                        settings);
              }
            }
            else if (component != nullptr)
            {
              properties = component->getProperties();
              properties[STR_PROPERTY_ENTITY_AUTOSTART] = STR_VALUE_TRUE;

              change (component->getUid (),
                      properties,
                      entity->getProperties (),
                      settings);
            }
          }
        }

        break;
      }

      default:
      {
        break;
      }
    }
  }
}

void
StructuralView::select (QString uid, QMap<QString, QString> settings)
{
//  CPR_ASSERT (_entities.contains (uid) || uid == "");

  if (_entities.contains (uid))
  {
    if (uid != _selected)
    {
      //
      // Initializing...
      //
      StructuralEntity *entity = _entities.value (_selected);

      if (entity != nullptr)
        entity->setSelected (false);

      //
      // Selecting...
      //
      entity = _entities.value (uid);
      entity->setSelected (true);

      _selected = entity->getUid ();

      //
      // Notifing...
      //
      bool enablePaste = false;

      if (_clipboard != nullptr)
        if (entity->getUid () != _clipboard->getUid ())
          if (StructuralUtil::validateKinship (
                  _clipboard->getType (),
                  entity->getType ()))
            if (!isChild (entity, _clipboard))
              enablePaste = true;

      emit switchedCut (true);
      emit switchedCopy (true);
      emit switchedPaste (enablePaste);

      if (settings[STR_SETTING_NOTIFY] == STR_VALUE_TRUE)
      {
        emit selected (uid, settings);
      }
    }
  }
  else
  {
    //
    // Initializing...
    //
    StructuralEntity *entity = _entities.value (_selected);

    if (entity != nullptr)
      entity->setSelected (false);

    //
    // Unselecting...
    //
    _selected = "";

    //
    // Notifing...
    //
    emit switchedCut (false);
    emit switchedCopy (false);

    bool enablePaste = false;

    if (!STR_DEFAULT_WITH_BODY)
      if (_clipboard != nullptr)
        if (StructuralUtil::validateKinship (_clipboard->getType (),
                                             Structural::Body))
          enablePaste = true;

    emit switchedPaste (enablePaste);

    if (settings[STR_SETTING_NOTIFY] == STR_VALUE_TRUE)
    {
      emit selected (uid, settings);
    }
  }

  _scene->update ();
}

void
StructuralView::move (QString uid, QString parent,
                      QMap<QString, QString> properties,
                      QMap<QString, QString> settings)
{
  CPR_ASSERT (_entities.contains (uid));

  //
  // Initializing...
  //
  StructuralEntity *e = _entities.value (uid);
  StructuralEntity *p = _entities.value (parent);

  StructuralType type = Structural::Body;

  if (p != nullptr)
    type = p->getType ();

  CPR_ASSERT (StructuralUtil::validateKinship (e->getType (), type));
  if (e->getParent () != p)
  {
    //
    // Setting...
    //
    foreach (StructuralEntity *entity, _entities.values ())
      entity->setDraggable (false);

    select ("", settings);

    StructuralEntity *copy = clone (e, nullptr);

    foreach (const QString &name, properties.keys ())
      copy->setProperty (name, properties.value (name));

    copy->adjust ();

    if (properties.contains (STR_PROPERTY_ENTITY_TOP))
      copy->setTop (copy->getTop () - copy->getHeight () / 2);

    if (properties.contains (STR_PROPERTY_ENTITY_LEFT))
      copy->setLeft (copy->getLeft () - copy->getWidth () / 2);

    //
    // Moving...
    //
    paste (copy, p, settings.value (STR_SETTING_CODE), false);
    remove (uid, settings);
  }
}

void
StructuralView::createEntity (StructuralType type)
{
  QMap<QString, QString> properties;
  properties[STR_PROPERTY_ENTITY_TYPE]
      = StructuralUtil::typeToString (type);

  createEntity (type, properties, StructuralUtil::createSettings ());
}

void
StructuralView::createEntity (StructuralType type,
                              QMap<QString, QString> properties,
                              QMap<QString, QString> settings)
{
  QString uid = StructuralUtil::createUid ();
  QString parent = "";

  if (_entities.contains (_selected))
    parent = _selected;

  if (!properties.contains (STR_PROPERTY_ENTITY_TYPE))
    properties[STR_PROPERTY_ENTITY_TYPE] = StructuralUtil::typeToString (type);

  if (!settings.contains (STR_SETTING_UNDO))
    settings[STR_SETTING_UNDO] = STR_VALUE_TRUE;

  if (!settings.contains (STR_SETTING_NOTIFY))
    settings[STR_SETTING_NOTIFY] = STR_VALUE_TRUE;

  if (!settings.contains (STR_SETTING_CODE))
    settings[STR_SETTING_CODE] = StructuralUtil::createUid ();

  insert (uid, parent, properties, settings);
}

void
StructuralView::performAutostart ()
{
  CPR_ASSERT_X (!STR_DEFAULT_WITH_BODY && !STR_DEFAULT_WITH_FLOATING_INTERFACES,
                "performAutostart",
                "Autostart is only available when when body and floating"
                "interfaces are disabled in build");

  CPR_ASSERT (_entities.contains (_selected));

  StructuralEntity *e = _entities.value (_selected);
  QMap<QString, QString> props = e->getProperties ();

  QMap<QString, QString> settings
      = StructuralUtil::createSettings (true, true);

  // Case 1: media already has an autostart property
  if (props.contains (STR_PROPERTY_ENTITY_AUTOSTART) &&
      props[STR_PROPERTY_ENTITY_AUTOSTART] == STR_VALUE_TRUE)
  {
    QVector<StructuralEntity *> neighbors
        = StructuralUtil::getNeighbors (e);

    if (STR_DEFAULT_WITH_INTERFACES)
    {
      if (e->getCategory() == Structural::Interface)
        neighbors += StructuralUtil::getUpNeighbors (e);
    }

    // If STR_DEFAULT_WITH_INTERFACES is disabled, interface entities are
    // just hidden. So, it is OK to iterate through interfaces, since they
    // exist even with STR_DEFAULT_WITH_INTERFACES disabled.
    foreach (StructuralEntity* neighbor, neighbors)
    {
      if (neighbor->getCategory() == Structural::Interface)
      {
        if (neighbor->getProperty(STR_PROPERTY_REFERENCE_COMPONENT_UID)
            == e->getUid() || (STR_DEFAULT_WITH_INTERFACES &&
            neighbor->getProperty(STR_PROPERTY_REFERENCE_INTERFACE_UID)
            == e->getUid()))
        {
          remove (neighbor->getUid(), settings);
          break;
        }
      }
    }

    props[STR_PROPERTY_ENTITY_AUTOSTART] = STR_VALUE_FALSE;
  }
  // Case 2: media does not has autostart property
  else
  {
    QMap<QString, QString> pProperty;
    pProperty[STR_PROPERTY_ENTITY_TYPE]
        = StructuralUtil::typeToString (Structural::Port);

    QString pParent = "";

    if (e->getCategory() == Structural::Interface)
    {
      if (e->getParent () != nullptr)
      {
        pProperty.insert (STR_PROPERTY_REFERENCE_COMPONENT_UID,
                          e->getParent()->getUid());
        pProperty.insert (STR_PROPERTY_REFERENCE_COMPONENT_ID,
                          e->getParent()->getId());

        pProperty.insert (STR_PROPERTY_REFERENCE_INTERFACE_UID,
                          e->getUid());
        pProperty.insert (STR_PROPERTY_REFERENCE_INTERFACE_ID,
                          e->getId());

        pParent = e->getParent ()->getParent () != nullptr ?
            e->getParent ()->getParent ()->getUid () : "";
      }
    }
    else
    {
      pProperty.insert (STR_PROPERTY_REFERENCE_COMPONENT_UID,
                        e->getUid());
      pProperty.insert (STR_PROPERTY_REFERENCE_COMPONENT_ID,
                        e->getId());

      pParent = e->getParent () != nullptr ? e->getParent ()->getUid () : "";
    }

    insert (StructuralUtil::createUid (), pParent, pProperty, settings);

    props[STR_PROPERTY_ENTITY_AUTOSTART] = STR_VALUE_TRUE;
  }

  change (e->getUid (), props,
          e->getProperties (),
          StructuralUtil::createSettings (true, true));
}

void
StructuralView::performUndo ()
{
  if (_commands.canUndo ())
  {
    QString code = _commands.undoText ();

    while (code == _commands.undoText ())
    {
      _commands.undo ();
    }

    emit switchedRedo (true);
  }

  if (!_commands.canUndo ())
    emit switchedUndo (false);

  foreach (const QString &key, _references.keys ())
  {
    if (_entities.contains (key))
      if (_entities.value (key)->getType () == Structural::Media)
        adjustReferences (_entities.value (key));
  }

  foreach (const QString &key, _entities.keys ())
  {
    if (_entities.contains (key))
    {
      StructuralEntity *e = _entities.value (key);

      if (e->getCategory () == Structural::Edge)
      {
        StructuralEntity *c = _entities.value (
            e->getProperty (STR_PROPERTY_REFERENCE_COMPONENT_UID));
        StructuralEntity *i = _entities.value (
            e->getProperty (STR_PROPERTY_REFERENCE_INTERFACE_UID));

        if (c != nullptr)
        {
          if (i == nullptr)
          {
            foreach (StructuralEntity *r, c->getChildren ())
            {
              if (r->getId ()
                  == e->getProperty (
                         STR_PROPERTY_REFERENCE_INTERFACE_ID))
              {
                e->setProperty (STR_PROPERTY_REFERENCE_INTERFACE_UID,
                                          r->getUid ());

                if (((StructuralEdge *)e)->getTail () != nullptr)
                  e->setProperty (STR_PROPERTY_EDGE_HEAD,
                                            r->getUid ());
                else
                  e->setProperty (STR_PROPERTY_EDGE_TAIL,
                                            r->getUid ());

                adjustReferences (e);
                e->adjust (true);

                break;
              }
            }
          }
        }
      }
    }
  }
}

void
StructuralView::performRedo ()
{
  if (_commands.canRedo ())
  {
    QString code = _commands.redoText ();

    while (code == _commands.redoText ())
      _commands.redo ();

    emit switchedUndo (true);
  }

  if (!_commands.canRedo ())
    emit switchedRedo (false);
}

void
StructuralView::performCut ()
{
  performCopy ();

  StructuralEntity *entity = _entities.value (_selected);

  if (entity != nullptr)
  {
    _clipboard->setId (entity->getId ());

    if (_clipboard->getUid () == entity->getUid ())
      remove (_selected, StructuralUtil::createSettings ());
  }
}

void
StructuralView::performCopy ()
{
  if (_entities.contains (_selected))
  {
    if (_clipboard != nullptr)
    {
      delete (_clipboard);

      _clipboardReferences.clear ();
    }

    StructuralEntity *entity = _entities.value (_selected);

    if (entity != nullptr)
      if (entity->getCategory () != Structural::Edge
          && entity->getType () != Structural::Body)
      {
        _clipboard = clone (entity);
        _clipboard->setId ("c_" + _clipboard->getId ());

        emit switchedPaste (false);
      }
  }
}

void
StructuralView::performPaste ()
{
  if (_clipboard != nullptr)
  {
    StructuralEntity *parent = nullptr;

    if (_entities.contains (_selected))
      parent = _entities.value (_selected, nullptr);

    bool isAValidPaste = false;

    if (!STR_DEFAULT_WITH_BODY)
    {
      if (StructuralUtil::validateKinship (_clipboard->getType (),
                                           Structural::Body))
        isAValidPaste = true;
    }
    else
    {
      if (parent != nullptr)
        if (_clipboard->getUid () != parent->getUid ())
          if (StructuralUtil::validateKinship (
                  _clipboard->getType (),
                  parent->getType ())
              && !isChild (parent, _clipboard))
            isAValidPaste = true;
    }

    if (isAValidPaste)
    {
      // 0: Cancel - Nothing todo.
      // 1: No - Paste entity.
      // 2: Yes - Create refer entity.
      int result = 1;

      if (_entities.contains (_clipboard->getUid ()))
        result = QMessageBox::question (
            this, tr ("Paste"),
            tr ("Would you like to make a reference instead?"), tr ("Cancel"),
            tr ("No"), tr ("Yes"), 1);

      if (result == 1)
      {
        QString nextID = _clipboard->getId ();

        int n = 0;

        foreach (StructuralEntity *entity, _entities.values ())
          if (entity->getId ().startsWith (nextID))
            ++n;

        if (n > 0)
          nextID = nextID + "_" + QString::number (n);

        _clipboard->setId (nextID);

        paste (_clipboard, parent);
      }
      else if (result == 2)
      {
        QString uid = StructuralUtil::createUid ();

        QMap<QString, QString> properties
            = _clipboard->getProperties ();
        properties[STR_PROPERTY_ENTITY_UID] = uid;

        QString refID = _clipboard->getId ();
        refID = refID.section ('_', 1, 1);

        int n = 0;

        foreach (StructuralEntity *entity, _entities.values ())
          if (entity->getId ().startsWith ("r_" + refID))
            ++n;

        if (n > 0)
          refID = "r_" + refID + "_" + QString::number (n);
        else
          refID = "r_" + refID;

        properties[STR_PROPERTY_ENTITY_ID] = refID;

        properties[STR_PROPERTY_REFERENCE_REFER_ID]
            = _clipboard->getId ().section ('_', 1, 1);
        properties[STR_PROPERTY_REFERENCE_REFER_UID]
            = _clipboard->getUid ();

        properties.remove (STR_PROPERTY_ENTITY_TOP);
        properties.remove (STR_PROPERTY_ENTITY_LEFT);

        insert (uid, (parent != nullptr ? parent->getUid () : ""),
                properties, StructuralUtil::createSettings ());
      }

      // Fix entity drag status
      foreach (StructuralEntity *e, _entities.values ())
        e->setDraggable (false);
    }
  }
}

void
StructuralView::createLink (StructuralEntity *tail, StructuralEntity *head)
{
  StructuralEntity *parentTail
      = (StructuralEntity *)tail->getParent ();
  StructuralEntity *parentHead
      = (StructuralEntity *)head->getParent ();

  if ((parentTail != nullptr && parentHead != nullptr) || !STR_DEFAULT_WITH_BODY)
  {
    StructuralEntity *parent = nullptr;

    QPointF ptail;
    QPointF phead;

    if (parentTail == parentHead)
    {
      ptail = QPointF (tail->getLeft (), tail->getTop ());
      phead = QPointF (head->getLeft (), head->getTop ());

      parent = parentTail;
    }
    else if (parentTail != nullptr
             && parentTail->getParent () == parentHead
             && tail->getCategory () == Structural::Interface)
    {
      phead = QPointF (head->getLeft (), head->getTop ());
      ptail = parentTail->mapToParent (tail->getLeft (), tail->getTop ());

      parent = parentHead;
    }
    else if (parentHead != nullptr
             && parentTail == parentHead->getParent ()
             && head->getCategory () == Structural::Interface)
    {
      ptail = QPointF (tail->getLeft (), tail->getTop ());
      phead = parentHead->mapToParent (head->getLeft (), head->getTop ());

      parent = parentTail;
    }
    else if (parentTail != nullptr && parentHead != nullptr
             && parentTail->getParent ()
                    == parentHead->getParent ()
             && tail->getCategory () == Structural::Interface
             && head->getCategory () == Structural::Interface)
    {
      ptail = parentTail->mapToParent (tail->getLeft (), tail->getTop ());
      phead = parentHead->mapToParent (head->getLeft (), head->getTop ());

      parent = parentTail->getParent ();
    }

    setMode (Structural::Pointing);
    emit switchedPointer (true);

    if (parent != nullptr || !STR_DEFAULT_WITH_BODY)
    {
      emit requestedUpdate ();

      if (tail->getCategory () == Structural::Interface)
      {
        foreach (StructuralEntity *entity,
                 parentTail->getChildren ())
        {
          _dialog->addConditionInterface (
              entity->getUid (), entity->getId (),
              StructuralUtil::getIcon (entity->getType ()));
        }

        _dialog->setConditionInterface (tail->getId ());
      }
      else
      {
        foreach (StructuralEntity *entity, tail->getChildren ())
        {
          _dialog->addConditionInterface (
              entity->getUid (), entity->getId (),
              StructuralUtil::getIcon (entity->getType ()));
        }
      }

      if (head->getCategory () == Structural::Interface)
      {
        foreach (StructuralEntity *entity,
                 parentHead->getChildren ())
        {
          _dialog->addActionInterface (
              entity->getUid (), entity->getId (),
              StructuralUtil::getIcon (entity->getType ()));
        }

        _dialog->setActionInterface (head->getId ());
      }
      else
      {
        foreach (StructuralEntity *entity, head->getChildren ())
        {
          _dialog->addActionInterface (
              entity->getUid (), entity->getId (),
              StructuralUtil::getIcon (entity->getType ()));
        }
      }

      _dialog->setMode ();

      if (_dialog->exec ())
      {

        if (!_dialog->getConditionInterface ().isEmpty ())
        {
          QString uid = _dialog->getConditionInterface ();

          if (_entities.contains (uid))
          {
            tail = _entities.value (uid);
            parentTail = tail->getParent ();

            ptail
                = parentTail->mapToParent (tail->getLeft (), tail->getTop ());
          }
        }

        if (!_dialog->getActionInterface ().isEmpty ())
        {
          QString uid = _dialog->getActionInterface ();

          if (_entities.contains (uid))
          {
            head = _entities.value (uid);
            parentHead = head->getParent ();

            phead
                = parentHead->mapToParent (head->getLeft (), head->getTop ());
          }
        }

        ptail.setX (ptail.x () + tail->getWidth () / 2);
        ptail.setY (ptail.y () + tail->getHeight () / 2);
        phead.setX (phead.x () + head->getWidth () / 2);
        phead.setY (phead.y () + head->getHeight () / 2);

        QString uid = StructuralUtil::createUid ();

        QMap<QString, QString> properties;
        properties[STR_PROPERTY_ENTITY_TYPE]
            = StructuralUtil::typeToString (Structural::Link);
        properties[STR_PROPERTY_REFERENCE_XCONNECTOR_ID]
            = _dialog->getConnector ();

        qreal x = 0.0, y = 0.0;

        if (ptail.x () <= phead.x () && ptail.y () <= phead.y ())
        {
          if (ptail.x () + tail->getWidth () / 2
              < phead.x () - head->getWidth () / 2)
            x = (ptail.x () + tail->getWidth () / 2 + phead.x ()
                 - head->getWidth () / 2)
                / 2;
          else
            x = ptail.x () + (phead.x () - ptail.x ()) / 2;

          if (ptail.y () + tail->getHeight () / 2
              < phead.y () - head->getHeight () / 2)
            y = (ptail.y () + tail->getHeight () / 2 + phead.y ()
                 - head->getHeight () / 2)
                / 2;
          else
            y = ptail.y () + (phead.y () - ptail.y ()) / 2;
        }
        else if (ptail.x () > phead.x () && ptail.y () <= phead.y ())
        {
          if (phead.x () + head->getWidth () / 2
              < ptail.x () - tail->getWidth () / 2)
            x = (phead.x () + head->getWidth () / 2 + ptail.x ()
                 - tail->getWidth () / 2)
                / 2;
          else
            x = phead.x () + (ptail.x () - phead.x ()) / 2;

          if (ptail.y () + tail->getHeight () / 2
              < phead.y () - head->getHeight () / 2)
            y = (ptail.y () + tail->getHeight () / 2 + phead.y ()
                 - head->getHeight () / 2)
                / 2;
          else
            y = ptail.y () + (phead.y () - ptail.y ()) / 2;
        }
        else if (ptail.x () <= phead.x () && ptail.y () > phead.y ())
        {
          if (ptail.x () + tail->getWidth () / 2
              < phead.x () - head->getWidth () / 2)
            x = (ptail.x () + tail->getWidth () / 2 + phead.x ()
                 - head->getWidth () / 2)
                / 2;
          else
            x = ptail.x () + (phead.x () - ptail.x ()) / 2;

          if (phead.y () + head->getHeight () / 2
              < ptail.y () - tail->getHeight () / 2)
            y = (phead.y () + head->getHeight () / 2 + ptail.y ()
                 - tail->getHeight () / 2)
                / 2;
          else
            y = phead.y () + (ptail.y () - phead.y ()) / 2;
        }
        else if (ptail.x () > phead.x () && ptail.y () > phead.y ())
        {
          if (phead.x () + head->getWidth () / 2
              < ptail.x () - tail->getWidth () / 2)
            x = (phead.x () + head->getWidth () / 2 + ptail.x ()
                 - tail->getWidth () / 2)
                / 2;
          else
            x = phead.x () + (ptail.x () - phead.x ()) / 2;

          if (phead.y () + head->getHeight () / 2
              < ptail.y () - tail->getHeight () / 2)
            y = (phead.y () + head->getHeight () / 2 + ptail.y ()
                 - tail->getHeight () / 2)
                / 2;
          else
            y = phead.y () + (ptail.y () - phead.y ()) / 2;
        }

        properties[STR_PROPERTY_ENTITY_TOP] = QString::number (y);
        properties[STR_PROPERTY_ENTITY_LEFT] = QString::number (x);

        QMap<QString, QString> params = _dialog->getConnectorParams ();

        foreach (const QString &key, params.keys ())
        {
          if (!params.value (key).isEmpty ())
          {
            QString uid = StructuralUtil::createUid ();

            properties[QString (STR_PROPERTY_LINKPARAM_NAME) + ":" + uid]
                = key;
            properties[QString (STR_PROPERTY_LINKPARAM_VALUE) + ":" + uid]
                = params.value (key);
          }
        }

        QMap<QString, QString> settings
            = StructuralUtil::createSettings (true, true);

        insert (uid, (parent != nullptr ? parent->getUid () : ""),
                properties, settings);

        if (_entities.contains (uid))
        {
          createBind (tail, _entities.value (uid), _dialog->getCondition (),
                      settings.value (STR_SETTING_CODE));
          createBind (_entities.value (uid), head, _dialog->getAction (),
                      settings.value (STR_SETTING_CODE));
        }
      }
    }
  }
}

void
StructuralView::createBind (StructuralEntity *tail, StructuralEntity *head,
                            const QString &role, const QString &code)
{
  StructuralEntity *parentTail = tail->getParent ();
  StructuralEntity *parentHead = head->getParent ();

  if ((parentTail != nullptr && parentHead != nullptr) || !STR_DEFAULT_WITH_BODY)
  {
    StructuralEntity *parent = nullptr;

    if (parentTail == parentHead)
      parent = parentTail;

    else if (parentTail != nullptr
             && parentTail->getParent () == parentHead
             && tail->getCategory () == Structural::Interface)
      parent = parentHead;

    else if (parentHead != nullptr
             && parentTail == parentHead->getParent ()
             && head->getCategory () == Structural::Interface)
      parent = parentTail;

    if (parent != nullptr || !STR_DEFAULT_WITH_BODY)
    {
      QString uid = StructuralUtil::createUid ();

      QMap<QString, QString> properties;
      properties[STR_PROPERTY_ENTITY_TYPE]
          = StructuralUtil::typeToString (Structural::Bind);
      properties[STR_PROPERTY_EDGE_TAIL] = tail->getUid ();
      properties[STR_PROPERTY_EDGE_HEAD] = head->getUid ();

      properties[STR_PROPERTY_BIND_ROLE] = role;
      properties[STR_PROPERTY_ENTITY_ID] = role;

      StructuralEntity *entityLink = nullptr;
      StructuralEntity *entityNonLink = nullptr;
      StructuralEntity *parentNonLink = nullptr;

      if (tail->getType () == Structural::Link)
      {
        entityLink = tail;
        entityNonLink = head;
        parentNonLink = parentHead;

        if (role.isEmpty ())
        {
          emit requestedUpdate ();

          setMode (Structural::Pointing);
          emit switchedPointer (true);

          if (entityNonLink->getCategory () == Structural::Interface)
          {
            foreach (StructuralEntity *entity,
                     parentNonLink->getChildren ())
            {
              _dialog->addActionInterface (
                  entity->getUid (), entity->getId (),
                  StructuralUtil::getIcon (entity->getType ()));
            }

            _dialog->setActionInterface (entityNonLink->getId ());
          }
          else
          {
            foreach (StructuralEntity *entity,
                     entityNonLink->getChildren ())
            {
              _dialog->addActionInterface (
                  entity->getUid (), entity->getId (),
                  StructuralUtil::getIcon (entity->getType ()));
            }
          }

          _dialog->setMode (entityLink->getProperty (
                                STR_PROPERTY_REFERENCE_XCONNECTOR_ID),
                            "", "", StructuralLinkDialog::CreateAction);

          if (_dialog->exec ())
          {
            if (!_dialog->getActionInterface ().isEmpty ())
            {
              QString uid = _dialog->getActionInterface ();

              if (_entities.contains (uid))
              {
                entityNonLink = _entities.value (uid);
                parentNonLink = entityNonLink->getParent ();

                properties[STR_PROPERTY_EDGE_HEAD]
                    = entityNonLink->getUid ();
              }
            }

            properties[STR_PROPERTY_BIND_ROLE] = _dialog->getAction ();
            ;
            properties[STR_PROPERTY_ENTITY_ID] = _dialog->getAction ();
            ;
          }
          else
          {
            return;
          }
        }

        QMap<QString, QString> params = _dialog->getActionParams ();

        foreach (const QString &key, params.keys ())
        {
          if (!params.value (key).isEmpty ())
          {
            QString uid = StructuralUtil::createUid ();

            properties.insert (
                QString (STR_PROPERTY_BINDPARAM_NAME) + ":" + uid, key);
            properties.insert (QString (STR_PROPERTY_BINDPARAM_VALUE) + ":"
                                   + uid,
                               params.value (key));
          }
        }
      }
      else if (head->getType () == Structural::Link)
      {
        entityLink = head;
        entityNonLink = tail;
        parentNonLink = parentTail;

        if (role.isEmpty ())
        {
          emit requestedUpdate ();

          setMode (Structural::Pointing);
          emit switchedPointer (true);

          if (entityNonLink->getCategory () == Structural::Interface)
          {
            foreach (StructuralEntity *entity,
                     parentNonLink->getChildren ())
            {
              _dialog->addConditionInterface (
                  entity->getUid (), entity->getId (),
                  StructuralUtil::getIcon (entity->getType ()));
            }

            _dialog->setConditionInterface (entityNonLink->getId ());
          }
          else
          {
            foreach (StructuralEntity *entity,
                     entityNonLink->getChildren ())
            {
              _dialog->addConditionInterface (
                  entity->getUid (), entity->getId (),
                  StructuralUtil::getIcon (entity->getType ()));
            }
          }

          _dialog->setMode (entityLink->getProperty (
                                STR_PROPERTY_REFERENCE_XCONNECTOR_ID),
                            "", "", StructuralLinkDialog::CreateCondition);

          if (_dialog->exec ())
          {
            if (!_dialog->getConditionInterface ().isEmpty ())
            {
              QString uid = _dialog->getConditionInterface ();

              if (_entities.contains (uid))
              {
                entityNonLink = _entities.value (uid);
                parentNonLink = entityNonLink->getParent ();

                properties[STR_PROPERTY_EDGE_TAIL]
                    = entityNonLink->getUid ();
              }
            }

            properties[STR_PROPERTY_BIND_ROLE] = _dialog->getCondition ();
            properties[STR_PROPERTY_ENTITY_ID] = _dialog->getCondition ();
          }
          else
          {
            return;
          }
        }

        QMap<QString, QString> params = _dialog->getConditionParams ();

        foreach (const QString &key, params.keys ())
        {
          if (!params.value (key).isEmpty ())
          {
            QString uid = StructuralUtil::createUid ();

            properties.insert (
                QString (STR_PROPERTY_BINDPARAM_NAME) + ":" + uid, key);
            properties.insert (QString (STR_PROPERTY_BINDPARAM_VALUE) + ":"
                                   + uid,
                               params.value (key));
          }
        }
      }

      if (entityLink != nullptr && entityNonLink != nullptr)
      {
        properties[STR_PROPERTY_REFERENCE_LINK_UID]
            = entityLink->getUid ();

        if (entityNonLink->getCategory () == Structural::Interface)
        {
          properties[STR_PROPERTY_REFERENCE_INTERFACE_ID]
              = entityNonLink->getId ();
          properties[STR_PROPERTY_REFERENCE_INTERFACE_UID]
              = entityNonLink->getUid ();
          properties[STR_PROPERTY_REFERENCE_COMPONENT_ID]
              = parentNonLink->getId ();
          properties[STR_PROPERTY_REFERENCE_COMPONENT_UID]
              = parentNonLink->getUid ();
        }
        else
        {
          properties[STR_PROPERTY_REFERENCE_COMPONENT_ID]
              = entityNonLink->getId ();
          properties[STR_PROPERTY_REFERENCE_COMPONENT_UID]
              = entityNonLink->getUid ();
          properties[STR_PROPERTY_REFERENCE_INTERFACE_ID] = "";
          properties[STR_PROPERTY_REFERENCE_INTERFACE_UID] = "";
        }
      }

      QMap<QString, QString> settings
          = StructuralUtil::createSettings (true, true);

      if (!code.isEmpty ())
        settings[STR_SETTING_CODE] = code;

      insert (uid, (parent != nullptr ? parent->getUid () : ""),
              properties, settings);
    }
  }
}

void
StructuralView::createReference (StructuralEntity *tail,
                                 StructuralEntity *head)
{
  if (tail->getType () == Structural::Port
      || tail->getType () == Structural::SwitchPort)
  {

    StructuralEntity *parentTail = tail->getParent ();
    StructuralEntity *parentHead = head->getParent ();

    if (parentTail == parentHead
        || (parentHead != nullptr
            && parentTail == parentHead->getParent ()))
    {
      QMap<QString, QString> previous = tail->getProperties ();
      QMap<QString, QString> properties;

      if (tail->getType () == Structural::Port)
        properties = tail->getProperties ();

      if (head->getCategory () == Structural::Interface)
      {
        properties[STR_PROPERTY_REFERENCE_INTERFACE_ID]
            = head->getId ();
        properties[STR_PROPERTY_REFERENCE_INTERFACE_UID]
            = head->getUid ();
        properties[STR_PROPERTY_REFERENCE_COMPONENT_ID]
            = parentHead->getId ();
        properties[STR_PROPERTY_REFERENCE_COMPONENT_UID]
            = parentHead->getUid ();
      }
      else
      {
        properties[STR_PROPERTY_REFERENCE_COMPONENT_ID]
            = head->getId ();
        properties[STR_PROPERTY_REFERENCE_COMPONENT_UID]
            = head->getUid ();
        properties[STR_PROPERTY_REFERENCE_INTERFACE_ID] = "";
        properties[STR_PROPERTY_REFERENCE_INTERFACE_UID] = "";
      }

      setMode (Structural::Pointing);
      emit switchedPointer (true);

      if (tail->getType () == Structural::Port)
      {
        change (tail->getUid (), properties, previous,
                StructuralUtil::createSettings (true, true));
      }
      else if (tail->getType () == Structural::SwitchPort)
      {
        properties[STR_PROPERTY_ENTITY_TYPE]
            = StructuralUtil::typeToString (Structural::Mapping);
        properties[STR_PROPERTY_EDGE_TAIL] = tail->getUid ();
        properties[STR_PROPERTY_EDGE_HEAD] = head->getUid ();

        insert (StructuralUtil::createUid (), parentTail->getUid (),
                properties, StructuralUtil::createSettings (true, true));
      }
    }
  }
}

QString
StructuralView::getNewId (StructuralEntity *entity)
{
  QString name = "";

  if (entity != nullptr)
  {
    StructuralType type = entity->getType ();

    if (_counter.contains (type))
      _counter[type] = 0;

    int n;
    QString prefix;

    n = _counter.value (type);
    prefix = StructuralUtil::getPrefix (type);

    if (!prefix.isEmpty ())
    {
      bool contains;

      do
      {
        contains = false;

        foreach (StructuralEntity *e, _entities.values ())
        {
          if (e->getId () == QString (prefix + QString::number (n)))
          {
            contains = true;
            ++n;
            break;
          }
        }
      } while (contains);

      name = QString (prefix + QString::number (n));
    }
  }

  return name;
}

void
StructuralView::performDelete ()
{
  if (_entities.contains (_selected))
    remove (_selected, StructuralUtil::createSettings ());
}

void
StructuralView::snapshot ()
{
  qreal top = _scene->height ();
  qreal left = _scene->width ();
  qreal bottom = 0.0;
  qreal right = 0.0;

  foreach (StructuralEntity *entity, _entities.values ())
  {
    if (entity->getParent () == nullptr)
    {
      if (entity->getTop () < top)
        top = entity->getTop ();

      if (entity->getLeft () < left)
        left = entity->getLeft ();

      if (entity->getLeft () + entity->getWidth () > right)
        right = entity->getLeft () + entity->getWidth ();

      if (entity->getTop () + entity->getHeight () > bottom)
        bottom = entity->getTop () + entity->getHeight ();
    }
  }

  StructuralEntity *selected = nullptr;

  if (_entities.contains (_selected))
    selected = _entities.value (_selected);

  if (selected != nullptr)
    selected->setSelected (false);

  QImage image = QImage (right - left + 50, bottom - top + 50,
                         QImage::Format_ARGB32_Premultiplied);
  image.fill (Qt::white);

  QPainter painter (&image);
  _scene->render (
      &painter, QRect (),
      QRect (left - 25, top - 25, right - left + 50, bottom - top + 50));
  painter.end ();

  QMessageBox::StandardButton reply = QMessageBox::question (
      this, "Copy to clipboard?", "Would you like to copy to clipboard?",
      QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes);

  if (reply == QMessageBox::Yes)
  {
    QApplication::clipboard ()->setImage (image, QClipboard::Clipboard);
  }
  else if (reply == QMessageBox::No)
  {
    QString location = QFileDialog::getSaveFileName (
        nullptr, tr ("Save snapshot..."), QDir::homePath (),
        tr ("Images (*.png)"));

    if (!location.isEmpty ())
    {
      if (!location.endsWith (".png"))
        location += ".png";

      image.save (location, "png");

      if (selected != nullptr)
        selected->setSelected (true);
    }
  }
}

void
StructuralView::toggleMinimapVisibility ()
{
  if (!_minimap->isVisible ())
    _minimap->show ();
  else
    _minimap->hide ();
}

void
StructuralView::performBody ()
{
  createEntity (Structural::Body);
}

void
StructuralView::performContext ()
{
  createEntity (Structural::Context);
}

void
StructuralView::performSwitch ()
{
  createEntity (Structural::Switch);
}

void
StructuralView::performMedia ()
{
  createEntity (Structural::Media);
}

void
StructuralView::performPort ()
{
  createEntity (Structural::Port);
}

void
StructuralView::performArea ()
{
  createEntity (Structural::Area);
}

void
StructuralView::performProperty ()
{
  createEntity (Structural::Property);
}

void
StructuralView::performSwitchPort ()
{
  createEntity (Structural::SwitchPort);
}

#ifdef WITH_GRAPHVIZ
void
StructuralView::performAutoAdjust ()
{
  adjustLayout ();
}
#endif

void
StructuralView::performProperties ()
{
  // TODO
}

void
StructuralView::performInsert (StructuralType type,
                               QMap<QString, QString> properties)
{
  createEntity (type, properties, StructuralUtil::createSettings ());
}

void
StructuralView::mouseMoveEvent (QMouseEvent *event)
{
  if (_linking)
  {
    if (_linkingTail != nullptr)
      _linkingTail->setHovering (false);

    if (_linkingHead != nullptr)
      _linkingHead->setHovering (false);

    QList<QGraphicsItem *> tailItems = _scene->items (_tool->getLine ().p1 ());

    if (tailItems.count () && tailItems.first () == _tool)
    {
      tailItems.removeFirst ();
    }

    QList<QGraphicsItem *> headItems = _scene->items (_tool->getLine ().p2 ());

    if (headItems.count () && headItems.first () == _tool)
    {
      headItems.removeFirst ();
    }

    if (tailItems.count ())
    {
      _linkingTail = (StructuralEntity *)tailItems.first ();

      if (_linkingTail != nullptr)
        _linkingTail->setHovering (true);
    }

    if (headItems.count ())
    {
      _linkingHead = (StructuralEntity *)headItems.first ();

      if (_linkingHead != nullptr)
        _linkingHead->setHovering (true);
    }

    _tool->setLine (
        QLineF (_tool->getLine ().p1 (), mapToScene (event->pos ())));
    _scene->update ();
  }

  QGraphicsView::mouseMoveEvent (event);
}

void
StructuralView::mousePressEvent (QMouseEvent *event)
{
  if (_mode == Structural::Linking)
  {
    if (_tool != nullptr)
    {
      delete (_tool);
      _tool = nullptr;
    }

    _tool = new StructuralLinkTool ();
    _scene->addItem (_tool);
    _tool->setLine (
        QLineF (mapToScene (event->pos ()), mapToScene (event->pos ())));
    _tool->adjust (true);

    _linking = true;
  }
  else
  {
    QGraphicsView::mousePressEvent (event);

    if (!event->isAccepted ())
    {
      event->accept ();

      select ("", StructuralUtil::createSettings ());
    }
  }
}

void
StructuralView::mouseReleaseEvent (QMouseEvent *event)
{
  if (_linking)
  {
    if (_linkingTail != nullptr)
      _linkingTail->setHovering (false);

    if (_linkingHead != nullptr)
      _linkingHead->setHovering (false);

    QList<QGraphicsItem *> tailItems = _scene->items (_tool->getLine ().p1 ());

    if (tailItems.count () && tailItems.first () == _tool)
      tailItems.removeFirst ();

    QList<QGraphicsItem *> headtems = _scene->items (_tool->getLine ().p2 ());

    if (headtems.count () && headtems.first () == _tool)
      headtems.removeFirst ();

    if (tailItems.count () && headtems.count ())
    {
      StructuralEntity *tail = (StructuralEntity *)tailItems.first ();
      StructuralEntity *head = (StructuralEntity *)headtems.first ();

      if (tail != head)
      {
        // if linking NODE to NODE
        if (tail->getCategory () == Structural::Node
            && head->getCategory () == Structural::Node)
        {

          if (tail->getType () != Structural::Link
              && head->getType () != Structural::Link)
            createLink (tail, head);
          else if (tail->getType () == Structural::Link)
            createBind (tail, head);
          else if (head->getType () == Structural::Link)
            createBind (tail, head);

          // if linking NODE to INTERFACE
        }
        else if (tail->getCategory () == Structural::Node
                 && head->getCategory () == Structural::Interface)
        {
          if (tail->getType () != Structural::Link)
            createLink (tail, head);
          else
            createBind (tail, head);

          // if linking INTERFACE to NODE
        }
        else if (tail->getCategory () == Structural::Interface
                 && head->getCategory () == Structural::Node)
        {
          StructuralEntity *parentTail = tail->getParent ();
          StructuralEntity *parentHead = head->getParent ();

          if (parentTail == parentHead
              && (tail->getType () == Structural::Port
                  || tail->getType () == Structural::SwitchPort))
            createReference (tail, head);
          else if (head->getType () != Structural::Link)
            createLink (tail, head);
          else
            createBind (tail, head);

          // if linking INTERFACE to INTERFACE
        }
        else if (tail->getCategory () == Structural::Interface
                 && head->getCategory () == Structural::Interface)
        {

          StructuralEntity *parentTail = tail->getParent ();
          StructuralEntity *parentHead = head->getParent ();

          if (parentHead != nullptr
              && parentTail == parentHead->getParent ())
            createReference (tail, head);
          else
            createLink (tail, head);
        }
      }
    }

    if (_tool != nullptr)
    {
      _scene->removeItem (_tool);
      _scene->update ();

      delete _tool;
      _tool = nullptr;
    }

    _linking = false;
  }

  QGraphicsView::mouseReleaseEvent (event);
}

void
StructuralView::keyPressEvent (QKeyEvent *event)
{
  if (event->key () == Qt::Key_Delete || event->key () == Qt::Key_Backspace)
  {
    performDelete ();
    event->accept ();
  }
  else if (event->key () == Qt::Key_Shift)
  {
    setMode (Structural::Linking);
    emit switchedLink (true);

    event->accept ();
  }
  else if (event->key () == Qt::Key_Control)
  {
    foreach (StructuralEntity *e, _entities.values ())
      e->setDraggable (true);

    event->accept ();
  }

  if (!event->isAccepted ())
    QGraphicsView::keyPressEvent (event);
}

void
StructuralView::keyReleaseEvent (QKeyEvent *event)
{
  if (event->key () == Qt::Key_Shift)
  {
    setMode (Structural::Pointing);
    emit switchedPointer (true);

    event->accept ();
  }
  else if (event->key () == Qt::Key_Control)
  {
    foreach (StructuralEntity *e, _entities.values ())
      e->setDraggable (false);

    event->accept ();
  }

  if (!event->isAccepted ())
    QGraphicsView::keyReleaseEvent (event);
}

void
StructuralView::wheelEvent (QWheelEvent *event)
{
  if (event->modifiers () & Qt::ControlModifier)
  {
    if (event->delta () > 0)
      zoomIn ();
    else
      zoomOut ();

    event->accept ();
  }
  else
  {
    QGraphicsView::wheelEvent (event);
  }
}

void
StructuralView::dragEnterEvent (QDragEnterEvent *event)
{
  QGraphicsView::dragEnterEvent (event);

  if (!STR_DEFAULT_WITH_BODY)
  {
    if (!event->isAccepted ())
    {
      QList<QUrl> list = event->mimeData ()->urls ();
      StructuralType type = StructuralUtil::stringToType (
          event->mimeData ()->objectName ());

      if (!list.isEmpty ()
          || StructuralUtil::validateKinship (type, Structural::Body))
        event->setAccepted (true);
      else
        event->setAccepted (false);
    }
  }
}

void
StructuralView::dragMoveEvent (QDragMoveEvent *event)
{
  QGraphicsView::dragMoveEvent (event);

  if (!STR_DEFAULT_WITH_BODY)
  {
    if (!event->isAccepted ())
    {
      QList<QUrl> list = event->mimeData ()->urls ();
      StructuralType type = StructuralUtil::stringToType (
          event->mimeData ()->objectName ());

      if (!list.isEmpty ()
          || StructuralUtil::validateKinship (type, Structural::Body))
        event->setAccepted (true);
      else
        event->setAccepted (false);
    }
  }
}

void
StructuralView::dropEvent (QDropEvent *event)
{
  QGraphicsView::dropEvent (event);

  if (!STR_DEFAULT_WITH_BODY)
  {
    if (event->isAccepted ())
    {
      QList<QUrl> list = event->mimeData ()->urls ();
      StructuralType type = StructuralUtil::stringToType (
          event->mimeData ()->objectName ());

      if (!list.isEmpty ())
      {
        foreach (QUrl url, list)
        {
          QString filename = url.toLocalFile ();

          QMap<QString, QString> properties;
          properties[STR_PROPERTY_ENTITY_TYPE]
              = StructuralUtil::typeToString (Structural::Media);
          properties[STR_PROPERTY_ENTITY_ID]
              = StructuralUtil::formatId (QFileInfo (filename).baseName ());
          properties[STR_PROPERTY_CONTENT_LOCATION] = filename;

          QPointF p = mapToScene (event->pos ());

          properties[STR_PROPERTY_ENTITY_TOP]
              = QString::number (p.y () - STR_DEFAULT_CONTENT_H / 2);
          properties[STR_PROPERTY_ENTITY_LEFT]
              = QString::number (p.x () - STR_DEFAULT_CONTENT_W / 2);

          insert (StructuralUtil::createUid (), "", properties,
                  StructuralUtil::createSettings ());
        }
      }
      else if (StructuralUtil::validateKinship (type, Structural::Body))
      {
        QPointF p = mapToScene (event->pos ());

        QMap<QString, QString> properties;
        properties[STR_PROPERTY_ENTITY_TOP] = QString::number (p.y ());
        properties[STR_PROPERTY_ENTITY_LEFT] = QString::number (p.x ());

        move (event->mimeData ()->text (), "", properties,
              StructuralUtil::createSettings ());
      }
    }
  }
}

void
StructuralView::clean ()
{
  select ("", StructuralUtil::createSettings ());

  foreach (const QString &uid, _entities.keys ())
  {
    QMap<QString, QString> settings
        = StructuralUtil::createSettings (true, false);
    settings[STR_SETTING_UNDO_TRACE] = "0";

    remove (uid, settings);
  }

  _references.clear ();
  _entities.clear ();
  _commands.clear ();

  emit switchedRedo (false);
  emit switchedUndo (false);
}

void
StructuralView::performLinkDialog (StructuralLink *entity)
{
  emit requestedUpdate ();

  _dialog->setMode (
      entity->getProperty (STR_PROPERTY_REFERENCE_XCONNECTOR_ID), "",
      "", StructuralLinkDialog::EditLink);

  QMap<QString, QString> pLink;
  foreach (const QString &name, entity->getProperties ().keys ())
  {
    if (name.contains (STR_PROPERTY_LINKPARAM_NAME))
    {
      QString lpUid = name.right (name.length () - name.lastIndexOf (':') - 1);
      QString lpName = entity->getProperties ().value (name);
      QString lpValue = entity->getProperties ().value (
          QString (STR_PROPERTY_LINKPARAM_VALUE) + ":" + lpUid);

      pLink.insert (lpName, lpValue);
    }
  }

  _dialog->setConnectorParams (pLink);

  if (_dialog->exec ())
  {
    QMap<QString, QString> prev = entity->getProperties ();
    QMap<QString, QString> properties = entity->getProperties ();

    properties.insert (STR_PROPERTY_REFERENCE_XCONNECTOR_ID,
                       _dialog->getConnector ());

    QMap<QString, QString> p = _dialog->getConnectorParams ();

    foreach (const QString &name, p.keys ())
    {
      if (properties.key (name).contains (STR_PROPERTY_LINKPARAM_NAME))
      {
        QString key = properties.key (name);
        QString uid = key.right (key.length () - key.lastIndexOf (':') - 1);

        if (!p.value (name).isEmpty ())
        {
          properties.insert (QString (STR_PROPERTY_LINKPARAM_NAME) + ":" + uid,
                             name);
          properties.insert (QString (STR_PROPERTY_LINKPARAM_VALUE) + ":"
                                 + uid,
                             p.value (name));
        }
        else
        {
          properties.remove (QString (STR_PROPERTY_LINKPARAM_NAME) + ":"
                             + uid);
          properties.remove (QString (STR_PROPERTY_LINKPARAM_VALUE) + ":"
                             + uid);
        }
      }
      else if (!p.value (name).isEmpty ())
      {
        QString uid = StructuralUtil::createUid ();

        properties.insert (QString (STR_PROPERTY_LINKPARAM_NAME) + ":" + uid,
                           name);
        properties.insert (QString (STR_PROPERTY_LINKPARAM_VALUE) + ":" + uid,
                           p.value (name));
      }
    }

    QMap<QString, QString> settings
        = StructuralUtil::createSettings (true, true);

    change (entity->getUid (), properties, prev, settings);
  }
}

void
StructuralView::performBindDialog (StructuralBind *entity)
{
  QMap<QString, QString> pBind;
  foreach (const QString &name, entity->getProperties ().keys ())
  {
    if (name.contains (STR_PROPERTY_BINDPARAM_NAME))
    {
      QString bpUid = name.right (name.length () - name.lastIndexOf (':') - 1);
      QString bpName = entity->getProperties ().value (name);
      QString bpValue = entity->getProperties ().value (
          QString (STR_PROPERTY_BINDPARAM_VALUE) + ":" + bpUid);

      pBind.insert (bpName, bpValue);
    }
  }

  emit requestedUpdate ();

  if (StructuralUtil::isCondition (entity->getRole ()))
  {
    _dialog->setMode (entity->getHead ()->getProperty (
                          STR_PROPERTY_REFERENCE_XCONNECTOR_ID),
                      "", "", StructuralLinkDialog::EditCondition);

    _dialog->setCondition (entity->getId ());
    _dialog->setConditionParams (pBind);

    if (entity->getTail ()->getCategory () == Structural::Interface)
    {
      foreach (
          StructuralEntity *e,
          entity->getTail ()->getParent ()->getChildren ())
      {
        _dialog->addConditionInterface (
            e->getUid (), e->getId (),
            StructuralUtil::getIcon (e->getType ()));
      }

      _dialog->setConditionInterface (
          entity->getProperty (STR_PROPERTY_REFERENCE_INTERFACE_ID));
    }
    else
    {
      foreach (StructuralEntity *e,
               entity->getTail ()->getChildren ())
      {
        _dialog->addConditionInterface (
            e->getUid (), e->getId (),
            StructuralUtil::getIcon (e->getType ()));
      }

      if (!STR_DEFAULT_WITH_INTERFACES)
      {
        if (!entity
                 ->getProperty (STR_PROPERTY_REFERENCE_INTERFACE_UID)
                 .isEmpty ()
            && _entities.contains (entity->getProperty (
                   STR_PROPERTY_REFERENCE_INTERFACE_UID)))
        {
          StructuralEntity *interface = _entities.value (
              entity->getProperty (
                  STR_PROPERTY_REFERENCE_INTERFACE_UID));

          _dialog->setConditionInterface (interface->getId ());
        }
      }
    }
  }
  else if (StructuralUtil::isAction (entity->getRole ()))
  {

    _dialog->setMode (entity->getTail ()->getProperty (
                          STR_PROPERTY_REFERENCE_XCONNECTOR_ID),
                      "", "", StructuralLinkDialog::EditAction);

    _dialog->setAction (entity->getId ());
    _dialog->setActionParams (pBind);

    if (entity->getHead ()->getCategory () == Structural::Interface)
    {
      foreach (
          StructuralEntity *e,
          entity->getHead ()->getParent ()->getChildren ())
      {
        _dialog->addActionInterface (
            e->getUid (), e->getId (),
            StructuralUtil::getIcon (e->getType ()));
      }

      _dialog->setActionInterface (
          entity->getProperty (STR_PROPERTY_REFERENCE_INTERFACE_ID));
    }
    else
    {
      foreach (StructuralEntity *e,
               entity->getHead ()->getChildren ())
      {
        _dialog->addActionInterface (
            e->getUid (), e->getId (),
            StructuralUtil::getIcon (e->getType ()));
      }

      if (!STR_DEFAULT_WITH_INTERFACES)
      {
        if (!entity
                 ->getProperty (STR_PROPERTY_REFERENCE_INTERFACE_UID)
                 .isEmpty ()
            && _entities.contains (entity->getProperty (
                   STR_PROPERTY_REFERENCE_INTERFACE_UID)))
        {
          StructuralEntity *interface = _entities.value (
              entity->getProperty (
                  STR_PROPERTY_REFERENCE_INTERFACE_UID));

          _dialog->setActionInterface (interface->getId ());
        }
      }
    }
  }

  if (_dialog->exec ())
  {
    QMap<QString, QString> prev = entity->getProperties ();
    QMap<QString, QString> properties = entity->getProperties ();

    QString role;
    QString property;
    QString interface;
    QMap<QString, QString> p;

    if (StructuralUtil::isCondition (entity->getRole ()))
    {
      role = _dialog->getCondition ();
      property = STR_PROPERTY_EDGE_TAIL;
      interface = _dialog->getConditionInterface ();
      p = _dialog->getConditionParams ();
    }
    else if (StructuralUtil::isAction (entity->getRole ()))
    {
      role = _dialog->getAction ();
      property = STR_PROPERTY_EDGE_HEAD;
      interface = _dialog->getActionInterface ();
      p = _dialog->getActionParams ();
    }

    properties[STR_PROPERTY_BIND_ROLE] = role;
    properties[STR_PROPERTY_ENTITY_ID] = role;

    if (!interface.isEmpty () && _entities.contains (interface))
    {
      properties[property] = interface;
      properties[STR_PROPERTY_REFERENCE_INTERFACE_UID] = interface;
      properties[STR_PROPERTY_REFERENCE_INTERFACE_ID]
          = _entities.value (interface)->getId ();
    }

    foreach (const QString &name, p.keys ())
    {
      if (properties.key (name).contains (STR_PROPERTY_BINDPARAM_NAME))
      {
        QString key = properties.key (name);
        QString uid = key.right (key.length () - key.lastIndexOf (':') - 1);

        if (!p.value (name).isEmpty ())
        {
          properties.insert (QString (STR_PROPERTY_BINDPARAM_NAME) + ":" + uid,
                             name);
          properties.insert (QString (STR_PROPERTY_BINDPARAM_VALUE) + ":"
                                 + uid,
                             p.value (name));
        }
        else
        {
          properties.remove (QString (STR_PROPERTY_BINDPARAM_NAME) + ":"
                             + uid);
          properties.remove (QString (STR_PROPERTY_BINDPARAM_VALUE) + ":"
                             + uid);
        }
      }
      else if (!p.value (name).isEmpty ())
      {
        QString uid = StructuralUtil::createUid ();

        properties.insert (QString (STR_PROPERTY_BINDPARAM_NAME) + ":" + uid,
                           name);
        properties.insert (QString (STR_PROPERTY_BINDPARAM_VALUE) + ":" + uid,
                           p.value (name));
      }
    }

    QMap<QString, QString> settings
        = StructuralUtil::createSettings (true, true);

    change (entity->getUid (), properties, prev, settings);
  }
}

StructuralEntity *
StructuralView::clone (StructuralEntity *entity, StructuralEntity *newparent)
{
  StructuralEntity *newentity = nullptr;

  switch (entity->getType ())
  {
    case Structural::Media:
    {
      newentity = new StructuralContent ();
      break;
    }

    case Structural::Link:
    {
      newentity = new StructuralLink ();
      break;
    }

    case Structural::Body:
    case Structural::Context:
    case Structural::Switch:
    {
      newentity = new StructuralComposition ();
      break;
    }

    case Structural::Area:
    case Structural::Property:
    case Structural::SwitchPort:
    case Structural::Port:
    {
      newentity = new StructuralInterface ();
      break;
    }

    case Structural::Mapping:
    {
      newentity = new StructuralEdge ();
      break;
    }

    case Structural::Bind:
    {
      newentity = new StructuralBind ();
      assert (newparent != nullptr);
      break;
    }

    default:
    {
      break;
    }
  }

  if (newentity != nullptr)
  {
    newentity->setProperties (entity->getProperties ());

    if (newparent != nullptr)
      newentity->setParent (newparent);

    foreach (StructuralEntity *child, entity->getChildren ())
      newentity->addChild (clone (child, newentity));
  }

  return newentity;
}

bool
StructuralView::isChild (StructuralEntity *entity, StructuralEntity *parent)
{
  if (entity != parent)
    foreach (StructuralEntity *e, parent->getChildren ())
      if (e->getUid () == entity->getUid ()
          || isChild (entity, e))
        return true;

  return false;
}

void
StructuralView::paste (StructuralEntity *entity, StructuralEntity *parent)
{
  paste (entity, parent, StructuralUtil::createUid (), true);
}

void
StructuralView::paste (StructuralEntity *entity, StructuralEntity *parent,
                       const QString &code, bool adjust)
{
  if (entity != nullptr)
  {
    QString uid = StructuralUtil::createUid ();

    QMap<QString, QString> properties = entity->getProperties ();
    properties[STR_PROPERTY_ENTITY_UID] = uid;

    if (adjust)
    {
      properties.remove (STR_PROPERTY_ENTITY_TOP);
      properties.remove (STR_PROPERTY_ENTITY_LEFT);
    }

    QMap<QString, QString> setting = StructuralUtil::createSettings (
        STR_VALUE_TRUE, STR_VALUE_TRUE, code);

    insert (uid, (parent != nullptr ? parent->getUid () : ""),
            properties, setting);

    if (_entities.contains (uid))
    {
      _clipboardReferences[entity->getUid ()] = uid;

      foreach (StructuralEntity *e, entity->getChildren ())
      {
        if (e->getCategory () != Structural::Edge
            && e->getType () != Structural::Port
            && e->getType () != Structural::Link)
        {

          paste (e, _entities.value (uid), code, false);
        }
      }

      foreach (StructuralEntity *e, entity->getChildren ())
      {
        if (e->getCategory () == Structural::Edge
            || e->getType () == Structural::Port
            || e->getType () == Structural::Link)
        {

          QMap<QString, QString> p = e->getProperties ();

          QString property = STR_PROPERTY_EDGE_TAIL;

          if (p.contains (property))
            if (_clipboardReferences.contains (p.value (property)))
              e->setProperty (
                  property, _clipboardReferences.value (p.value (property)));

          property = STR_PROPERTY_EDGE_HEAD;

          if (p.contains (property))
            if (_clipboardReferences.contains (p.value (property)))
              e->setProperty (
                  property, _clipboardReferences.value (p.value (property)));

          property = STR_PROPERTY_REFERENCE_COMPONENT_UID;

          if (p.contains (property))
            if (_clipboardReferences.contains (p.value (property)))
              e->setProperty (
                  property, _clipboardReferences.value (p.value (property)));

          property = STR_PROPERTY_REFERENCE_INTERFACE_UID;

          if (p.contains (property))
            if (_clipboardReferences.contains (p.value (property)))
              e->setProperty (
                  property, _clipboardReferences.value (p.value (property)));

          property = STR_PROPERTY_REFERENCE_LINK_UID;

          if (p.contains (property))
            if (_clipboardReferences.contains (p.value (property)))
              e->setProperty (
                  property, _clipboardReferences.value (p.value (property)));

          property = STR_PROPERTY_REFERENCE_XCONNECTOR_UID;

          if (p.contains (property))
            if (_clipboardReferences.contains (p.value (property)))
              e->setProperty (
                  property, _clipboardReferences.value (p.value (property)));

          foreach (const QString &k, p.keys ())
            if (k.contains (STR_PROPERTY_LINKPARAM_NAME)
                || k.contains (STR_PROPERTY_LINKPARAM_VALUE)
                || k.contains (STR_PROPERTY_BINDPARAM_NAME)
                || k.contains (STR_PROPERTY_BINDPARAM_VALUE))
            {

              property = k.left (k.length () - k.lastIndexOf (':') - 1);

              e->setProperty (
                  property + ":" + StructuralUtil::createUid (), p.value (k));
              e->setProperty (k, "");
            }

          paste (e, _entities.value (uid), code, false);
        }
      }
    }
  }
}

#ifdef WITH_GRAPHVIZ
#include "graphviz/cgraph.h"
#include "graphviz/gvc.h"

#define WITH_INTERFACE_AS_NODE 1

void
StructuralView::adjustLayout ()
{
  StructuralEntity *entity = nullptr;

  if (_entities.contains (_selected))
    entity = _entities.value (_selected);

  adjustLayout (entity, StructuralUtil::createUid ());
}

void
StructuralView::adjustLayout (StructuralEntity *entity, const QString &code)
{
  QVector<StructuralEntity *> children;

  if (entity != nullptr)
  {
    children = entity->getStructuralEntities ();
  }
  else if (!STR_DEFAULT_WITH_BODY)
  {
    foreach (StructuralEntity *current, _entities.values ())
      if (current->getStructuralParent () == nullptr)
        children += current;
  }

  // Autoadjust children
  foreach (StructuralEntity *e, children)
  {
    if (e->getStructuralType () == Structural::Context
        || e->getStructuralType () == Structural::Switch
        || e->getStructuralType () == Structural::Body)
    {

      adjustLayout (e, code);
    }
  }

  // Create graphviz context
  GVC_t *c;
  c = gvContext ();

  // Create graphviz graph
  Agraph_t *g;
  g = agopen ("g", Agdirected, nullptr);

  qreal GRAPHVIZ_DPI = 72;

  QMap<QString, Agnode_t *> nodes;

  // Add nodes in graphviz graph based on children entities.
  // Skip link entities.
  foreach (StructuralEntity *e, children)
  {
    if ((e->getCategory () == Structural::Node
         || (WITH_INTERFACE_AS_NODE
             && e->getCategory () == Structural::Interface))
        && e->getStructuralType () != Structural::Link)
    {

      QString w = QString::number (e->getWidth () / GRAPHVIZ_DPI);
      QString h = QString::number (e->getHeight () / GRAPHVIZ_DPI);

      Agnode_t *node = agnode (
          g, (char *)e->getStructuralUid ().toStdString ().c_str (), 1);
      agsafeset (node, "width", (char *)w.toStdString ().c_str (),
                 (char *)w.toStdString ().c_str ());
      agsafeset (node, "height", (char *)h.toStdString ().c_str (),
                 (char *)h.toStdString ().c_str ());

      nodes.insert (e->getStructuralUid (), node);
    }
  }

  //    QString r = nodes.firstKey();
  //    agsafeset(g, "root", (char*) r.toStdString().c_str(), (char*)
  //    r.toStdString().c_str());

  // Load condition and action entities for each link entity.
  QMap<QString, StructuralEntity *> links;
  QMap<QString, QVector<StructuralEntity *> > tails;
  QMap<QString, QVector<StructuralEntity *> > heads;

  foreach (StructuralEntity *e, children)
  {
    if (e->getCategory () == Structural::Edge)
    {
      StructuralEdge *ee = (StructuralEdge *)e;

      StructuralEntity *tail = ee->getTail ();
      StructuralEntity *head = ee->getHead ();

      if (tail != nullptr && head != nullptr)
      {
        if (tail->getStructuralType () == Structural::Link)
        {
          QString uid = tail->getStructuralUid ();

          links.insert (uid, tail);

          if (!heads.contains (uid))
            heads.insert (uid, QVector<StructuralEntity *> ());

          // In case the bind connect the link entity with a interface entity
          // (Node -> Interface) consider the parent of the interface entity
          // instead (Node -> Node).
          if (head->getCategory () == Structural::Interface
              && head->getStructuralParent () != entity)
            head = head->getStructuralParent ();

          QVector<StructuralEntity *> entities = heads.value (uid);
          entities.append (head);

          heads.insert (uid, entities);
        }
        else if (head->getStructuralType () == Structural::Link)
        {
          QString uid = head->getStructuralUid ();

          links.insert (uid, head);

          if (!tails.contains (uid))
            tails.insert (uid, QVector<StructuralEntity *> ());

          // In case the bind connect the link entity with a interface entity
          // (Interface -> Node) consider the parent of the interface entity
          // instead (Node -> Node).
          if (tail->getCategory () == Structural::Interface
              && tail->getStructuralParent () != entity)
            tail = tail->getStructuralParent ();

          QVector<StructuralEntity *> entities = tails.value (uid);
          entities.append (tail);

          tails.insert (uid, entities);
        }
        else if (WITH_INTERFACE_AS_NODE)
        {
          if (e->getStructuralType () == Structural::Link)
          {
            QString uid = e->getStructuralUid ();

            links.insert (uid, e);

            if (!heads.contains (uid))
              heads.insert (uid, QVector<StructuralEntity *> ());

            // In case the bind connect the link entity with a interface entity
            // (Node -> Interface) consider the parent of the interface entity
            // instead (Node -> Node).
            if (head->getCategory () == Structural::Interface
                && head->getStructuralParent () != entity)
              head = head->getStructuralParent ();

            QVector<StructuralEntity *> headentities = heads.value (uid);
            headentities.append (head);

            heads.insert (uid, headentities);

            if (!tails.contains (uid))
              tails.insert (uid, QVector<StructuralEntity *> ());

            // In case the bind connect the link entity with a interface entity
            // (Interface -> Node) consider the parent of the interface entity
            // instead (Node -> Node).
            if (tail->getCategory () == Structural::Interface
                && tail->getStructuralParent () != entity)
              tail = tail->getStructuralParent ();

            QVector<StructuralEntity *> tailentities = tails.value (uid);
            tailentities.append (tail);

            tails.insert (uid, tailentities);
          }
        }
      }
    }
  }

  QMap<Agedge_t *, StructuralEntity *> edges;

  // Add edges in graphviz graph based on the tails and heads loaded
  // from each link entity.
  foreach (StructuralEntity *link, links)
  {
    QString uid = link->getStructuralUid ();

    foreach (StructuralEntity *tail, tails.value (uid))
    {
      foreach (StructuralEntity *head, heads.value (uid))
      {
        if (nodes.contains (tail->getStructuralUid ())
            && nodes.contains (head->getStructuralUid ()))
        {

          Agedge_t *edge
              = agedge (g, nodes.value (tail->getStructuralUid ()),
                        nodes.value (head->getStructuralUid ()), nullptr, 1);

          edges.insert (edge, link);
        }
      }
    }
  }

  agattr (g, AGRAPH, "splines", "polyline");
  agattr (g, AGRAPH, "nodesep", "0.4");
  agattr (g, AGNODE, "shape", "box");
  agattr (g, AGNODE, "nodesep", "0.4");
  agattr (g, AGEDGE, "minlen", "3");

  // Adjust graph, nodes and edges size and pos using 'dot' algorithm.
  gvLayout (c, g, "dot");

  // Update entity size in view.
  qreal PADDING = 64 * 2;

  qreal nextTop;
  qreal nextLeft;
  qreal nextWidth;
  qreal nextHeight;

  if (entity != nullptr)
  {
    nextTop = entity->getTop ();
    nextLeft = entity->getLeft ();
    nextWidth = GD_bb (g).UR.x + PADDING;
    nextHeight = GD_bb (g).UR.y + PADDING;

    change (entity->getStructuralUid (),
            StructuralUtil::createProperties (nextTop, nextLeft, nextWidth,
                                              nextHeight),
            entity->getStructuralProperties (),
            StructuralUtil::createSettings (STR_VALUE_TRUE, STR_VALUE_FALSE,
                                            code));
  }

  qreal parentWidth;
  qreal parentHeight;

  if (entity != nullptr)
  {
    parentWidth = entity->getWidth ();
    parentHeight = entity->getHeight ();
  }
  else if (!STR_DEFAULT_WITH_BODY)
  {
    parentWidth = _scene->width ();
    parentHeight = _scene->height ();
  }

  // Update children entities 'pos' in view. Skip link entities for now.
  foreach (const QString &key, nodes.keys ())
  {
    Agnode_t *n = nodes.value (key);
    StructuralEntity *e = _entities.value (key);

    nextTop = (GD_bb (g).UR.y - ND_coord (n).y) + parentHeight / 2
              - GD_bb (g).UR.y / 2 - e->getHeight () / 2;
    nextLeft = ND_coord (n).x + parentWidth / 2 - GD_bb (g).UR.x / 2
               - e->getWidth () / 2;

    QMap<QString, QString> properties = e->getStructuralProperties ();
    properties[STR_PROPERTY_ENTITY_TOP] = QString::number (nextTop);
    properties[STR_PROPERTY_ENTITY_LEFT] = QString::number (nextLeft);

    change (e->getStructuralUid (), properties, e->getStructuralProperties (),
            StructuralUtil::createSettings (STR_VALUE_TRUE, STR_VALUE_FALSE,
                                            code));
  }

  /*
  // Update link entities 'pos' based on
  // boundbox.
  //
  // Note:
  // This is NOT the best solution.
  //
  // Known Issues:
  // - link entity can be moved into a composition
  // - links entity can be moved to a same position
  //
  qreal boxTop;
  qreal boxLeft;
  qreal boxWidth;
  qreal boxHeight;

  foreach (StructuralEntity* link, links.values()) {
    boxTop = 1024*1024;;
    boxLeft = 1024*1024;
    boxWidth = 0;
    boxHeight = 0;

    foreach (StructuralEntity* tail, tails.value(link->getStructuralUid())) {
      if (tail->getTop() < boxTop)
        boxTop = tail->getTop();

      if (tail->getLeft() < boxLeft)
        boxLeft = tail->getLeft();

      if (tail->getLeft() + tail->getWidth() > boxWidth)
        boxWidth = tail->getLeft() + tail->getWidth();

      if (tail->getTop() + tail->getHeight() > boxHeight)
        boxHeight = tail->getTop() + tail->getHeight();
    }

    foreach (StructuralEntity* head, heads.value(link->getStructuralUid())) {
      if (head->getTop() < boxTop)
        boxTop = head->getTop();

      if (head->getLeft() < boxLeft)
        boxLeft = head->getLeft();

      if (head->getLeft() + head->getWidth() > boxWidth)
        boxWidth = head->getLeft() + head->getWidth();

      if (head->getTop() + head->getHeight() > boxHeight)
        boxHeight = head->getTop() + head->getHeight();
    }

    boxWidth = boxWidth - boxLeft;
    boxHeight = boxHeight - boxTop;

    nextTop = boxTop + boxHeight/2 - link->getHeight()/2;
    nextLeft = boxLeft + boxWidth/2 - link->getWidth()/2;

    change(link->getStructuralUid(),
           StructuralUtil::createProperties(nextTop, nextLeft),
           link->getStructuralProperties(),
           StructuralUtil::createSettings(STR_VALUE_TRUE, STR_VALUE_FALSE,
  code));
  }
  */

  // Update link entities 'pos' using
  // path generated by graphviz.
  //
  qreal p = 0.5;

  QMap<QString, QPointF> last;

  foreach (Agedge_t *edge, edges.keys ())
  {
    QVector<QLineF> lines;
    qreal length = 0.0;

    // Creating lines
    const splines *s = ED_spl (edge);

    if ((s->list != 0) && (s->list->size % 3 == 1))
    {
      QLineF currentLine;
      QPointF currentPoint;

      bezier b = s->list[0];

      if (b.sflag)
      {
        currentLine = QLineF (b.sp.x, b.sp.y, b.list[0].x, b.list[0].y);

        lines.append (currentLine);
        length += currentLine.length ();
      }

      currentPoint = QPointF (b.list[0].x, b.list[0].y);

      for (int i = 2; i < b.size; i += 2)
      {
        currentLine = QLineF (currentPoint.x (), currentPoint.y (),
                              b.list[i].x, b.list[i].y);

        lines.append (currentLine);
        length += currentLine.length ();

        currentPoint = QPointF (b.list[i].x, b.list[i].y);
      }

      if (b.eflag)
      {
        currentLine
            = QLineF (currentPoint.x (), currentPoint.y (), b.ep.x, b.ep.y);

        lines.append (currentLine);
        length += currentLine.length ();
      }

      // Set link 'pos'
      qreal currentLength = 0;

      foreach (QLineF line, lines)
      {
        if (currentLength + line.length () < length * p)
        {
          currentLength += line.length ();
        }
        else
        {
          StructuralEntity *link = edges.value (edge);

          QPointF next
              = line.pointAt (((length * p) - currentLength) / line.length ());

          if (last.contains (link->getStructuralUid ()))
          {
            next
                = QLineF (last[link->getStructuralUid ()], next).pointAt (0.5);
          }

          last[link->getStructuralUid ()] = next;

          nextTop = (GD_bb (g).UR.y - next.y ()) + parentHeight / 2
                    - GD_bb (g).UR.y / 2 - link->getHeight () / 2;
          nextLeft = next.x () + parentWidth / 2 - GD_bb (g).UR.x / 2
                     - link->getWidth () / 2;

          QMap<QString, QString> properties = link->getStructuralProperties ();
          properties[STR_PROPERTY_ENTITY_TOP] = QString::number (nextTop);
          properties[STR_PROPERTY_ENTITY_LEFT] = QString::number (nextLeft);

          change (link->getStructuralUid (), properties,
                  link->getStructuralProperties (),
                  StructuralUtil::createSettings (STR_VALUE_TRUE,
                                                  STR_VALUE_FALSE, code));

          break;
        }
      }
    }
  }
}
#endif
