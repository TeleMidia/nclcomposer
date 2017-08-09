#include "StructuralView.h"

#include <QDebug>
#include <QMessageBox>
#include <assert.h>

#include <QClipboard>
#include <QFileDialog>
#include <QMimeData>
#include <QUuid>

#include "StructuralUtil.h"

StructuralView::StructuralView (QWidget *parent) : QGraphicsView (parent)
{
  createObjects ();
  createConnection ();

  _mode = Structural::Pointing;

  _zoom = ZOOM_ORIGINAL;

  _linking = false;
  _linkingTail = NULL;
  _linkingHead = NULL;

  _selected = "";

  _clipboard = NULL;
  _tool = NULL;

  setAttribute (Qt::WA_TranslucentBackground);
  setAcceptDrops (true);
  //  setDragMode(ScrollHandDrag);
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

StructuralMenu *
StructuralView::getMenu ()
{
  return _menu;
}

StructuralEntity *
StructuralView::getBody ()
{
  StructuralEntity *entity = NULL;

  foreach (StructuralEntity *e, _entities.values ())
  {
    if (e->getStructuralType () == Structural::Body)
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

QString
StructuralView::getError (const QString &uid) const
{
  QString error = "";

  if (_entities.contains (uid))
  {
    StructuralEntity *entity = _entities.value (uid);
    error = entity->getError ();
  }

  return error;
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
              == StructuralUtil::translateTypeToString (Structural::Body))
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

      if (e->getStructuralCategory () == Structural::Edge
          || e->getStructuralType () == Structural::Port || e->isReference ())
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
      if (e->getStructuralType () == Structural::Body)
        createDocument (e, document, root);
  }
  else
  {
    foreach (StructuralEntity *e, _entities.values ())
      if (e->getStructuralParent () == NULL)
        createDocument (e, document, root);
  }

  document->appendChild (root);

  return document->toString (4);
}

void
StructuralView::createObjects ()
{
  // Creating menu...
  _menu = new StructuralMenu (this);

  // Creating scene...
  _scene = new StructuralScene (this);
  _scene->setMenu (_menu);

  // Creating minimap...
  _minimap = new StructuralMinimap (this);
  _minimap->init (this);
  _minimap->setMinimumSize (STR_DEFAULT_MINIMAP_W, STR_DEFAULT_MINIMAP_H);
  _minimap->setMaximumSize (STR_DEFAULT_MINIMAP_W * 2,
                            STR_DEFAULT_MINIMAP_H * 2);
  _minimap->show ();

  // Creating dialogs
  _dialog = new StructuralLinkDialog (this);

  // Setting...
  setScene (_scene);
  centerOn (0, 0);
}

void
StructuralView::resizeEvent (QResizeEvent *event)
{
  _minimap->move (event->size ().width () - _minimap->width (),
                  event->size ().height () - _minimap->height ());

  QGraphicsView::resizeEvent (event);
}

void
StructuralView::createConnection ()
{
  // Connecting with menu...
  connect (this, SIGNAL (switchedUndo (bool)), _menu,
           SLOT (switchUndo (bool)));
  connect (this, SIGNAL (switchedRedo (bool)), _menu,
           SLOT (switchRedo (bool)));
  connect (this, SIGNAL (switchedCut (bool)), _menu, SLOT (switchCut (bool)));
  connect (this, SIGNAL (switchedCopy (bool)), _menu,
           SLOT (switchCopy (bool)));
  connect (this, SIGNAL (switchedPaste (bool)), _menu,
           SLOT (switchPaste (bool)));
  connect (this, SIGNAL (switchedBody (bool)), _menu,
           SLOT (switchBody (bool)));

  connect (_menu, SIGNAL (performedHelp ()), SLOT (performHelp ()));
  connect (_menu, SIGNAL (performedAutostart ()), SLOT (performAutostart ()));
  connect (_menu, SIGNAL (performedUndo ()), SLOT (performUndo ()));
  connect (_menu, SIGNAL (performedRedo ()), SLOT (performRedo ()));
  connect (_menu, SIGNAL (performedCut ()), SLOT (performCut ()));
  connect (_menu, SIGNAL (performedCopy ()), SLOT (performCopy ()));
  connect (_menu, SIGNAL (performedPaste ()), SLOT (paste ()));
  connect (_menu, SIGNAL (performedDelete ()), SLOT (performDelete ()));
  connect (_menu, SIGNAL (performedSnapshot ()), SLOT (performSnapshot ()));
  connect (_menu, SIGNAL (performedMedia ()), SLOT (performMedia ()));
  connect (_menu, SIGNAL (performedContext ()), SLOT (performContext ()));
  connect (_menu, SIGNAL (performedSwitch ()), SLOT (performSwitch ()));
  connect (_menu, SIGNAL (performedBody ()), SLOT (performBody ()));
  connect (_menu, SIGNAL (performedArea ()), SLOT (performArea ()));
  connect (_menu, SIGNAL (performedProperty ()), SLOT (performProperty ()));
  connect (_menu, SIGNAL (performedPort ()), SLOT (performPort ()));
  connect (_menu, SIGNAL (performedSwitchPort ()),
           SLOT (performSwitchPort ()));

#ifdef WITH_GRAPHVIZ
  connect (_menu, SIGNAL (performedAutoAdjust ()),
           SLOT (performAutoAdjust ()));
#endif

  connect (_menu, SIGNAL (performedProperties ()),
           SLOT (performProperties ()));

  connect (_menu,
           SIGNAL (performedInsert (StructuralType, QMap<QString, QString>)),
           SLOT (performInsert (StructuralType, QMap<QString, QString>)));
}

void
StructuralView::performZoomIn ()
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
StructuralView::performZoomOut ()
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
StructuralView::performZoomOriginal ()
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
  if (entity->getStructuralType () != Structural::Reference)
  {
    QDomElement element = document->createElement ("entity");
    element.setAttribute ("uid", entity->getStructuralUid ());
    element.setAttribute ("type", entity->getStructuralType ());

    foreach (const QString &key, entity->getStructuralProperties ().keys ())
    {
      QDomElement property = document->createElement ("property");
      property.setAttribute ("name", key);
      property.setAttribute ("value", entity->getStructuralProperty (key));

      element.appendChild (property);
    }

    foreach (StructuralEntity *e, entity->getStructuralEntities ())
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

    StructuralEntity *e = NULL;
    StructuralEntity *p = _entities.value (parent, NULL);

    //
    // Creating...
    //

    StructuralType type = StructuralUtil::translateStringToType (
        properties[STR_PROPERTY_ENTITY_TYPE]);

    if (p != NULL || !STR_DEFAULT_WITH_BODY)
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
          e->setStructuralType (type);

          // Connecting...
          // nothing
          break;

        case Structural::Link:
        {
          // Creating...
          e = new StructuralLink ();
          e->setStructuralType (type);

          // Connecting...
          connect ((StructuralLink *)e,
                   SIGNAL (performedEdit (StructuralLink *)),
                   SLOT (performDialog (StructuralLink *)));

          break;
        }

        case Structural::Area:
        case Structural::Property:
        case Structural::Port:
        case Structural::SwitchPort:
        {
          // Creating...
          e = new StructuralInterface ();
          e->setStructuralType (type);

          // Connecting...
          // nothing
          break;
        }

        case Structural::Reference:
        case Structural::Mapping:
        {
          // Creating...
          e = new StructuralEdge ();
          e->setStructuralType (type);

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
          e->setStructuralType (type);

          // Setting...
          // nothing

          // Connecting...
          connect (((StructuralBind *)e),
                   SIGNAL (performedEdit (StructuralBind *)),
                   SLOT (performDialog (StructuralBind *)));

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
          if (getBody () == NULL)
          {
            // Creating...
            e = new StructuralComposition ();
            e->setStructuralType (type);

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

    if (e != NULL)
    {

      //
      // Setting...
      //

      // Setting 'uid'...
      e->setStructuralUid (uid);

      // Setting 'id'...
      if (!properties.contains (STR_PROPERTY_ENTITY_ID))
        e->setStructuralId (getNewId (e));

      // Setting 'parent'...
      if (p != NULL)
        e->setStructuralParent (p);
      else
        _scene->addItem (e);

      // Setting 'top'...
      if (!properties.contains (STR_PROPERTY_ENTITY_TOP))
      {
        if (p != NULL)
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
        if (p != NULL)
          e->setLeft (p->getWidth () / 2 - e->getWidth () / 2);
        else
        {
          auto rect = this->mapToScene (this->viewport ()->rect ().center ());
          e->setLeft (rect.x () - e->getWidth () / 2);
        }
      }

      // Setting 'menu'...
      e->setMenu (_menu);

      // Setting 'others'...
      e->setStructuralProperties (properties);

      //
      _entities[uid] = e;

      //
      // Adjusting...
      //

      // Adjust 'compositions'
      if (p != NULL)
      {
        if (!p->isUncollapsed ())
        {
          if (p->getStructuralType () == Structural::Body
              || p->getStructuralType () == Structural::Switch
              || p->getStructuralType () == Structural::Context)
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
      if (e->getStructuralType () == Structural::Bind)
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
        if (e->getStructuralCategory () == Structural::Interface)
        {
          if (p != NULL)
          {
            if (p->getStructuralType () == Structural::Media)
            {
              if (p->isReference ())
                adjustReferences (p);
              else
                foreach (const QString &key, _references.keys ())
                  if (_references.contains (key))
                    if (_references.value (key) == p->getStructuralUid ())
                      if (_entities.contains (key))
                        adjustReferences (_entities.value (key));
            }
          }
        }
      }

      //
      // Connecting...
      //

      connect (e, SIGNAL (inserted (QString, QString, QMap<QString, QString>,
                                    QMap<QString, QString>)),
               SLOT (insert (QString, QString, QMap<QString, QString>,
                             QMap<QString, QString>)));
      connect (e, SIGNAL (removed (QString, QMap<QString, QString>)),
               SLOT (remove (QString, QMap<QString, QString>)));
      connect (
          e, SIGNAL (changed (QString, QMap<QString, QString>,
                              QMap<QString, QString>, QMap<QString, QString>)),
          SLOT (change (QString, QMap<QString, QString>,
                        QMap<QString, QString>, QMap<QString, QString>)));
      connect (e, SIGNAL (selected (QString, QMap<QString, QString>)),
               SLOT (select (QString, QMap<QString, QString>)));
      connect (e, SIGNAL (moved (QString, QString, QMap<QString, QString>,
                                 QMap<QString, QString>)),
               SLOT (move (QString, QString, QMap<QString, QString>,
                           QMap<QString, QString>)));

      //
      // Saving...
      //

      if (settings[STR_SETTING_UNDO] == STR_VALUE_TRUE)
      {
        Insert *command = new Insert (uid, parent,
                                      e->getStructuralProperties (), settings);
        command->setText (settings[STR_SETTING_CODE]);

        connect (command,
                 SIGNAL (insert (QString, QString, QMap<QString, QString>,
                                 QMap<QString, QString>)),
                 SLOT (insert (QString, QString, QMap<QString, QString>,
                               QMap<QString, QString>)));
        connect (command, SIGNAL (remove (QString, QMap<QString, QString>)),
                 SLOT (remove (QString, QMap<QString, QString>)));
        connect (
            command,
            SIGNAL (change (QString, QMap<QString, QString>,
                            QMap<QString, QString>, QMap<QString, QString>)),
            SLOT (change (QString, QMap<QString, QString>,
                          QMap<QString, QString>, QMap<QString, QString>)));
        connect (command, SIGNAL (select (QString, QMap<QString, QString>)),
                 SLOT (select (QString, QMap<QString, QString>)));

        _commnads.push (command);
        emit switchedUndo (true);
      }

      //
      // Notifing...
      //

      if (settings[STR_SETTING_NOTIFY] == STR_VALUE_TRUE)
      {
        emit inserted (uid, parent, e->getStructuralProperties (), settings);
      }

      if (!STR_DEFAULT_WITH_INTERFACES)
      {
        // Since interfaces are hidden, no update related with that entities
        // are notified. However, the structural view create a new id when this
        // property
        // is empty, then the change must be explicit notified for the hidden
        // entities
        // (interface in this case).
        emit changed (uid, e->getStructuralProperties (),
                      e->getStructuralProperties (), // this param is not used
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

  if (bind->getTail () != NULL && bind->getHead () != NULL)
  {
    foreach (StructuralEntity *e, _entities.values ())
    {
      if (e->getStructuralType () == Structural::Bind)
      {
        if (e != bind)
        {
          StructuralBind *b = (StructuralBind *)e;

          if (b->getTail () != NULL && b->getHead () != NULL)
          {
            if ((bind->getTail () == b->getTail ()
                 || bind->getTail () == b->getHead ())
                && (bind->getHead () == b->getHead ()
                    || bind->getHead () == b->getTail ()))
            {

              int current = b->getAngle ();

              if (b->getHead ()->getStructuralType () != Structural::Link)
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

    if (bind->getHead ()->getStructuralType () != Structural::Link)
      return -angle;
  }

  return angle;
}

void
StructuralView::remove (QString uid, QMap<QString, QString> settings)
{
  if (_entities.contains (uid))
  {
    //
    // Initializing...
    //
    StructuralEntity *e = _entities.value (uid);
    StructuralEntity *p = e->getStructuralParent ();

    StructuralType type = e->getStructuralType ();

    //
    // Saving...
    //
    if (settings[STR_SETTING_UNDO] == STR_VALUE_TRUE
        && (!e->isReference ()
            || e->getStructuralCategory () != Structural::Interface))
    {
      QString parent = "";

      if (_entities[uid]->getStructuralParent () != NULL)
        parent = _entities[uid]->getStructuralParent ()->getStructuralUid ();

      Remove *command
          = new Remove (_entities[uid]->getStructuralUid (), parent,
                        _entities[uid]->getStructuralProperties (), settings);
      command->setText (settings[STR_SETTING_CODE]);

      connect (command,
               SIGNAL (insert (QString, QString, QMap<QString, QString>,
                               QMap<QString, QString>)),
               SLOT (insert (QString, QString, QMap<QString, QString>,
                             QMap<QString, QString>)));
      connect (command, SIGNAL (remove (QString, QMap<QString, QString>)),
               SLOT (remove (QString, QMap<QString, QString>)));
      connect (command, SIGNAL (change (QString, QMap<QString, QString>,
                                        QMap<QString, QString>,
                                        QMap<QString, QString>)),
               SLOT (change (QString, QMap<QString, QString>,
                             QMap<QString, QString>, QMap<QString, QString>)));
      connect (command, SIGNAL (select (QString, QMap<QString, QString>)),
               SLOT (select (QString, QMap<QString, QString>)));

      _commnads.push (command);
      emit switchedUndo (true);
      return;
    }

    //
    // Removing...
    //

    // Removing 'references'...
    if (e->getStructuralCategory () == Structural::Interface)
    {
      foreach (const QString &key, _references.keys (e->getStructuralUid ()))
        remove (key, StructuralUtil::createSettings (
                         STR_VALUE_FALSE, STR_VALUE_FALSE,
                         settings.value (STR_SETTING_CODE)));

      // Only remove interface -> interface references. Keeping
      // Media -> Media references to enable undo. All "trash" references
      // are ignore when saving the project.
      _references.remove (e->getStructuralUid ());
    }
    else if (e->getStructuralType () == Structural::Media)
    {
      foreach (const QString &key, _references.keys (e->getStructuralUid ()))
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

    while (!e->getStructuralEntities ().isEmpty ())
      remove (e->getStructuralEntities ().first ()->getStructuralUid (),
              settings);

    // Removing 'edges'...
    if (e->getStructuralCategory () != Structural::Edge)
    {
      QVector<StructuralEntity *> relatives;
      relatives += StructuralUtil::getNeighbors (e);
      relatives += StructuralUtil::getUpNeighbors (e);

      foreach (StructuralEntity *entity, relatives)
      {
        if (entity->getStructuralCategory () == Structural::Edge)
        {
          StructuralEdge *edge = (StructuralEdge *)entity;

          if (edge->getTail () == e || edge->getHead () == e)
          {
            if (edge->getStructuralType () != Structural::Reference)
            {
              remove (edge->getStructuralUid (),
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
                    = edge->getTail ()->getStructuralProperties ();
                QMap<QString, QString> properties
                    = edge->getTail ()->getStructuralProperties ();

                properties[STR_PROPERTY_REFERENCE_COMPONENT_ID] = "";
                properties[STR_PROPERTY_REFERENCE_COMPONENT_UID] = "";
                properties[STR_PROPERTY_REFERENCE_INTERFACE_ID] = "";
                properties[STR_PROPERTY_REFERENCE_INTERFACE_UID] = "";

                change (edge->getTail ()->getStructuralUid (), properties,
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
      foreach (const QString &key, e->getStructuralProperties ().keys ())
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
      if (e->getStructuralType () == Structural::Port)
      {
        StructuralEntity *target = NULL;

        if (_entities.contains (e->getStructuralProperty (
                STR_PROPERTY_REFERENCE_COMPONENT_UID)))
        {
          target = _entities.value (
              e->getStructuralProperty (STR_PROPERTY_REFERENCE_COMPONENT_UID));

          if (_entities.contains (e->getStructuralProperty (
                  STR_PROPERTY_REFERENCE_INTERFACE_UID)))
          {
            target = _entities.value (e->getStructuralProperty (
                STR_PROPERTY_REFERENCE_INTERFACE_UID));
          }
        }

        if (target != NULL)
          target->setStructuralProperty (STR_PROPERTY_ENTITY_AUTOSTART,
                                         STR_VALUE_FALSE);
      }
    }

    // Removing 'others'...
    if (p != NULL)
      p->removeStructuralEntity (e);
    else
      _scene->removeItem (e);

    _entities.remove (uid);
    delete e;
    e = NULL;

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
      Change *command = new Change (uid, properties, previous, settings);
      command->setText (settings[STR_SETTING_CODE]);

      connect (command,
               SIGNAL (insert (QString, QString, QMap<QString, QString>,
                               QMap<QString, QString>)),
               SLOT (insert (QString, QString, QMap<QString, QString>,
                             QMap<QString, QString>)));
      connect (command, SIGNAL (remove (QString, QMap<QString, QString>)),
               SLOT (remove (QString, QMap<QString, QString>)));
      connect (command, SIGNAL (change (QString, QMap<QString, QString>,
                                        QMap<QString, QString>,
                                        QMap<QString, QString>)),
               SLOT (change (QString, QMap<QString, QString>,
                             QMap<QString, QString>, QMap<QString, QString>)));
      connect (command, SIGNAL (select (QString, QMap<QString, QString>)),
               SLOT (select (QString, QMap<QString, QString>)));

      _commnads.push (command);
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
      if (entity->getStructuralType () == Structural::Port)
      {
        StructuralEntity *lasttarget = NULL;

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

        if (lasttarget != NULL)
          lasttarget->setStructuralProperty (STR_PROPERTY_ENTITY_AUTOSTART,
                                             STR_VALUE_FALSE);
      }
    }

    // Setting 'others'...
    entity->setStructuralProperties (properties);

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
  if (entity != NULL)
  {
    StructuralType type = entity->getStructuralType ();

    switch (type)
    {
      case Structural::Media:
      {
        bool hasChange = false;

        if (!entity->getStructuralProperty (STR_PROPERTY_REFERENCE_REFER_ID)
                 .isEmpty ())
        {
          if (_entities.contains (entity->getStructuralProperty (
                  STR_PROPERTY_REFERENCE_REFER_UID)))
          {
            StructuralEntity *refer
                = _entities.value (entity->getStructuralProperty (
                    STR_PROPERTY_REFERENCE_REFER_UID));

            if (entity != refer && !refer->isReference ())
            {
              if (refer->getStructuralType () == Structural::Media)
              {
                QString instance = "new";

                if (!entity
                         ->getStructuralProperty (
                             STR_PROPERTY_CONTENT_INSTANCE)
                         .isEmpty ())
                  instance = entity->getStructuralProperty (
                      STR_PROPERTY_CONTENT_INSTANCE);

                foreach (StructuralEntity *e, entity->getStructuralEntities ())
                  if (e->isReference ())
                    if (_entities.contains (e->getStructuralProperty (
                            STR_PROPERTY_REFERENCE_REFER_UID)))
                      if (_entities
                              .value (e->getStructuralProperty (
                                  STR_PROPERTY_REFERENCE_REFER_UID))
                              ->getStructuralParent ()
                          != refer)
                        remove (e->getStructuralUid (),
                                StructuralUtil::createSettings (false, false));

                QVector<QString> entities;

                QMap<QString, QString> mapReferToChild;
                QMap<QString, QString> mapChildToRefer;

                foreach (StructuralEntity *e, entity->getStructuralEntities ())
                {
                  if (e->isReference ())
                  {
                    mapChildToRefer[e->getStructuralUid ()]
                        = e->getStructuralProperty (
                            STR_PROPERTY_REFERENCE_REFER_UID);
                    mapReferToChild[e->getStructuralProperty (
                        STR_PROPERTY_REFERENCE_REFER_UID)]
                        = e->getStructuralUid ();
                  }

                  entities.append (e->getStructuralUid ());
                }

                foreach (StructuralEntity *e, refer->getStructuralEntities ())
                {
                  if (e->getStructuralCategory () == Structural::Interface)
                  {
                    if (!mapReferToChild.contains (e->getStructuralUid ()))
                    {
                      if (!entities.contains (
                              _references.value (e->getStructuralUid ())))
                      {
                        QString uid = StructuralUtil::createUid ();

                        QMap<QString, QString> properties
                            = e->getStructuralProperties ();
                        properties[STR_PROPERTY_ENTITY_UID] = uid;
                        properties[STR_PROPERTY_ENTITY_REFERENCE]
                            = STR_VALUE_TRUE;

                        properties[STR_PROPERTY_REFERENCE_REFER_ID]
                            = e->getStructuralId ();
                        properties[STR_PROPERTY_REFERENCE_REFER_UID]
                            = e->getStructuralUid ();

                        properties.remove (STR_PROPERTY_ENTITY_TOP);
                        properties.remove (STR_PROPERTY_ENTITY_LEFT);

                        mapChildToRefer[uid] = e->getStructuralUid ();
                        mapChildToRefer[e->getStructuralUid ()] = uid;

                        _references[uid] = e->getStructuralUid ();

                        QMap<QString, QString> settings
                            = StructuralUtil::createSettings (false, false);
                        settings[STR_SETTING_ADJUST_REFERENCE]
                            = STR_VALUE_FALSE;

                        insert (uid, entity->getStructuralUid (), properties,
                                settings);
                      }
                    }
                  }
                }

                if (instance == "new")
                {
                  foreach (StructuralEntity *e,
                           entity->getStructuralEntities ())
                    if (!e->isReference ())
                      foreach (const QString &key,
                               _references.keys (e->getStructuralUid ()))
                        remove (key,
                                StructuralUtil::createSettings (false, false));
                }
                else if (instance == "instSame" || instance == "gradSame")
                {
                  foreach (StructuralEntity *e,
                           entity->getStructuralEntities ())
                    if (!e->isReference ())
                      foreach (const QString &key,
                               _references.keys (e->getStructuralUid ()))
                        if (_entities.contains (key))
                          if (_entities.value (key)->getStructuralParent ()
                              != refer)
                            remove (key, StructuralUtil::createSettings (
                                             false, false));

                  bool hasNewEntity = false;

                  foreach (StructuralEntity *e,
                           entity->getStructuralEntities ())
                  {
                    if (e->getStructuralCategory () == Structural::Interface)
                    {
                      if (!mapChildToRefer.contains (e->getStructuralUid ()))
                      {
                        if (!_references.values ().contains (
                                e->getStructuralUid ()))
                        {
                          QString uid = StructuralUtil::createUid ();

                          QMap<QString, QString> properties
                              = e->getStructuralProperties ();
                          properties[STR_PROPERTY_ENTITY_UID] = uid;
                          properties[STR_PROPERTY_ENTITY_REFERENCE]
                              = STR_VALUE_TRUE;

                          properties.remove (STR_PROPERTY_ENTITY_TOP);
                          properties.remove (STR_PROPERTY_ENTITY_LEFT);

                          properties[STR_PROPERTY_REFERENCE_REFER_ID]
                              = e->getStructuralId ();
                          properties[STR_PROPERTY_REFERENCE_REFER_UID]
                              = e->getStructuralUid ();

                          _references[uid] = e->getStructuralUid ();

                          QMap<QString, QString> settings
                              = StructuralUtil::createSettings (false, false);
                          settings[STR_SETTING_ADJUST_REFERENCE]
                              = STR_VALUE_FALSE;

                          insert (uid, refer->getStructuralUid (), properties,
                                  settings);

                          hasNewEntity = true;
                        }
                      }
                    }
                  }

                  if (hasNewEntity)
                    foreach (const QString &key,
                             _references.keys (refer->getStructuralUid ()))
                      if (_entities.contains (key))
                        if (key != entity->getStructuralUid ())
                          adjustReferences (_entities.value (key));
                }

                _references[entity->getStructuralUid ()]
                    = refer->getStructuralUid ();
                entity->setReference (true);
                hasChange = true;
              }
            }
          }
        }

        if (_references.contains (entity->getStructuralUid ()))
        {
          if (!hasChange)
          {
            foreach (StructuralEntity *e, entity->getStructuralEntities ())
            {
              if (e->isReference ())
              {
                remove (e->getStructuralUid (),
                        StructuralUtil::createSettings (false, false));
              }
              else
              {
                foreach (const QString &key,
                         _references.keys (e->getStructuralUid ()))
                  remove (key, StructuralUtil::createSettings (false, false));
              }
            }

            entity->setReference (false);
          }
        }

        QVector<StructuralEntity *> relatives;
        relatives += StructuralUtil::getNeighbors (entity);
        relatives += StructuralUtil::getUpNeighbors (entity);

        foreach (StructuralEntity *relative, relatives)
        {
          if (relative->getStructuralCategory () == Structural::Edge)
          {
            StructuralEdge *edge = (StructuralEdge *)relative;

            if (edge->getTail () == entity || edge->getHead () == entity)
              adjustReferences (edge);
          }
        }

        break;
      }

      case Structural::Reference:
      case Structural::Mapping:
      case Structural::Bind:
      {
        if (_entities.contains (
                entity->getStructuralProperty (STR_PROPERTY_EDGE_TAIL)))
          ((StructuralEdge *)entity)
              ->setTail (_entities.value (
                  entity->getStructuralProperty (STR_PROPERTY_EDGE_TAIL)));

        if (_entities.contains (
                entity->getStructuralProperty (STR_PROPERTY_EDGE_HEAD)))
          ((StructuralEdge *)entity)
              ->setHead (_entities.value (
                  entity->getStructuralProperty (STR_PROPERTY_EDGE_HEAD)));

        bool isVisible = true;

        StructuralEntity *tail = ((StructuralEdge *)entity)->getTail ();
        StructuralEntity *head = ((StructuralEdge *)entity)->getHead ();

        if (tail != NULL && head != NULL)
        {
          if (!STR_DEFAULT_WITH_INTERFACES)
          {
            if (tail->getStructuralCategory () == Structural::Interface)
            {
              ((StructuralEdge *)entity)->setTail (tail->getStructuralParent ());
            }

            if (head->getStructuralCategory () == Structural::Interface)
            {
              ((StructuralEdge *)entity)->setHead (head->getStructuralParent ());
            }
          }

          if (entity->getStructuralParent () != NULL)
            if (!entity->getStructuralParent ()->isUncollapsed ())
              isVisible = false;

          StructuralEntity *component = NULL;
          StructuralEntity *interface = NULL;

          if (!entity
                   ->getStructuralProperty (
                       STR_PROPERTY_REFERENCE_INTERFACE_ID)
                   .isEmpty ())
          {
            if (head->getStructuralId ()
                    != entity->getStructuralProperty (
                           STR_PROPERTY_REFERENCE_INTERFACE_ID)
                && tail->getStructuralId ()
                       != entity->getStructuralProperty (
                              STR_PROPERTY_REFERENCE_INTERFACE_ID))
              isVisible = false;

            if (_entities.contains (entity->getStructuralProperty (
                    STR_PROPERTY_REFERENCE_INTERFACE_UID)))
              interface = _entities.value (entity->getStructuralProperty (
                  STR_PROPERTY_REFERENCE_INTERFACE_UID));
            else
              isVisible = false;
          }

          if (!entity
                   ->getStructuralProperty (
                       STR_PROPERTY_REFERENCE_COMPONENT_ID)
                   .isEmpty ())
          {
            if (entity
                    ->getStructuralProperty (
                        STR_PROPERTY_REFERENCE_INTERFACE_ID)
                    .isEmpty ())
              if (head->getStructuralId ()
                      != entity->getStructuralProperty (
                             STR_PROPERTY_REFERENCE_COMPONENT_ID)
                  && tail->getStructuralId ()
                         != entity->getStructuralProperty (
                                STR_PROPERTY_REFERENCE_COMPONENT_ID))
                isVisible = false;

            if (_entities.contains (entity->getStructuralProperty (
                    STR_PROPERTY_REFERENCE_COMPONENT_UID)))
              component = _entities.value (entity->getStructuralProperty (
                  STR_PROPERTY_REFERENCE_COMPONENT_UID));
            else
              isVisible = false;
          }
          else
          {
            isVisible = false;
          }

          if (component != NULL)
          {
            if (interface != NULL)
              if (interface->getStructuralParent () != component)
                isVisible = false;

            if (entity->getStructuralParent ()
                != component->getStructuralParent ())
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

      case Structural::Port:
      {
        foreach (StructuralEntity *e, _entities.values ())
          if (e->getStructuralType () == Structural::Reference)
            if (e->getStructuralProperty (STR_PROPERTY_EDGE_TAIL)
                == entity->getStructuralUid ())
              remove (e->getStructuralUid (),
                      StructuralUtil::createSettings (false, false));

        StructuralEntity *component = NULL;
        StructuralEntity *interface = NULL;

        if (_entities.contains (entity->getStructuralProperty (
                STR_PROPERTY_REFERENCE_INTERFACE_UID)))
          interface = _entities.value (entity->getStructuralProperty (
              STR_PROPERTY_REFERENCE_INTERFACE_UID));

        if (_entities.contains (entity->getStructuralProperty (
                STR_PROPERTY_REFERENCE_COMPONENT_UID)))
          component = _entities.value (entity->getStructuralProperty (
              STR_PROPERTY_REFERENCE_COMPONENT_UID));

        StructuralEntity *head = NULL;

        if (component != NULL)
        {
          head = component;

          if (interface != NULL)
            if (interface->getStructuralParent () == component)
              head = interface;

          if (head != NULL)
          {
            QMap<QString, QString> properties;
            properties[STR_PROPERTY_ENTITY_TYPE]
                = StructuralUtil::translateTypeToString (
                    Structural::Reference);
            properties[STR_PROPERTY_EDGE_TAIL] = entity->getStructuralUid ();
            properties[STR_PROPERTY_EDGE_HEAD] = head->getStructuralUid ();

            properties[STR_PROPERTY_REFERENCE_COMPONENT_ID]
                = entity->getStructuralProperty (
                    STR_PROPERTY_REFERENCE_COMPONENT_ID);
            properties[STR_PROPERTY_REFERENCE_COMPONENT_UID]
                = entity->getStructuralProperty (
                    STR_PROPERTY_REFERENCE_COMPONENT_UID);
            properties[STR_PROPERTY_REFERENCE_INTERFACE_ID]
                = entity->getStructuralProperty (
                    STR_PROPERTY_REFERENCE_INTERFACE_ID);
            properties[STR_PROPERTY_REFERENCE_INTERFACE_UID]
                = entity->getStructuralProperty (
                    STR_PROPERTY_REFERENCE_INTERFACE_UID);

            QString parentUid;

            if (entity->getStructuralParent () != NULL)
              parentUid = entity->getStructuralParent ()->getStructuralUid ();
            else
              parentUid = "";

            if (!STR_DEFAULT_WITH_BODY
                && !STR_DEFAULT_WITH_FLOATING_INTERFACES)
            {
              if (entity->getStructuralParent () == NULL)
              {
                entity->setHidden (true);

                head->setStructuralProperty (STR_PROPERTY_ENTITY_AUTOSTART,
                                             STR_VALUE_TRUE);

                properties[STR_PROPERTY_ENTITY_HIDDEN] = STR_VALUE_TRUE;
              }
            }

            insert (StructuralUtil::createUid (), parentUid, properties,
                    StructuralUtil::createSettings (false, false));
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
  if (_entities.contains (uid))
  {
    if (uid != _selected)
    {
      //
      // Initializing...
      //
      StructuralEntity *entity = _entities.value (_selected);

      if (entity != NULL)
        entity->setSelected (false);

      //
      // Selecting...
      //
      entity = _entities.value (uid);
      entity->setSelected (true);

      _selected = entity->getStructuralUid ();

      //
      // Notifing...
      //
      bool enablePaste = false;

      if (_clipboard != NULL)
        if (entity->getStructuralUid () != _clipboard->getStructuralUid ())
          if (StructuralUtil::validateKinship (
                  _clipboard->getStructuralType (),
                  entity->getStructuralType ()))
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

    if (entity != NULL)
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
      if (_clipboard != NULL)
        if (StructuralUtil::validateKinship (_clipboard->getStructuralType (),
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
  if (_entities.contains (uid))
  {
    //
    // Initializing...
    //
    StructuralEntity *e = _entities.value (uid);
    StructuralEntity *p = _entities.value (parent);

    StructuralType type = Structural::Body;

    if (p != NULL)
      type = p->getStructuralType ();

    if (StructuralUtil::validateKinship (e->getStructuralType (), type)
        && e->getStructuralParent () != p)
    {
      //
      // Setting...
      //
      foreach (StructuralEntity *entity, _entities.values ())
        entity->setDraggable (false);

      select ("", settings);

      StructuralEntity *copy = clone (e, NULL);

      foreach (const QString &name, properties.keys ())
        copy->setStructuralProperty (name, properties.value (name));

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
}

void
StructuralView::createEntity (StructuralType type)
{
  QMap<QString, QString> properties;
  properties[STR_PROPERTY_ENTITY_TYPE]
      = StructuralUtil::translateTypeToString (type);

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
    properties[STR_PROPERTY_ENTITY_TYPE]
        = StructuralUtil::translateTypeToString (type);

  if (!settings.contains (STR_SETTING_UNDO))
    settings[STR_SETTING_UNDO] = STR_VALUE_TRUE;

  if (!settings.contains (STR_SETTING_NOTIFY))
    settings[STR_SETTING_NOTIFY] = STR_VALUE_TRUE;

  if (!settings.contains (STR_SETTING_CODE))
    settings[STR_SETTING_CODE] = StructuralUtil::createUid ();

  insert (uid, parent, properties, settings);
}

void
StructuralView::performHelp ()
{
  // TODO
}

void
StructuralView::performAutostart ()
{
  if (_entities.contains (_selected))
  {
    StructuralEntity *entity = _entities.value (_selected);

    QMap<QString, QString> properties = entity->getStructuralProperties ();

    if (properties.contains (STR_PROPERTY_ENTITY_AUTOSTART))
    {
      if (properties[STR_PROPERTY_ENTITY_AUTOSTART] == STR_VALUE_TRUE)
        // properties.remove(STR_PROPERTY_ENTITY_AUTOSTART);
        properties[STR_PROPERTY_ENTITY_AUTOSTART] = STR_VALUE_FALSE;
      else
        properties[STR_PROPERTY_ENTITY_AUTOSTART] = STR_VALUE_TRUE;
    }
    else
    {
      properties[STR_PROPERTY_ENTITY_AUTOSTART] = STR_VALUE_TRUE;
    }

    change (entity->getStructuralUid (), properties,
            entity->getStructuralProperties (),
            StructuralUtil::createSettings (true, true));
  }
}

void
StructuralView::performUndo ()
{
  if (_commnads.canUndo ())
  {
    QString code = _commnads.undoText ();

    while (code == _commnads.undoText ())
      _commnads.undo ();

    emit switchedRedo (true);
  }

  if (!_commnads.canUndo ())
    emit switchedUndo (false);

  foreach (const QString &key, _references.keys ())
  {
    if (_entities.contains (key))
      if (_entities.value (key)->getStructuralType () == Structural::Media)
        adjustReferences (_entities.value (key));
  }

  foreach (const QString &key, _entities.keys ())
  {
    if (_entities.contains (key))
    {
      StructuralEntity *e = _entities.value (key);

      if (e->getStructuralCategory () == Structural::Edge)
      {
        StructuralEntity *c = _entities.value (
            e->getStructuralProperty (STR_PROPERTY_REFERENCE_COMPONENT_UID));
        StructuralEntity *i = _entities.value (
            e->getStructuralProperty (STR_PROPERTY_REFERENCE_INTERFACE_UID));

        if (c != NULL)
        {
          if (i == NULL)
          {
            foreach (StructuralEntity *r, c->getStructuralEntities ())
            {
              if (r->getStructuralId ()
                  == e->getStructuralProperty (
                         STR_PROPERTY_REFERENCE_INTERFACE_ID))
              {
                e->setStructuralProperty (STR_PROPERTY_REFERENCE_INTERFACE_UID,
                                          r->getStructuralUid ());

                if (((StructuralEdge *)e)->getTail () != NULL)
                  e->setStructuralProperty (STR_PROPERTY_EDGE_HEAD,
                                            r->getStructuralUid ());
                else
                  e->setStructuralProperty (STR_PROPERTY_EDGE_TAIL,
                                            r->getStructuralUid ());

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
  if (_commnads.canRedo ())
  {
    QString code = _commnads.redoText ();

    while (code == _commnads.redoText ())
      _commnads.redo ();

    emit switchedUndo (true);
  }

  if (!_commnads.canRedo ())
    emit switchedRedo (false);
}

void
StructuralView::performCut ()
{
  performCopy ();

  StructuralEntity *entity = _entities.value (_selected);

  if (entity != NULL)
  {
    _clipboard->setStructuralId (entity->getStructuralId ());

    if (_clipboard->getStructuralUid () == entity->getStructuralUid ())
      remove (_selected, StructuralUtil::createSettings ());
  }
}

void
StructuralView::performCopy ()
{
  if (_entities.contains (_selected))
  {
    if (_clipboard != NULL)
    {
      delete (_clipboard);

      _clipboardReferences.clear ();
    }

    StructuralEntity *entity = _entities.value (_selected);

    if (entity != NULL)
      if (entity->getStructuralCategory () != Structural::Edge
          && entity->getStructuralType () != Structural::Body)
      {
        _clipboard = clone (entity);
        _clipboard->setStructuralId ("c_" + _clipboard->getStructuralId ());

        emit switchedPaste (false);
      }
  }
}

void
StructuralView::performPaste ()
{
  if (_clipboard != NULL)
  {
    StructuralEntity *parent = NULL;

    if (_entities.contains (_selected))
      parent = _entities.value (_selected, NULL);

    bool isAValidPaste = false;

    if (!STR_DEFAULT_WITH_BODY)
    {
      if (StructuralUtil::validateKinship (_clipboard->getStructuralType (),
                                           Structural::Body))
        isAValidPaste = true;
    }
    else
    {
      if (parent != NULL)
        if (_clipboard->getStructuralUid () != parent->getStructuralUid ())
          if (StructuralUtil::validateKinship (
                  _clipboard->getStructuralType (),
                  parent->getStructuralType ())
              && !isChild (parent, _clipboard))
            isAValidPaste = true;
    }

    if (isAValidPaste)
    {
      // 0: Cancel - Nothing todo.
      // 1: No - Paste entity.
      // 2: Yes - Create refer entity.
      int result = 1;

      if (_entities.contains (_clipboard->getStructuralUid ()))
        result = QMessageBox::question (
            this, tr ("Paste"),
            tr ("Would you like to make a reference instead?"), tr ("Cancel"),
            tr ("No"), tr ("Yes"), 1);

      if (result == 1)
      {
        QString nextID = _clipboard->getStructuralId ();

        int n = 0;

        foreach (StructuralEntity *entity, _entities.values ())
          if (entity->getStructuralId ().startsWith (nextID))
            ++n;

        if (n > 0)
          nextID = nextID + "_" + QString::number (n);

        _clipboard->setStructuralId (nextID);

        paste (_clipboard, parent);
      }
      else if (result == 2)
      {
        QString uid = StructuralUtil::createUid ();

        QMap<QString, QString> properties
            = _clipboard->getStructuralProperties ();
        properties[STR_PROPERTY_ENTITY_UID] = uid;

        QString refID = _clipboard->getStructuralId ();
        refID = refID.section ('_', 1, 1);

        int n = 0;

        foreach (StructuralEntity *entity, _entities.values ())
          if (entity->getStructuralId ().startsWith ("r_" + refID))
            ++n;

        if (n > 0)
          refID = "r_" + refID + "_" + QString::number (n);
        else
          refID = "r_" + refID;

        properties[STR_PROPERTY_ENTITY_ID] = refID;

        properties[STR_PROPERTY_REFERENCE_REFER_ID]
            = _clipboard->getStructuralId ().section ('_', 1, 1);
        properties[STR_PROPERTY_REFERENCE_REFER_UID]
            = _clipboard->getStructuralUid ();

        properties.remove (STR_PROPERTY_ENTITY_TOP);
        properties.remove (STR_PROPERTY_ENTITY_LEFT);

        insert (uid, (parent != NULL ? parent->getStructuralUid () : ""),
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
      = (StructuralEntity *)tail->getStructuralParent ();
  StructuralEntity *parentHead
      = (StructuralEntity *)head->getStructuralParent ();

  if ((parentTail != NULL && parentHead != NULL) || !STR_DEFAULT_WITH_BODY)
  {
    StructuralEntity *parent = NULL;

    QPointF ptail;
    QPointF phead;

    if (parentTail == parentHead)
    {
      ptail = QPointF (tail->getLeft (), tail->getTop ());
      phead = QPointF (head->getLeft (), head->getTop ());

      parent = parentTail;
    }
    else if (parentTail != NULL
             && parentTail->getStructuralParent () == parentHead
             && tail->getStructuralCategory () == Structural::Interface)
    {
      phead = QPointF (head->getLeft (), head->getTop ());
      ptail = parentTail->mapToParent (tail->getLeft (), tail->getTop ());

      parent = parentHead;
    }
    else if (parentHead != NULL
             && parentTail == parentHead->getStructuralParent ()
             && head->getStructuralCategory () == Structural::Interface)
    {
      ptail = QPointF (tail->getLeft (), tail->getTop ());
      phead = parentHead->mapToParent (head->getLeft (), head->getTop ());

      parent = parentTail;
    }
    else if (parentTail != NULL && parentHead != NULL
             && parentTail->getStructuralParent ()
                    == parentHead->getStructuralParent ()
             && tail->getStructuralCategory () == Structural::Interface
             && head->getStructuralCategory () == Structural::Interface)
    {
      ptail = parentTail->mapToParent (tail->getLeft (), tail->getTop ());
      phead = parentHead->mapToParent (head->getLeft (), head->getTop ());

      parent = parentTail->getStructuralParent ();
    }

    setMode (Structural::Pointing);
    emit switchedPointer (true);

    if (parent != NULL || !STR_DEFAULT_WITH_BODY)
    {
      emit requestedUpdate ();

      if (tail->getStructuralCategory () == Structural::Interface)
      {
        foreach (StructuralEntity *entity,
                 parentTail->getStructuralEntities ())
        {
          _dialog->addConditionInterface (
              entity->getStructuralUid (), entity->getStructuralId (),
              StructuralUtil::getIcon (entity->getStructuralType ()));
        }

        _dialog->setConditionInterface (tail->getStructuralId ());
      }
      else
      {
        foreach (StructuralEntity *entity, tail->getStructuralEntities ())
        {
          _dialog->addConditionInterface (
              entity->getStructuralUid (), entity->getStructuralId (),
              StructuralUtil::getIcon (entity->getStructuralType ()));
        }
      }

      if (head->getStructuralCategory () == Structural::Interface)
      {
        foreach (StructuralEntity *entity,
                 parentHead->getStructuralEntities ())
        {
          _dialog->addActionInterface (
              entity->getStructuralUid (), entity->getStructuralId (),
              StructuralUtil::getIcon (entity->getStructuralType ()));
        }

        _dialog->setActionInterface (head->getStructuralId ());
      }
      else
      {
        foreach (StructuralEntity *entity, head->getStructuralEntities ())
        {
          _dialog->addActionInterface (
              entity->getStructuralUid (), entity->getStructuralId (),
              StructuralUtil::getIcon (entity->getStructuralType ()));
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
            parentTail = tail->getStructuralParent ();

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
            parentHead = head->getStructuralParent ();

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
            = StructuralUtil::translateTypeToString (Structural::Link);
        properties[STR_PROPERTY_REFERENCE_XCONNECTOR_ID]
            = _dialog->getConnector ();

        qreal x;
        qreal y;

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

        insert (uid, (parent != NULL ? parent->getStructuralUid () : ""),
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
  StructuralEntity *parentTail = tail->getStructuralParent ();
  StructuralEntity *parentHead = head->getStructuralParent ();

  if ((parentTail != NULL && parentHead != NULL) || !STR_DEFAULT_WITH_BODY)
  {
    StructuralEntity *parent = NULL;

    if (parentTail == parentHead)
      parent = parentTail;

    else if (parentTail != NULL
             && parentTail->getStructuralParent () == parentHead
             && tail->getStructuralCategory () == Structural::Interface)
      parent = parentHead;

    else if (parentHead != NULL
             && parentTail == parentHead->getStructuralParent ()
             && head->getStructuralCategory () == Structural::Interface)
      parent = parentTail;

    if (parent != NULL || !STR_DEFAULT_WITH_BODY)
    {
      QString uid = StructuralUtil::createUid ();

      QMap<QString, QString> properties;
      properties[STR_PROPERTY_ENTITY_TYPE]
          = StructuralUtil::translateTypeToString (Structural::Bind);
      properties[STR_PROPERTY_EDGE_TAIL] = tail->getStructuralUid ();
      properties[STR_PROPERTY_EDGE_HEAD] = head->getStructuralUid ();

      properties[STR_PROPERTY_BIND_ROLE] = role;
      properties[STR_PROPERTY_ENTITY_ID] = role;

      StructuralEntity *entityLink = NULL;
      StructuralEntity *entityNonLink = NULL;
      StructuralEntity *parentNonLink = NULL;

      if (tail->getStructuralType () == Structural::Link)
      {
        entityLink = tail;
        entityNonLink = head;
        parentNonLink = parentHead;

        if (role.isEmpty ())
        {
          emit requestedUpdate ();

          setMode (Structural::Pointing);
          emit switchedPointer (true);

          if (entityNonLink->getStructuralCategory () == Structural::Interface)
          {
            foreach (StructuralEntity *entity,
                     parentNonLink->getStructuralEntities ())
            {
              _dialog->addActionInterface (
                  entity->getStructuralUid (), entity->getStructuralId (),
                  StructuralUtil::getIcon (entity->getStructuralType ()));
            }

            _dialog->setActionInterface (entityNonLink->getStructuralId ());
          }
          else
          {
            foreach (StructuralEntity *entity,
                     entityNonLink->getStructuralEntities ())
            {
              _dialog->addActionInterface (
                  entity->getStructuralUid (), entity->getStructuralId (),
                  StructuralUtil::getIcon (entity->getStructuralType ()));
            }
          }

          _dialog->setMode (entityLink->getStructuralProperty (
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
                parentNonLink = entityNonLink->getStructuralParent ();

                properties[STR_PROPERTY_EDGE_HEAD]
                    = entityNonLink->getStructuralUid ();
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
      else if (head->getStructuralType () == Structural::Link)
      {
        entityLink = head;
        entityNonLink = tail;
        parentNonLink = parentTail;

        if (role.isEmpty ())
        {
          emit requestedUpdate ();

          setMode (Structural::Pointing);
          emit switchedPointer (true);

          if (entityNonLink->getStructuralCategory () == Structural::Interface)
          {
            foreach (StructuralEntity *entity,
                     parentNonLink->getStructuralEntities ())
            {
              _dialog->addConditionInterface (
                  entity->getStructuralUid (), entity->getStructuralId (),
                  StructuralUtil::getIcon (entity->getStructuralType ()));
            }

            _dialog->setConditionInterface (entityNonLink->getStructuralId ());
          }
          else
          {
            foreach (StructuralEntity *entity,
                     entityNonLink->getStructuralEntities ())
            {
              _dialog->addConditionInterface (
                  entity->getStructuralUid (), entity->getStructuralId (),
                  StructuralUtil::getIcon (entity->getStructuralType ()));
            }
          }

          _dialog->setMode (entityLink->getStructuralProperty (
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
                parentNonLink = entityNonLink->getStructuralParent ();

                properties[STR_PROPERTY_EDGE_TAIL]
                    = entityNonLink->getStructuralUid ();
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

      if (entityLink != NULL && entityNonLink != NULL)
      {
        properties[STR_PROPERTY_REFERENCE_LINK_UID]
            = entityLink->getStructuralUid ();

        if (entityNonLink->getStructuralCategory () == Structural::Interface)
        {
          properties[STR_PROPERTY_REFERENCE_INTERFACE_ID]
              = entityNonLink->getStructuralId ();
          properties[STR_PROPERTY_REFERENCE_INTERFACE_UID]
              = entityNonLink->getStructuralUid ();
          properties[STR_PROPERTY_REFERENCE_COMPONENT_ID]
              = parentNonLink->getStructuralId ();
          properties[STR_PROPERTY_REFERENCE_COMPONENT_UID]
              = parentNonLink->getStructuralUid ();
        }
        else
        {
          properties[STR_PROPERTY_REFERENCE_COMPONENT_ID]
              = entityNonLink->getStructuralId ();
          properties[STR_PROPERTY_REFERENCE_COMPONENT_UID]
              = entityNonLink->getStructuralUid ();
          properties[STR_PROPERTY_REFERENCE_INTERFACE_ID] = "";
          properties[STR_PROPERTY_REFERENCE_INTERFACE_UID] = "";
        }
      }

      QMap<QString, QString> settings
          = StructuralUtil::createSettings (true, true);

      if (!code.isEmpty ())
        settings[STR_SETTING_CODE] = code;

      insert (uid, (parent != NULL ? parent->getStructuralUid () : ""),
              properties, settings);
    }
  }
}

void
StructuralView::createReference (StructuralEntity *tail,
                                 StructuralEntity *head)
{
  if (tail->getStructuralType () == Structural::Port
      || tail->getStructuralType () == Structural::SwitchPort)
  {

    StructuralEntity *parentTail = tail->getStructuralParent ();
    StructuralEntity *parentHead = head->getStructuralParent ();

    if (parentTail == parentHead
        || (parentHead != NULL
            && parentTail == parentHead->getStructuralParent ()))
    {
      QMap<QString, QString> previous = tail->getStructuralProperties ();
      QMap<QString, QString> properties;

      if (tail->getStructuralType () == Structural::Port)
        properties = tail->getStructuralProperties ();

      if (head->getStructuralCategory () == Structural::Interface)
      {
        properties[STR_PROPERTY_REFERENCE_INTERFACE_ID]
            = head->getStructuralId ();
        properties[STR_PROPERTY_REFERENCE_INTERFACE_UID]
            = head->getStructuralUid ();
        properties[STR_PROPERTY_REFERENCE_COMPONENT_ID]
            = parentHead->getStructuralId ();
        properties[STR_PROPERTY_REFERENCE_COMPONENT_UID]
            = parentHead->getStructuralUid ();
      }
      else
      {
        properties[STR_PROPERTY_REFERENCE_COMPONENT_ID]
            = head->getStructuralId ();
        properties[STR_PROPERTY_REFERENCE_COMPONENT_UID]
            = head->getStructuralUid ();
        properties[STR_PROPERTY_REFERENCE_INTERFACE_ID] = "";
        properties[STR_PROPERTY_REFERENCE_INTERFACE_UID] = "";
      }

      setMode (Structural::Pointing);
      emit switchedPointer (true);

      if (tail->getStructuralType () == Structural::Port)
      {
        change (tail->getStructuralUid (), properties, previous,
                StructuralUtil::createSettings (true, true));
      }
      else if (tail->getStructuralType () == Structural::SwitchPort)
      {
        properties[STR_PROPERTY_ENTITY_TYPE]
            = StructuralUtil::translateTypeToString (Structural::Mapping);
        properties[STR_PROPERTY_EDGE_TAIL] = tail->getStructuralUid ();
        properties[STR_PROPERTY_EDGE_HEAD] = head->getStructuralUid ();

        insert (StructuralUtil::createUid (), parentTail->getStructuralUid (),
                properties, StructuralUtil::createSettings (true, true));
      }
    }
  }
}

QString
StructuralView::getNewId (StructuralEntity *entity)
{
  QString name = "";

  if (entity != NULL)
  {
    StructuralType type = entity->getStructuralType ();

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
          if (e->getStructuralId () == QString (prefix + QString::number (n)))
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
StructuralView::performSnapshot ()
{
  qreal top = _scene->height ();
  qreal left = _scene->width ();
  qreal bottom = 0.0;
  qreal right = 0.0;

  foreach (StructuralEntity *entity, _entities.values ())
  {
    if (entity->getStructuralParent () == NULL)
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

  StructuralEntity *selected = NULL;

  if (_entities.contains (_selected))
    selected = _entities.value (_selected);

  if (selected != NULL)
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
        NULL, tr ("Save snapshot..."), QDir::homePath (),
        tr ("Images (*.png)"));

    if (!location.isEmpty ())
    {
      if (!location.endsWith (".png"))
        location += ".png";

      image.save (location, "png");

      if (selected != NULL)
        selected->setSelected (true);
    }
  }
}

void
StructuralView::performMinimap ()
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
    if (_linkingTail != NULL)
      _linkingTail->setHovering (false);

    if (_linkingHead != NULL)
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

      if (_linkingTail != NULL)
        _linkingTail->setHovering (true);
    }

    if (headItems.count ())
    {
      _linkingHead = (StructuralEntity *)headItems.first ();

      if (_linkingHead != NULL)
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
    if (_tool != NULL)
    {
      delete (_tool);
      _tool = NULL;
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
    if (_linkingTail != NULL)
      _linkingTail->setHovering (false);

    if (_linkingHead != NULL)
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
        if (tail->getStructuralCategory () == Structural::Node
            && head->getStructuralCategory () == Structural::Node)
        {

          if (tail->getStructuralType () != Structural::Link
              && head->getStructuralType () != Structural::Link)
            createLink (tail, head);
          else if (tail->getStructuralType () == Structural::Link)
            createBind (tail, head);
          else if (head->getStructuralType () == Structural::Link)
            createBind (tail, head);

          // if linking NODE to INTERFACE
        }
        else if (tail->getStructuralCategory () == Structural::Node
                 && head->getStructuralCategory () == Structural::Interface)
        {
          if (tail->getStructuralType () != Structural::Link)
            createLink (tail, head);
          else
            createBind (tail, head);

          // if linking INTERFACE to NODE
        }
        else if (tail->getStructuralCategory () == Structural::Interface
                 && head->getStructuralCategory () == Structural::Node)
        {
          StructuralEntity *parentTail = tail->getStructuralParent ();
          StructuralEntity *parentHead = head->getStructuralParent ();

          if (parentTail == parentHead
              && (tail->getStructuralType () == Structural::Port
                  || tail->getStructuralType () == Structural::SwitchPort))
            createReference (tail, head);
          else if (head->getStructuralType () != Structural::Link)
            createLink (tail, head);
          else
            createBind (tail, head);

          // if linking INTERFACE to INTERFACE
        }
        else if (tail->getStructuralCategory () == Structural::Interface
                 && head->getStructuralCategory () == Structural::Interface)
        {

          StructuralEntity *parentTail = tail->getStructuralParent ();
          StructuralEntity *parentHead = head->getStructuralParent ();

          if (parentHead != NULL
              && parentTail == parentHead->getStructuralParent ())
            createReference (tail, head);
          else
            createLink (tail, head);
        }
      }
    }

    if (_tool != NULL)
    {
      _scene->removeItem (_tool);
      _scene->update ();

      delete _tool;
      _tool = NULL;
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
      performZoomIn ();
    else
      performZoomOut ();

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
      StructuralType type = StructuralUtil::translateStringToType (
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
      StructuralType type = StructuralUtil::translateStringToType (
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
      StructuralType type = StructuralUtil::translateStringToType (
          event->mimeData ()->objectName ());

      if (!list.isEmpty ())
      {
        foreach (QUrl url, list)
        {
          QString filename = url.toLocalFile ();

          QMap<QString, QString> properties;
          properties[STR_PROPERTY_ENTITY_TYPE]
              = StructuralUtil::translateTypeToString (Structural::Media);
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
  _commnads.clear ();

  emit switchedRedo (false);
  emit switchedUndo (false);
}

void
StructuralView::performDialog (StructuralLink *entity)
{
  emit requestedUpdate ();

  _dialog->setMode (
      entity->getStructuralProperty (STR_PROPERTY_REFERENCE_XCONNECTOR_ID), "",
      "", StructuralLinkDialog::EditLink);

  QMap<QString, QString> pLink;
  foreach (const QString &name, entity->getStructuralProperties ().keys ())
  {
    if (name.contains (STR_PROPERTY_LINKPARAM_NAME))
    {
      QString lpUid = name.right (name.length () - name.lastIndexOf (':') - 1);
      QString lpName = entity->getStructuralProperties ().value (name);
      QString lpValue = entity->getStructuralProperties ().value (
          QString (STR_PROPERTY_LINKPARAM_VALUE) + ":" + lpUid);

      pLink.insert (lpName, lpValue);
    }
  }

  _dialog->setConnectorParams (pLink);

  if (_dialog->exec ())
  {
    QMap<QString, QString> prev = entity->getStructuralProperties ();
    QMap<QString, QString> properties = entity->getStructuralProperties ();

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

    change (entity->getStructuralUid (), properties, prev, settings);
  }
}

void
StructuralView::performDialog (StructuralBind *entity)
{
  QMap<QString, QString> pBind;
  foreach (const QString &name, entity->getStructuralProperties ().keys ())
  {
    if (name.contains (STR_PROPERTY_BINDPARAM_NAME))
    {
      QString bpUid = name.right (name.length () - name.lastIndexOf (':') - 1);
      QString bpName = entity->getStructuralProperties ().value (name);
      QString bpValue = entity->getStructuralProperties ().value (
          QString (STR_PROPERTY_BINDPARAM_VALUE) + ":" + bpUid);

      pBind.insert (bpName, bpValue);
    }
  }

  emit requestedUpdate ();

  if (StructuralUtil::isCondition (entity->getRole ()))
  {
    _dialog->setMode (entity->getHead ()->getStructuralProperty (
                          STR_PROPERTY_REFERENCE_XCONNECTOR_ID),
                      "", "", StructuralLinkDialog::EditCondition);

    _dialog->setCondition (entity->getStructuralId ());
    _dialog->setConditionParams (pBind);

    if (entity->getTail ()->getStructuralCategory () == Structural::Interface)
    {
      foreach (
          StructuralEntity *e,
          entity->getTail ()->getStructuralParent ()->getStructuralEntities ())
      {
        _dialog->addConditionInterface (
            e->getStructuralUid (), e->getStructuralId (),
            StructuralUtil::getIcon (e->getStructuralType ()));
      }

      _dialog->setConditionInterface (
          entity->getStructuralProperty (STR_PROPERTY_REFERENCE_INTERFACE_ID));
    }
    else
    {
      foreach (StructuralEntity *e,
               entity->getTail ()->getStructuralEntities ())
      {
        _dialog->addConditionInterface (
            e->getStructuralUid (), e->getStructuralId (),
            StructuralUtil::getIcon (e->getStructuralType ()));
      }

      if (!STR_DEFAULT_WITH_INTERFACES)
      {
        if (!entity
                 ->getStructuralProperty (STR_PROPERTY_REFERENCE_INTERFACE_UID)
                 .isEmpty ()
            && _entities.contains (entity->getStructuralProperty (
                   STR_PROPERTY_REFERENCE_INTERFACE_UID)))
        {
          StructuralEntity *interface = _entities.value (
              entity->getStructuralProperty (
                  STR_PROPERTY_REFERENCE_INTERFACE_UID));

          _dialog->setConditionInterface (interface->getStructuralId ());
        }
      }
    }
  }
  else if (StructuralUtil::isAction (entity->getRole ()))
  {

    _dialog->setMode (entity->getTail ()->getStructuralProperty (
                          STR_PROPERTY_REFERENCE_XCONNECTOR_ID),
                      "", "", StructuralLinkDialog::EditAction);

    _dialog->setAction (entity->getStructuralId ());
    _dialog->setActionParams (pBind);

    if (entity->getHead ()->getStructuralCategory () == Structural::Interface)
    {
      foreach (
          StructuralEntity *e,
          entity->getHead ()->getStructuralParent ()->getStructuralEntities ())
      {
        _dialog->addActionInterface (
            e->getStructuralUid (), e->getStructuralId (),
            StructuralUtil::getIcon (e->getStructuralType ()));
      }

      _dialog->setActionInterface (
          entity->getStructuralProperty (STR_PROPERTY_REFERENCE_INTERFACE_ID));
    }
    else
    {
      foreach (StructuralEntity *e,
               entity->getHead ()->getStructuralEntities ())
      {
        _dialog->addActionInterface (
            e->getStructuralUid (), e->getStructuralId (),
            StructuralUtil::getIcon (e->getStructuralType ()));
      }

      if (!STR_DEFAULT_WITH_INTERFACES)
      {
        if (!entity
                 ->getStructuralProperty (STR_PROPERTY_REFERENCE_INTERFACE_UID)
                 .isEmpty ()
            && _entities.contains (entity->getStructuralProperty (
                   STR_PROPERTY_REFERENCE_INTERFACE_UID)))
        {
          StructuralEntity *interface = _entities.value (
              entity->getStructuralProperty (
                  STR_PROPERTY_REFERENCE_INTERFACE_UID));

          _dialog->setActionInterface (interface->getStructuralId ());
        }
      }
    }
  }

  if (_dialog->exec ())
  {
    QMap<QString, QString> prev = entity->getStructuralProperties ();
    QMap<QString, QString> properties = entity->getStructuralProperties ();

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
          = _entities.value (interface)->getStructuralId ();
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

    change (entity->getStructuralUid (), properties, prev, settings);
  }
}

StructuralEntity *
StructuralView::clone (StructuralEntity *entity, StructuralEntity *newparent)
{
  StructuralEntity *newentity = NULL;

  switch (entity->getStructuralType ())
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
      assert (newparent != NULL);
      break;
    }

    default:
    {
      break;
    }
  }

  if (newentity != NULL)
  {
    newentity->setStructuralProperties (entity->getStructuralProperties ());

    if (newparent != NULL)
      newentity->setStructuralParent (newparent);

    foreach (StructuralEntity *child, entity->getStructuralEntities ())
      newentity->addStructuralEntity (clone (child, newentity));
  }

  return newentity;
}

bool
StructuralView::isChild (StructuralEntity *entity, StructuralEntity *parent)
{
  if (entity != parent)
    foreach (StructuralEntity *e, parent->getStructuralEntities ())
      if (e->getStructuralUid () == entity->getStructuralUid ()
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
  if (entity != NULL)
  {
    QString uid = StructuralUtil::createUid ();

    QMap<QString, QString> properties = entity->getStructuralProperties ();
    properties[STR_PROPERTY_ENTITY_UID] = uid;

    if (adjust)
    {
      properties.remove (STR_PROPERTY_ENTITY_TOP);
      properties.remove (STR_PROPERTY_ENTITY_LEFT);
    }

    QMap<QString, QString> setting = StructuralUtil::createSettings (
        STR_VALUE_TRUE, STR_VALUE_TRUE, code);

    insert (uid, (parent != NULL ? parent->getStructuralUid () : ""),
            properties, setting);

    if (_entities.contains (uid))
    {
      _clipboardReferences[entity->getStructuralUid ()] = uid;

      foreach (StructuralEntity *e, entity->getStructuralEntities ())
      {
        if (e->getStructuralCategory () != Structural::Edge
            && e->getStructuralType () != Structural::Port
            && e->getStructuralType () != Structural::Link)
        {

          paste (e, _entities.value (uid), code, false);
        }
      }

      foreach (StructuralEntity *e, entity->getStructuralEntities ())
      {
        if (e->getStructuralCategory () == Structural::Edge
            || e->getStructuralType () == Structural::Port
            || e->getStructuralType () == Structural::Link)
        {

          QMap<QString, QString> p = e->getStructuralProperties ();

          QString property = STR_PROPERTY_EDGE_TAIL;

          if (p.contains (property))
            if (_clipboardReferences.contains (p.value (property)))
              e->setStructuralProperty (
                  property, _clipboardReferences.value (p.value (property)));

          property = STR_PROPERTY_EDGE_HEAD;

          if (p.contains (property))
            if (_clipboardReferences.contains (p.value (property)))
              e->setStructuralProperty (
                  property, _clipboardReferences.value (p.value (property)));

          property = STR_PROPERTY_REFERENCE_COMPONENT_UID;

          if (p.contains (property))
            if (_clipboardReferences.contains (p.value (property)))
              e->setStructuralProperty (
                  property, _clipboardReferences.value (p.value (property)));

          property = STR_PROPERTY_REFERENCE_INTERFACE_UID;

          if (p.contains (property))
            if (_clipboardReferences.contains (p.value (property)))
              e->setStructuralProperty (
                  property, _clipboardReferences.value (p.value (property)));

          property = STR_PROPERTY_REFERENCE_LINK_UID;

          if (p.contains (property))
            if (_clipboardReferences.contains (p.value (property)))
              e->setStructuralProperty (
                  property, _clipboardReferences.value (p.value (property)));

          property = STR_PROPERTY_REFERENCE_XCONNECTOR_UID;

          if (p.contains (property))
            if (_clipboardReferences.contains (p.value (property)))
              e->setStructuralProperty (
                  property, _clipboardReferences.value (p.value (property)));

          foreach (const QString &k, p.keys ())
            if (k.contains (STR_PROPERTY_LINKPARAM_NAME)
                || k.contains (STR_PROPERTY_LINKPARAM_VALUE)
                || k.contains (STR_PROPERTY_BINDPARAM_NAME)
                || k.contains (STR_PROPERTY_BINDPARAM_VALUE))
            {

              property = k.left (k.length () - k.lastIndexOf (':') - 1);

              e->setStructuralProperty (
                  property + ":" + StructuralUtil::createUid (), p.value (k));
              e->setStructuralProperty (k, "");
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
  StructuralEntity *entity = NULL;

  if (_entities.contains (_selected))
    entity = _entities.value (_selected);

  adjustLayout (entity, StructuralUtil::createUid ());
}

void
StructuralView::adjustLayout (StructuralEntity *entity, const QString &code)
{
  QVector<StructuralEntity *> children;

  if (entity != NULL)
  {
    children = entity->getStructuralEntities ();
  }
  else if (!STR_DEFAULT_WITH_BODY)
  {
    foreach (StructuralEntity *current, _entities.values ())
      if (current->getStructuralParent () == NULL)
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
  g = agopen ("g", Agdirected, NULL);

  qreal GRAPHVIZ_DPI = 72;

  QMap<QString, Agnode_t *> nodes;

  // Add nodes in graphviz graph based on children entities.
  // Skip link entities.
  foreach (StructuralEntity *e, children)
  {
    if ((e->getStructuralCategory () == Structural::Node
         || (WITH_INTERFACE_AS_NODE
             && e->getStructuralCategory () == Structural::Interface))
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
    if (e->getStructuralCategory () == Structural::Edge)
    {
      StructuralEdge *ee = (StructuralEdge *)e;

      StructuralEntity *tail = ee->getTail ();
      StructuralEntity *head = ee->getHead ();

      if (tail != NULL && head != NULL)
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
          if (head->getStructuralCategory () == Structural::Interface
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
          if (tail->getStructuralCategory () == Structural::Interface
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
            if (head->getStructuralCategory () == Structural::Interface
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
            if (tail->getStructuralCategory () == Structural::Interface
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
                        nodes.value (head->getStructuralUid ()), NULL, 1);

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

  if (entity != NULL)
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

  if (entity != NULL)
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
