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

StructuralView::StructuralView (QWidget *parent)
    : QGraphicsView (parent), _minimap (new StructuralMinimap (this)),
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
  _minimap->setMinimumSize (ST_DEFAULT_MINIMAP_W, ST_DEFAULT_MINIMAP_H);
  _minimap->setMaximumSize (ST_DEFAULT_MINIMAP_W * 2,
                            ST_DEFAULT_MINIMAP_H * 2);
  _minimap->show ();

  // Setting...
  setScene (_scene);
  centerOn (0, 0);
}

StructuralScene *
StructuralView::getScene ()
{
  return _scene;
}

StructuralEntity *
StructuralView::getBody ()
{
  StructuralEntity *body = nullptr;

  for (StructuralEntity *ent : _entities.values ())
  {
    if (ent->getType () == Structural::Body)
    {
      body = ent;
      break;
    }
  }

  return body;
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
  CPR_ASSERT (_entities.contains (uid));

  StructuralEntity *entity = _entities.value (uid);
  entity->setError (error);
  entity->adjust (true);
}

void
StructuralView::cleanErrors ()
{
  for (StructuralEntity *ent : _entities.values ())
    ent->setError ("");
}

void
StructuralView::load (const QString &data)
{
  QDomDocument *dom = new QDomDocument ();
  dom->setContent (data);

  QDomElement root = dom->firstChildElement ();
  QDomNodeList rootChildren = root.childNodes ();

  // Cheking if exist a 'body' entity when 'body' is enabled in view.
  // If don't, adds one.
  if (ST_DEFAULT_WITH_BODY)
  {
    bool hasBody = false;

    for (int i = 0; i < rootChildren.length (); i++)
    {
      if (rootChildren.item (i).isElement ())
      {
        QDomElement elt = rootChildren.item (i).toElement ();

        if (elt.nodeName () == "entity")
        {
          if (elt.attributeNode ("type").nodeValue ()
              == StructuralUtil::typeToStr (Structural::Body))
          {
            hasBody = true;
            break;
          }
        }
      }
    }

    if (!hasBody)
    {
      createEntity (Structural::Body);

      root.setAttribute ("uid", _entities.firstKey ());
    }
  }

  for (int i = 0; i < rootChildren.length (); i++)
  {
    if (rootChildren.item (i).isElement ())
    {
      QDomElement elt = rootChildren.item (i).toElement ();

      if (elt.nodeName () == "entity")
      {
        load (elt, root);
      }
      else if (elt.nodeName () == "reference")
      {
        _references[elt.attributeNode ("uid").nodeValue ()]
            = elt.attributeNode ("refer").nodeValue ();
      }
    }
  }

  for (const QString &key : _entities.keys ())
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
StructuralView::load (QDomElement ent, QDomElement parent)
{
  if (!ent.attributeNode ("uid").nodeValue ().isEmpty ())
  {
    QString entUid = ent.attributeNode ("uid").nodeValue ();

    QString parentUid;
    if (!parent.attributeNode ("uid").nodeValue ().isEmpty ())
      parentUid = parent.attributeNode ("uid").nodeValue ();
    else
      parentUid = "";

    QDomNodeList entChildren = ent.childNodes ();

    QMap<QString, QString> props;
    QDomNamedNodeMap attrs = ent.attributes ();
    for (int i = 0; i < attrs.count (); i++)
    {
      QDomAttr attr = attrs.item (i).toAttr ();
      props.insert (attr.name (), attr.value ());
    }

    QMap<QString, QString> stgs = StructuralUtil::createSettings (false, false);
    stgs[ST_SETTINGS_ADJUST_REFERENCE] = ST_VALUE_FALSE;

    insert (entUid, parentUid, props, stgs);

    for (int i = 0; i < entChildren.length (); i++)
    {
      if (entChildren.item (i).isElement ())
      {
        QDomElement child = entChildren.item (i).toElement ();

        if (child.nodeName () == "entity")
          load (child, ent);
      }
    }
  }
}

QString
StructuralView::save ()
{
  QDomDocument *doc = new QDomDocument ();

  QDomElement root = doc->createElement ("structural");

  for (const QString &key : _references.keys ())
  {
    CPR_ASSERT (_entities.contains (key)
                && _entities.contains (_references.value (key)));
    QDomElement reference = doc->createElement ("reference");
    reference.setAttribute ("uid", key);
    reference.setAttribute ("refer", _references.value (key));

    root.appendChild (reference);
  }

  if (ST_DEFAULT_WITH_BODY)
  {
    for (StructuralEntity *e : _entities.values ())
    {
      if (e->getType () == Structural::Body)
        createDocument (e, doc, root);
    }
  }
  else
  {
    for (StructuralEntity *e : _entities.values ())
    {
      if (e->getParent () == nullptr)
        createDocument (e, doc, root);
    }
  }

  doc->appendChild (root);

  return doc->toString (4);
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
StructuralView::createDocument (StructuralEntity *ent, QDomDocument *doc,
                                QDomElement parent)
{
  if (ent->getType () != Structural::Reference)
  {
    QDomElement elt = doc->createElement ("entity");
    elt.setAttribute ("uid", ent->getUid ());
    elt.setAttribute ("type", ent->getType ());

    for (const QString &key : ent->getProperties ().keys ())
      elt.setAttribute (key, ent->getProperty (key));

    for (StructuralEntity *e : ent->getChildren ())
      createDocument (e, doc, elt);

    parent.appendChild (elt);
  }
}

StructuralEntity *
StructuralView::createEntity (StructuralType type, StructuralEntity *parent)
{
  StructuralEntity *e = nullptr;

  if (parent != nullptr || !ST_DEFAULT_WITH_BODY)
  {
    switch (type)
    {
      case Structural::Media:
        e = new StructuralContent ();
        break;

      case Structural::Context:
      case Structural::Switch:
        e = new StructuralComposition ();
        e->setType (type);
        break;

      case Structural::Link:
        {
          e = new StructuralLink ();
          e->setType (type);

          connect ((StructuralLink *)e, &StructuralLink::performedEdit, this,
                   &StructuralView::performLinkDialog);

          break;
        }

      case Structural::Area:
      case Structural::Property:
      case Structural::Port:
      case Structural::SwitchPort:
        {
          e = new StructuralInterface ();
          e->setType (type);
          break;
        }

      case Structural::Reference:
      case Structural::Mapping:
        {
          e = new StructuralEdge ();
          e->setType (type);
          break;
        }

      case Structural::Bind:
        {
          e = new StructuralBind ();
          e->setType (type);

          connect ((StructuralBind *)e, &StructuralBind::performedEdit, this,
                   &StructuralView::performBindDialog);
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
            e = new StructuralComposition ();
            e->setType (type);

            e->setWidth (ST_DEFAULT_BODY_W);
            e->setHeight (ST_DEFAULT_BODY_H);

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

  return e;
}

void
StructuralView::insert (QString uid, QString parent,
                        QMap<QString, QString> props,
                        QMap<QString, QString> settings)
{
  CPR_ASSERT (!_entities.contains (uid));

  StructuralEntity *p = _entities.value (parent, nullptr);
  StructuralType type = StructuralUtil::strToType (props[ST_ATTR_ENT_TYPE]);
  StructuralEntity *e = createEntity (type, p);

//  CPR_ASSERT_NON_NULL (e);

  if (e != nullptr)
  {
    e->setUid (uid);

    if (!props.contains (ST_ATTR_ENT_ID))
      e->setId (getNewId (e));

    if (p != nullptr)
      e->setParent (p);
    else
      _scene->addItem (e);

    if (!props.contains (ST_ATTR_ENT_TOP))
    {
      if (p != nullptr)
        e->setTop (p->getHeight () / 2 - e->getHeight () / 2);
      else
      {
        auto rect = this->mapToScene (this->viewport ()->rect ().center ());
        e->setTop (rect.y () - e->getHeight () / 2);
      }
    }

    if (!props.contains (ST_ATTR_ENT_LEFT))
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
    e->setProperties (props);

    _entities[uid] = e;

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
    if (settings[ST_SETTINGS_ADJUST_REFERENCE] != ST_VALUE_FALSE)
    {
      adjustReferences (e);
    }

    // Adjust 'angles'
    if (e->getType () == Structural::Bind)
      if (!props.contains (ST_ATTR_EDGE_ANGLE))
        ((StructuralBind *)e)->setAngle (getNewAngle ((StructuralBind *)e));

    // Adjust 'others'
    if (!props.contains (ST_ATTR_ENT_TOP)
        || !props.contains (ST_ATTR_ENT_LEFT))
      e->adjust (false);
    else
      e->adjust (true);


    if (settings[ST_SETTINGS_ADJUST_REFERENCE] != ST_VALUE_FALSE)
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
              for (const QString &key : _references.keys ())
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


    connect (e, &StructuralEntity::inserted, this, &StructuralView::insert);
    connect (e, &StructuralEntity::removed, this, &StructuralView::remove);
    connect (e, &StructuralEntity::changed, this, &StructuralView::change);
    connect (e, &StructuralEntity::selected, this, &StructuralView::select);
    connect (e, &StructuralEntity::moved, this, &StructuralView::move);

    if (settings[ST_SETTINGS_UNDO] == ST_VALUE_TRUE)
    {
      Insert *cmd = new Insert (uid, parent, e->getProperties (), settings);
      cmd->setText (settings[ST_SETTINGS_CODE]);

      _commands.push (cmd);

      connect (cmd, &Insert::insert, this, &StructuralView::insert);
      connect (cmd, &Insert::remove, this, &StructuralView::remove);
      connect (cmd, &Insert::change, this, &StructuralView::change);
      connect (cmd, &Insert::select, this, &StructuralView::select);

      emit switchedUndo (true);
    }

    if (settings[ST_SETTINGS_NOTIFY] == ST_VALUE_TRUE)
    {
      emit inserted (uid, parent, e->getProperties (), settings);
    }

    if (!ST_DEFAULT_WITH_INTERFACES)
    {
      // Since interfaces are hidden, no update related with that entities
      // are notified. However, the structural view create a new id when this
      // property is empty, then the change must be explicit notified for the
      // hidden entities (interface in this case).
      emit changed (uid, e->getProperties (),
                    e->getProperties (), // this param is not used
                    // in this case
                    StructuralUtil::createSettings (false, false));
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
    for (StructuralEntity *e : _entities.values ())
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
//  CPR_ASSERT (_entities.contains (uid));

  if (_entities.contains (uid))
  {
    StructuralEntity *e = _entities.value (uid);
    StructuralEntity *p = e->getParent ();
    StructuralType type = e->getType ();

    if (settings[ST_SETTINGS_UNDO] == ST_VALUE_TRUE
        && (!e->isReference () || e->getCategory () != Structural::Interface))
    {
      QString parent = "";

      if (_entities[uid]->getParent () != nullptr)
        parent = _entities[uid]->getParent ()->getUid ();

      Remove *cmd = new Remove (_entities[uid]->getUid (), parent,
                                _entities[uid]->getProperties (), settings);
      cmd->setText (settings[ST_SETTINGS_CODE]);

      connect (cmd, &Remove::insert, this, &StructuralView::insert);
      connect (cmd, &Remove::remove, this, &StructuralView::remove);
      connect (cmd, &Remove::change, this, &StructuralView::change);
      connect (cmd, &Remove::select, this, &StructuralView::select);

      _commands.push (cmd);
      emit switchedUndo (true);
      return;
    }

    // Removing 'references'...
    if (e->getCategory () == Structural::Interface)
    {
      for (const QString &key : _references.keys (e->getUid ()))
        remove (key, StructuralUtil::createSettings (
                         ST_VALUE_FALSE, ST_VALUE_FALSE,
                         settings.value (ST_SETTINGS_CODE)));

      // Only remove interface -> interface references. Keeping
      // Media -> Media references to enable undo. All "trash" references
      // are ignore when saving the project.
      _references.remove (e->getUid ());
    }
    else if (e->getType () == Structural::Media)
    {
      for (const QString &key : _references.keys (e->getUid ()))
      {
        if (_entities.contains (key))
        {
          _entities.value (key)->setReference (false);
          _entities.value (key)->adjust (true);
        }
      }
    }

    // Removing 'children'...
    if (settings.value (ST_SETTINGS_UNDO_TRACE) != ST_VALUE_FALSE)
      settings[ST_SETTINGS_UNDO] = ST_VALUE_TRUE;

    while (!e->getChildren ().isEmpty ())
      remove (e->getChildren ().first ()->getUid (), settings);

    // Removing 'edges'...
    if (e->getCategory () != Structural::Edge)
    {
      QVector<StructuralEntity *> relatives = StructuralUtil::getNeighbors (e);
      relatives += StructuralUtil::getUpNeighbors (e);

      for (StructuralEntity *rel : relatives)
      {
        if (rel->getCategory () == Structural::Edge)
        {
          StructuralEdge *edge = (StructuralEdge *)rel;

          if (edge->getTail () == e || edge->getHead () == e)
          {
            if (edge->getType () != Structural::Reference)
            {
              remove (edge->getUid (),
                      StructuralUtil::createSettings (
                          ST_VALUE_TRUE, settings.value (ST_SETTINGS_NOTIFY),
                          settings.value (ST_SETTINGS_CODE)));
            }
            else
            {
              if (ST_DEFAULT_WITH_INTERFACES)
              {
                // In case of 'Structural::Reference' edge, just change
                // 'Structural::Port' properties.
                //
                // Note:
                // The 'tail' of a 'Structural::Reference' edge
                // is always a 'Structural::Port' entity.
                QMap<QString, QString> previous
                    = edge->getTail ()->getProperties ();
                QMap<QString, QString> props
                    = edge->getTail ()->getProperties ();

                props[ST_ATTR_REFERENCE_COMPONENT_ID] = "";
                props[ST_ATTR_REFERENCE_COMPONENT_UID] = "";
                props[ST_ATTR_REFERENCE_INTERFACE_ID] = "";
                props[ST_ATTR_REFERENCE_INTERFACE_UID] = "";

                change (edge->getTail ()->getUid (), props, previous,
                        StructuralUtil::createSettings (
                            ST_VALUE_TRUE, settings.value (ST_SETTINGS_NOTIFY),
                            settings.value (ST_SETTINGS_CODE)));
              }
            }
          }
        }
      }
    }

    // Removing 'params'...
    if (type == Structural::Link || type == Structural::Bind)
    {
      for (const QString &key : e->getProperties ().keys ())
      {
        if (key.contains (ST_ATTR_LINKPARAM_NAME)
            || key.contains (ST_ATTR_BINDPARAM_NAME))
        {
          if (settings[ST_SETTINGS_NOTIFY] == ST_VALUE_TRUE)
          {
            emit removed (
                key.right (key.length () - key.lastIndexOf (':') - 1),
                settings);
          }
        }
      }
    }

    if (!ST_DEFAULT_WITH_BODY && !ST_DEFAULT_WITH_FLOATING_INTERFACES)
    {
      if (e->getType () == Structural::Port)
      {
        StructuralEntity *target = nullptr;

        if (_entities.contains (
                e->getProperty (ST_ATTR_REFERENCE_COMPONENT_UID)))
        {
          target = _entities.value (
              e->getProperty (ST_ATTR_REFERENCE_COMPONENT_UID));

          if (_entities.contains (
                  e->getProperty (ST_ATTR_REFERENCE_INTERFACE_UID)))
          {
            target = _entities.value (
                e->getProperty (ST_ATTR_REFERENCE_INTERFACE_UID));
          }
        }

        if (target != nullptr)
          target->setProperty (ST_ATTR_ENT_AUTOSTART, ST_VALUE_FALSE);
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

    if (settings[ST_SETTINGS_NOTIFY] == ST_VALUE_TRUE)
    {
      emit removed (uid, settings);
    }
  }
}

void
StructuralView::change (QString uid, QMap<QString, QString> props,
                        QMap<QString, QString> previous,
                        QMap<QString, QString> settings)
{
  CPR_ASSERT (_entities.contains (uid));

  if (_entities.contains (uid))
  {
    if (settings[ST_SETTINGS_UNDO] == ST_VALUE_TRUE)
    {
      Change *cmd = new Change (uid, props, previous, settings);
      cmd->setText (settings[ST_SETTINGS_CODE]);

      connect (cmd, &Change::insert, this, &StructuralView::insert);
      connect (cmd, &Change::remove, this, &StructuralView::remove);
      connect (cmd, &Change::change, this, &StructuralView::change);
      connect (cmd, &Change::select, this, &StructuralView::select);

      _commands.push (cmd);
      emit switchedUndo (true);
      return;
    }

    StructuralEntity *ent = _entities[uid];

    if (ent->isUncollapsed ())
      if (props.value (ST_ATTR_ENT_UNCOLLAPSED) == ST_VALUE_FALSE)
        ((StructuralComposition *)ent)->collapse ();

    if (!ent->isUncollapsed ())
      if (props.value (ST_ATTR_ENT_UNCOLLAPSED) == ST_VALUE_TRUE)
        ((StructuralComposition *)ent)->collapse ();

    if (!ST_DEFAULT_WITH_BODY && !ST_DEFAULT_WITH_FLOATING_INTERFACES)
    {
      if (ent->getType () == Structural::Port)
      {
        StructuralEntity *lasttarget = nullptr;

        if (_entities.contains (
                previous.value (ST_ATTR_REFERENCE_COMPONENT_UID)))
        {
          lasttarget = _entities.value (
              previous.value (ST_ATTR_REFERENCE_COMPONENT_UID));

          if (_entities.contains (
                  previous.value (ST_ATTR_REFERENCE_INTERFACE_UID)))
          {
            lasttarget = _entities.value (
                previous.value (ST_ATTR_REFERENCE_INTERFACE_UID));
          }
        }

        if (lasttarget != nullptr)
          lasttarget->setProperty (ST_ATTR_ENT_AUTOSTART, ST_VALUE_FALSE);
      }
    }

    ent->setProperties (props);

    // Adjust 'references'
    adjustReferences (ent);

    // Adjust 'others'
    ent->adjust (true);

    // Notifing...
    if (settings[ST_SETTINGS_NOTIFY] == ST_VALUE_TRUE)
    {
      emit changed (uid, props, previous, settings);
    }
  }
}

void
StructuralView::adjustReferences (StructuralEntity *ent)
{
  CPR_ASSERT_NON_NULL (ent);

  StructuralType type = ent->getType ();

  switch (type)
  {
    case Structural::Media:
      {

        // Ajusting others media entities that refer
        // the current one
        bool hasChange = false;

        if (!ent->getProperty (ST_ATTR_REFERENCE_REFER_ID).isEmpty ())
        {
          if (_entities.contains (
                ent->getProperty (ST_ATTR_REFERENCE_REFER_UID)))
          {
            StructuralEntity *refer = _entities.value (
                  ent->getProperty (ST_ATTR_REFERENCE_REFER_UID));

            if (ent != refer && !refer->isReference ())
            {
              if (refer->getType () == Structural::Media)
              {
                QString instance = "new";

                if (!ent->getProperty (ST_ATTR_NODE_INSTANCE).isEmpty ())
                  instance = ent->getProperty (ST_ATTR_NODE_INSTANCE);

                for (StructuralEntity *child : ent->getChildren ())
                  if (child->isReference ())
                    if (_entities.contains (
                          child->getProperty (ST_ATTR_REFERENCE_REFER_UID)))
                      if (_entities
                          .value (child->getProperty (
                                    ST_ATTR_REFERENCE_REFER_UID))
                          ->getParent ()
                          != refer)
                        remove (child->getUid (),
                                StructuralUtil::createSettings (false, false));

                QVector<QString> entities;

                QMap<QString, QString> referToChild;
                QMap<QString, QString> childToRefer;

                for (StructuralEntity *child : ent->getChildren ())
                {
                  if (child->isReference ())
                  {
                    childToRefer[child->getUid ()]
                        = child->getProperty (ST_ATTR_REFERENCE_REFER_UID);
                    referToChild[child->getProperty (
                          ST_ATTR_REFERENCE_REFER_UID)]
                        = child->getUid ();
                  }

                  entities.append (child->getUid ());
                }

                for (StructuralEntity *child : refer->getChildren ())
                {
                  if (child->getCategory () == Structural::Interface)
                  {
                    if (!referToChild.contains (child->getUid ()))
                    {
                      if (!entities.contains (
                            _references.value (child->getUid ())))
                      {
                        QString uid = StructuralUtil::createUid ();

                        QMap<QString, QString> props = child->getProperties ();
                        props[ST_ATTR_ENT_UID] = uid;
                        props[ST_ATTR_ENT_REFERENCE] = ST_VALUE_TRUE;

                        props[ST_ATTR_REFERENCE_REFER_ID] = child->getId ();
                        props[ST_ATTR_REFERENCE_REFER_UID] = child->getUid ();

                        props.remove (ST_ATTR_ENT_TOP);
                        props.remove (ST_ATTR_ENT_LEFT);

                        childToRefer[uid] = child->getUid ();
                        childToRefer[child->getUid ()] = uid;

                        _references[uid] = child->getUid ();

                        QMap<QString, QString> settings
                            = StructuralUtil::createSettings (false, false);
                        settings[ST_SETTINGS_ADJUST_REFERENCE]
                            = ST_VALUE_FALSE;

                        insert (uid, ent->getUid (), props, settings);
                      }
                    }
                  }
                }

                if (instance == "new")
                {
                  for (StructuralEntity *child : ent->getChildren ())
                    if (!child->isReference ())
                    {
                      for (const QString &key :
                           _references.keys (child->getUid ()))
                        remove (key,
                                StructuralUtil::createSettings (false, false));
                    }
                }
                else if (instance == "instSame" || instance == "gradSame")
                {
                  for (StructuralEntity *child : ent->getChildren ())
                    if (!child->isReference ())
                      for (const QString &key :
                           _references.keys (child->getUid ()))
                        if (_entities.contains (key))
                          if (_entities.value (key)->getParent () != refer)
                            remove (key, StructuralUtil::createSettings (
                                      false, false));

                  bool hasNewEntity = false;

                  for (StructuralEntity *child : ent->getChildren ())
                  {
                    if (child->getCategory () == Structural::Interface)
                    {
                      if (!childToRefer.contains (child->getUid ()))
                      {
                        if (!_references.values ().contains (child->getUid ()))
                        {
                          QString uid = StructuralUtil::createUid ();

                          QMap<QString, QString> props
                              = child->getProperties ();
                          props[ST_ATTR_ENT_UID] = uid;
                          props[ST_ATTR_ENT_REFERENCE] = ST_VALUE_TRUE;

                          props.remove (ST_ATTR_ENT_TOP);
                          props.remove (ST_ATTR_ENT_LEFT);

                          props[ST_ATTR_REFERENCE_REFER_ID] = child->getId ();
                          props[ST_ATTR_REFERENCE_REFER_UID]
                              = child->getUid ();

                          _references[uid] = child->getUid ();

                          QMap<QString, QString> settings
                              = StructuralUtil::createSettings (false, false);
                          settings[ST_SETTINGS_ADJUST_REFERENCE]
                              = ST_VALUE_FALSE;

                          insert (uid, refer->getUid (), props, settings);

                          hasNewEntity = true;
                        }
                      }
                    }
                  }

                  if (hasNewEntity)
                    for (const QString &key :
                         _references.keys (refer->getUid ()))
                      if (_entities.contains (key))
                        if (key != ent->getUid ())
                          adjustReferences (_entities.value (key));
                }

                _references[ent->getUid ()] = refer->getUid ();
                ent->setReference (true);
                hasChange = true;
              }
            }
          }
        }

        if (_references.contains (ent->getUid ()))
        {
          if (!hasChange)
          {
            for (StructuralEntity *child : ent->getChildren ())
            {
              if (child->isReference ())
              {
                remove (child->getUid (),
                        StructuralUtil::createSettings (false, false));
              }
              else
              {
                for (const QString &key : _references.keys (child->getUid ()))
                  remove (key, StructuralUtil::createSettings (false, false));
              }
            }

            ent->setReference (false);
          }
        }

        // Adjusting values of relatives entities
        QVector<StructuralEntity *> relatives
            = StructuralUtil::getNeighbors (ent);
        relatives += StructuralUtil::getUpNeighbors (ent);

        for (StructuralEntity *rel : relatives)
        {
          // Adjusting relative edges
          if (rel->getCategory () == Structural::Edge)
          {
            StructuralEdge *edge = (StructuralEdge *)rel;

            // Adjusting edge if it already has
            // an tail and head defined.
            if (edge->getTail () == ent || edge->getHead () == ent)
            {
              adjustReferences (edge);
            }

            // Adjusting edge if it is a bind and a component value
            // was defined
            else if (edge->getType () == Structural::Bind)
            {
              StructuralEdge *bind = (StructuralEdge *)edge;

              if (!bind->getProperty (ST_ATTR_REFERENCE_COMPONENT_ID)
                  .isEmpty ()
                  && bind->getProperty (ST_ATTR_REFERENCE_INTERFACE_ID)
                  .isEmpty ())
              {
                if (ent->getId ()
                    == bind->getProperty (ST_ATTR_REFERENCE_COMPONENT_ID))
                {
                  if (bind->getHead () != nullptr
                      && bind->getHead ()->getType () == Structural::Link)
                  {
                    bind->setTail (ent);

                    bind->setProperty (ST_ATTR_EDGE_TAIL, ent->getUid ());

                    bind->setProperty (ST_ATTR_REFERENCE_COMPONENT_UID,
                                       ent->getUid ());
                  }
                  else if (bind->getTail () != nullptr
                           && bind->getTail ()->getType () == Structural::Link)
                  {
                    bind->setHead (ent);

                    bind->setProperty (ST_ATTR_EDGE_HEAD, ent->getUid ());

                    bind->setProperty (ST_ATTR_REFERENCE_COMPONENT_UID,
                                       ent->getUid ());
                  }

                  adjustReferences (bind);
                }
              }
            }
          }

          // Adjusting relative ports
          else if (rel->getCategory () == Structural::Interface)
          {
            StructuralInterface *interface = (StructuralInterface *)rel;

            if (!ST_DEFAULT_WITH_INTERFACES)
            {
              if (interface->getType () == Structural::Port)
              {
                if (interface->getProperty (ST_ATTR_REFERENCE_COMPONENT_ID)
                    == ent->getId ())
                {
                  ent->setProperty (ST_ATTR_ENT_AUTOSTART, ST_VALUE_TRUE);
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
        if (_entities.contains (ent->getProperty (ST_ATTR_EDGE_TAIL)))
          ((StructuralEdge *)ent)
            ->setTail (
              _entities.value (ent->getProperty (ST_ATTR_EDGE_TAIL)));

        if (_entities.contains (ent->getProperty (ST_ATTR_EDGE_HEAD)))
          ((StructuralEdge *)ent)
            ->setHead (
              _entities.value (ent->getProperty (ST_ATTR_EDGE_HEAD)));

        bool isVisible = true;

        StructuralEntity *tail = ((StructuralEdge *)ent)->getTail ();
        StructuralEntity *head = ((StructuralEdge *)ent)->getHead ();

        if (tail != nullptr && head != nullptr)
        {
          if (!ST_DEFAULT_WITH_INTERFACES)
          {
            if (tail->getCategory () == Structural::Interface)
            {
              ((StructuralEdge *)ent)->setTail (tail->getParent ());
            }

            if (head->getCategory () == Structural::Interface)
            {
              ((StructuralEdge *)ent)->setHead (head->getParent ());
            }
          }

          if (ent->getParent () != nullptr)
            if (!ent->getParent ()->isUncollapsed ())
              isVisible = false;

          StructuralEntity *component = nullptr;
          StructuralEntity *interface = nullptr;

          if (!ent->getProperty (ST_ATTR_REFERENCE_INTERFACE_ID).isEmpty ())
          {
            if (head->getId ()
                != ent->getProperty (ST_ATTR_REFERENCE_INTERFACE_ID)
                && tail->getId ()
                != ent->getProperty (ST_ATTR_REFERENCE_INTERFACE_ID))
              isVisible = false;

            if (_entities.contains (
                  ent->getProperty (ST_ATTR_REFERENCE_INTERFACE_UID)))
              interface = _entities.value (
                    ent->getProperty (ST_ATTR_REFERENCE_INTERFACE_UID));
            else
              isVisible = false;
          }

          if (!ent->getProperty (ST_ATTR_REFERENCE_COMPONENT_ID).isEmpty ())
          {
            if (ent->getProperty (ST_ATTR_REFERENCE_INTERFACE_ID).isEmpty ())
              if (head->getId ()
                  != ent->getProperty (ST_ATTR_REFERENCE_COMPONENT_ID)
                  && tail->getId ()
                  != ent->getProperty (ST_ATTR_REFERENCE_COMPONENT_ID))
                isVisible = false;

            if (_entities.contains (
                  ent->getProperty (ST_ATTR_REFERENCE_COMPONENT_UID)))
              component = _entities.value (
                    ent->getProperty (ST_ATTR_REFERENCE_COMPONENT_UID));
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

            if (ent->getParent () != component->getParent ())
              isVisible = false;
          }

          //          if (!ST_DEFAULT_WITH_BODY &&
          //              !ST_DEFAULT_WITH_FLOATING_INTERFACES)
          //          {
          //            if (tail->isHidden())
          //              isVisible = false;
          //          }
        }
        else
        {
          isVisible = false;
        }

        ent->setHidden (!isVisible);

        break;
      }

    case Structural::Area:
    case Structural::Property:
    case Structural::Port:
    case Structural::Switch:
      {
        QVector<StructuralEntity *> relatives
            = StructuralUtil::getNeighbors (ent);
        relatives += StructuralUtil::getUpNeighbors (ent);

        for (StructuralEntity *rel : relatives)
        {
          if (rel->getCategory () == Structural::Edge)
          {
            StructuralEdge *edge = (StructuralEdge *)rel;

            if (edge->getTail () == ent || edge->getHead () == ent)
            {
              adjustReferences (edge);
            }
            else if (edge->getType () == Structural::Bind)
            {
              StructuralEdge *bind = (StructuralEdge *)edge;

              if (!bind->getProperty (ST_ATTR_REFERENCE_INTERFACE_ID)
                  .isEmpty ())
              {
                if (ent->getId ()
                    == bind->getProperty (ST_ATTR_REFERENCE_INTERFACE_ID))
                {
                  if (bind->getHead () != nullptr
                      && bind->getHead ()->getType () == Structural::Link)
                  {
                    bind->setTail (ent);

                    bind->setProperty (ST_ATTR_EDGE_TAIL, ent->getUid ());
                  }
                  else if (bind->getTail () != nullptr
                           && bind->getTail ()->getType () == Structural::Link)
                  {
                    bind->setHead (ent);

                    bind->setProperty (ST_ATTR_EDGE_HEAD, ent->getUid ());
                  }

                  bind->setProperty (ST_ATTR_REFERENCE_INTERFACE_UID,
                                     ent->getUid ());

                  bind->setProperty (ST_ATTR_REFERENCE_COMPONENT_ID,
                                     ent->getParent ()->getId ());

                  bind->setProperty (ST_ATTR_REFERENCE_COMPONENT_UID,
                                     ent->getParent ()->getUid ());

                  adjustReferences (bind);
                }
              }
            }
          }
        }

        if (type == Structural::Port)
        {
          if (ST_DEFAULT_WITH_INTERFACES)
          {
            for (StructuralEntity *e : _entities.values ())
              if (e->getType () == Structural::Reference)
                if (e->getProperty (ST_ATTR_EDGE_TAIL) == ent->getUid ())
                  remove (e->getUid (),
                          StructuralUtil::createSettings (false, false));

            StructuralEntity *component = nullptr;
            StructuralEntity *interface = nullptr;

            if (_entities.contains (
                  ent->getProperty (ST_ATTR_REFERENCE_INTERFACE_UID)))
              interface = _entities.value (
                    ent->getProperty (ST_ATTR_REFERENCE_INTERFACE_UID));

            if (_entities.contains (
                  ent->getProperty (ST_ATTR_REFERENCE_COMPONENT_UID)))
              component = _entities.value (
                    ent->getProperty (ST_ATTR_REFERENCE_COMPONENT_UID));

            StructuralEntity *head = nullptr;

            if (component != nullptr)
            {
              head = component;

              if (interface != nullptr)
                if (interface->getParent () == component)
                  head = interface;

              if (head != nullptr)
              {
                QMap<QString, QString> props;
                props[ST_ATTR_ENT_TYPE]
                    = StructuralUtil::typeToStr (Structural::Reference);
                props[ST_ATTR_EDGE_TAIL] = ent->getUid ();
                props[ST_ATTR_EDGE_HEAD] = head->getUid ();

                props[ST_ATTR_REFERENCE_COMPONENT_ID]
                    = ent->getProperty (ST_ATTR_REFERENCE_COMPONENT_ID);
                props[ST_ATTR_REFERENCE_COMPONENT_UID]
                    = ent->getProperty (ST_ATTR_REFERENCE_COMPONENT_UID);
                props[ST_ATTR_REFERENCE_INTERFACE_ID]
                    = ent->getProperty (ST_ATTR_REFERENCE_INTERFACE_ID);
                props[ST_ATTR_REFERENCE_INTERFACE_UID]
                    = ent->getProperty (ST_ATTR_REFERENCE_INTERFACE_UID);

                QString parentUid;

                if (ent->getParent () != nullptr)
                  parentUid = ent->getParent ()->getUid ();
                else
                  parentUid = "";

                if (!ST_DEFAULT_WITH_BODY
                    && !ST_DEFAULT_WITH_FLOATING_INTERFACES)
                {
                  if (ent->getParent () == nullptr)
                  {
                    ent->setHidden (true);

                    head->setProperty (ST_ATTR_ENT_AUTOSTART, ST_VALUE_TRUE);

                    props[ST_ATTR_ENT_HIDDEN] = ST_VALUE_TRUE;
                  }
                }

                insert (StructuralUtil::createUid (), parentUid, props,
                        StructuralUtil::createSettings (false, false));
              }
            }
          }
          else if (!ST_DEFAULT_WITH_FLOATING_INTERFACES)
          {
            StructuralEntity *component = nullptr;
            StructuralEntity *interface = nullptr;

            if (_entities.contains (
                  ent->getProperty (ST_ATTR_REFERENCE_COMPONENT_UID)))
            {
              component = _entities[ent->getProperty (
                    ST_ATTR_REFERENCE_COMPONENT_UID)];
            }

            if (_entities.contains (
                  ent->getProperty (ST_ATTR_REFERENCE_INTERFACE_UID)))
            {
              interface = _entities[ent->getProperty (
                    ST_ATTR_REFERENCE_INTERFACE_UID)];
            }

            QMap<QString, QString> props;

            QMap<QString, QString> settings
                = StructuralUtil::createSettings (true, true);

            if (interface != nullptr)
            {
              if (component != nullptr && interface->getParent () == component)
              {
                props = interface->getProperties ();
                props[ST_ATTR_ENT_AUTOSTART] = ST_VALUE_TRUE;

                change (interface->getUid (), props, ent->getProperties (),
                        settings);
              }
            }
            else if (component != nullptr)
            {
              props = component->getProperties ();
              props[ST_ATTR_ENT_AUTOSTART] = ST_VALUE_TRUE;

              change (component->getUid (), props, ent->getProperties (),
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

void
StructuralView::select (QString uid, QMap<QString, QString> settings)
{
  CPR_ASSERT (_entities.contains (uid) || uid == "");

  if (_entities.contains (uid))
  {
    if (uid != _selected)
    {
      StructuralEntity *ent = _entities.value (_selected);

      if (ent != nullptr)
        ent->setSelected (false);

      //
      // Selecting...
      //
      ent = _entities.value (uid);
      ent->setSelected (true);

      _selected = ent->getUid ();

      //
      // Notifing...
      //
      bool enablePaste = false;

      if (_clipboard != nullptr)
        if (ent->getUid () != _clipboard->getUid ())
          if (StructuralUtil::validateKinship (_clipboard->getType (),
                                               ent->getType ()))
            if (!isChild (ent, _clipboard))
              enablePaste = true;

      emit switchedCut (true);
      emit switchedCopy (true);
      emit switchedPaste (enablePaste);

      if (settings[ST_SETTINGS_NOTIFY] == ST_VALUE_TRUE)
      {
        emit selected (uid, settings);
      }
    }
  }
  else
  {
    StructuralEntity *ent = _entities.value (_selected);

    if (ent != nullptr)
      ent->setSelected (false);

    _selected = ""; // unselect

    emit switchedCut (false);
    emit switchedCopy (false);

    bool enablePaste = false;

    if (!ST_DEFAULT_WITH_BODY)
      if (_clipboard != nullptr)
        if (StructuralUtil::validateKinship (_clipboard->getType (),
                                             Structural::Body))
          enablePaste = true;

    emit switchedPaste (enablePaste);

    if (settings[ST_SETTINGS_NOTIFY] == ST_VALUE_TRUE)
    {
      emit selected (uid, settings);
    }
  }

  _scene->update ();
}

void
StructuralView::move (QString uid, QString parent,
                      QMap<QString, QString> props,
                      QMap<QString, QString> settings)
{
  CPR_ASSERT (_entities.contains (uid));

  StructuralEntity *e = _entities.value (uid);
  StructuralEntity *p = _entities.value (parent);

  StructuralType type = Structural::Body;

  if (p != nullptr)
    type = p->getType ();

  CPR_ASSERT (StructuralUtil::validateKinship (e->getType (), type));
  if (e->getParent () != p)
  {
    for (StructuralEntity *ent : _entities.values ())
      ent->setDraggable (false);

    select ("", settings);

    StructuralEntity *copy = clone (e, nullptr);

    for (const QString &name : props.keys ())
      copy->setProperty (name, props.value (name));

    copy->adjust ();

    if (props.contains (ST_ATTR_ENT_TOP))
      copy->setTop (copy->getTop () - copy->getHeight () / 2);

    if (props.contains (ST_ATTR_ENT_LEFT))
      copy->setLeft (copy->getLeft () - copy->getWidth () / 2);

    paste (copy, p, settings.value (ST_SETTINGS_CODE), false);
    remove (uid, settings);
  }
}

void
StructuralView::createEntity (StructuralType type,
                              QMap<QString, QString> props,
                              QMap<QString, QString> stgs)
{
  props.insert (ST_ATTR_ENT_TYPE, StructuralUtil::typeToStr (type));

  if (stgs.empty())
    stgs = StructuralUtil::createSettings();

  QString uid = StructuralUtil::createUid ();
  QString parent = "";

  if (_entities.contains (_selected))
    parent = _selected;

  if (!props.contains (ST_ATTR_ENT_TYPE))
    props[ST_ATTR_ENT_TYPE] = StructuralUtil::typeToStr (type);

  if (!stgs.contains (ST_SETTINGS_UNDO))
    stgs[ST_SETTINGS_UNDO] = ST_VALUE_TRUE;

  if (!stgs.contains (ST_SETTINGS_NOTIFY))
    stgs[ST_SETTINGS_NOTIFY] = ST_VALUE_TRUE;

  if (!stgs.contains (ST_SETTINGS_CODE))
    stgs[ST_SETTINGS_CODE] = StructuralUtil::createUid ();

  insert (uid, parent, props, stgs);
}

void
StructuralView::performAutostart ()
{
  CPR_ASSERT_X (!ST_DEFAULT_WITH_BODY && !ST_DEFAULT_WITH_FLOATING_INTERFACES,
                "performAutostart",
                "Autostart is only available when when body and floating"
                "interfaces are disabled in build");

  CPR_ASSERT (_entities.contains (_selected));

  StructuralEntity *e = _entities.value (_selected);
  QMap<QString, QString> props = e->getProperties ();

  QMap<QString, QString> settings
      = StructuralUtil::createSettings (true, true);

  // Case 1: media already has an autostart property
  if (props.contains (ST_ATTR_ENT_AUTOSTART)
      && props[ST_ATTR_ENT_AUTOSTART] == ST_VALUE_TRUE)
  {
    QVector<StructuralEntity *> neighbors = StructuralUtil::getNeighbors (e);

    if (ST_DEFAULT_WITH_INTERFACES)
    {
      if (e->getCategory () == Structural::Interface)
        neighbors += StructuralUtil::getUpNeighbors (e);
    }

    // If ST_DEFAULT_WITH_INTERFACES is disabled, interface entities are
    // just hidden. So, it is OK to iterate through interfaces, since they
    // exist even with ST_DEFAULT_WITH_INTERFACES disabled.
    for (StructuralEntity *neighbor : neighbors)
    {
      if (neighbor->getCategory () == Structural::Interface)
      {
        if (neighbor->getProperty (ST_ATTR_REFERENCE_COMPONENT_UID)
                == e->getUid ()
            || (ST_DEFAULT_WITH_INTERFACES
                && neighbor->getProperty (ST_ATTR_REFERENCE_INTERFACE_UID)
                       == e->getUid ()))
        {
          remove (neighbor->getUid (), settings);
          break;
        }
      }
    }

    props[ST_ATTR_ENT_AUTOSTART] = ST_VALUE_FALSE;
  }
  // Case 2: media does not has autostart property
  else
  {
    QMap<QString, QString> pProps;
    pProps[ST_ATTR_ENT_TYPE] = StructuralUtil::typeToStr (Structural::Port);

    QString pParent = "";

    if (e->getCategory () == Structural::Interface)
    {
      if (e->getParent () != nullptr)
      {
        pProps.insert (ST_ATTR_REFERENCE_COMPONENT_UID,
                       e->getParent ()->getUid ());
        pProps.insert (ST_ATTR_REFERENCE_COMPONENT_ID,
                       e->getParent ()->getId ());

        pProps.insert (ST_ATTR_REFERENCE_INTERFACE_UID, e->getUid ());
        pProps.insert (ST_ATTR_REFERENCE_INTERFACE_ID, e->getId ());

        pParent = e->getParent ()->getParent () != nullptr
                      ? e->getParent ()->getParent ()->getUid ()
                      : "";
      }
    }
    else
    {
      pProps.insert (ST_ATTR_REFERENCE_COMPONENT_UID, e->getUid ());
      pProps.insert (ST_ATTR_REFERENCE_COMPONENT_ID, e->getId ());

      pParent = e->getParent () != nullptr ? e->getParent ()->getUid () : "";
    }

    insert (StructuralUtil::createUid (), pParent, pProps, settings);

    props[ST_ATTR_ENT_AUTOSTART] = ST_VALUE_TRUE;
  }

  change (e->getUid (), props, e->getProperties (),
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

  for (const QString &key : _references.keys ())
  {
    if (_entities.contains (key))
      if (_entities.value (key)->getType () == Structural::Media)
        adjustReferences (_entities.value (key));
  }

  for (const QString &key : _entities.keys ())
  {
    if (_entities.contains (key))
    {
      StructuralEntity *e = _entities.value (key);

      if (e->getCategory () == Structural::Edge)
      {
        StructuralEntity *c = _entities.value (
            e->getProperty (ST_ATTR_REFERENCE_COMPONENT_UID));
        StructuralEntity *i = _entities.value (
            e->getProperty (ST_ATTR_REFERENCE_INTERFACE_UID));

        if (c != nullptr)
        {
          if (i == nullptr)
          {
            for (StructuralEntity *r : c->getChildren ())
            {
              if (r->getId ()
                  == e->getProperty (ST_ATTR_REFERENCE_INTERFACE_ID))
              {
                e->setProperty (ST_ATTR_REFERENCE_INTERFACE_UID, r->getUid ());

                if (((StructuralEdge *)e)->getTail () != nullptr)
                  e->setProperty (ST_ATTR_EDGE_HEAD, r->getUid ());
                else
                  e->setProperty (ST_ATTR_EDGE_TAIL, r->getUid ());

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

    StructuralEntity *ent = _entities.value (_selected);

    if (ent != nullptr && ent->getCategory () != Structural::Edge
        && ent->getType () != Structural::Body)
    {
      _clipboard = clone (ent);
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

    if (!ST_DEFAULT_WITH_BODY)
    {
      if (StructuralUtil::validateKinship (_clipboard->getType (),
                                           Structural::Body))
        isAValidPaste = true;
    }
    else
    {
      if (parent != nullptr)
        if (_clipboard->getUid () != parent->getUid ())
          if (StructuralUtil::validateKinship (_clipboard->getType (),
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

        for (StructuralEntity *entity : _entities.values ())
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

        QMap<QString, QString> props = _clipboard->getProperties ();
        props[ST_ATTR_ENT_UID] = uid;

        QString refID = _clipboard->getId ();
        refID = refID.section ('_', 1, 1);

        int n = 0;

        for (StructuralEntity *entity : _entities.values ())
          if (entity->getId ().startsWith ("r_" + refID))
            ++n;

        if (n > 0)
          refID = "r_" + refID + "_" + QString::number (n);
        else
          refID = "r_" + refID;

        props[ST_ATTR_ENT_ID] = refID;

        props[ST_ATTR_REFERENCE_REFER_ID]
            = _clipboard->getId ().section ('_', 1, 1);
        props[ST_ATTR_REFERENCE_REFER_UID] = _clipboard->getUid ();

        props.remove (ST_ATTR_ENT_TOP);
        props.remove (ST_ATTR_ENT_LEFT);

        insert (uid, (parent != nullptr ? parent->getUid () : ""), props,
                StructuralUtil::createSettings ());
      }

      // Fix entity drag status
      for (StructuralEntity *e : _entities.values ())
        e->setDraggable (false);
    }
  }
}

void
StructuralView::createLink (StructuralEntity *tail, StructuralEntity *head)
{
  StructuralEntity *parentTail = (StructuralEntity *)tail->getParent ();
  StructuralEntity *parentHead = (StructuralEntity *)head->getParent ();

  if ((parentTail != nullptr && parentHead != nullptr)
      || !ST_DEFAULT_WITH_BODY)
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
    else if (parentTail != nullptr && parentTail->getParent () == parentHead
             && tail->getCategory () == Structural::Interface)
    {
      phead = QPointF (head->getLeft (), head->getTop ());
      ptail = parentTail->mapToParent (tail->getLeft (), tail->getTop ());

      parent = parentHead;
    }
    else if (parentHead != nullptr && parentTail == parentHead->getParent ()
             && head->getCategory () == Structural::Interface)
    {
      ptail = QPointF (tail->getLeft (), tail->getTop ());
      phead = parentHead->mapToParent (head->getLeft (), head->getTop ());

      parent = parentTail;
    }
    else if (parentTail != nullptr && parentHead != nullptr
             && parentTail->getParent () == parentHead->getParent ()
             && tail->getCategory () == Structural::Interface
             && head->getCategory () == Structural::Interface)
    {
      ptail = parentTail->mapToParent (tail->getLeft (), tail->getTop ());
      phead = parentHead->mapToParent (head->getLeft (), head->getTop ());

      parent = parentTail->getParent ();
    }

    setMode (Structural::Pointing);
    emit switchedPointer (true);

    if (parent != nullptr || !ST_DEFAULT_WITH_BODY)
    {
      emit updateRequested ();

      if (tail->getCategory () == Structural::Interface)
      {
        for (StructuralEntity *ent : parentTail->getChildren ())
        {
          _dialog->addConditionInterface (
              ent->getUid (), ent->getId (),
              StructuralUtil::getIcon (ent->getType ()));
        }

        _dialog->setConditionInterface (tail->getId ());
      }
      else
      {
        for (StructuralEntity *ent : tail->getChildren ())
        {
          _dialog->addConditionInterface (
              ent->getUid (), ent->getId (),
              StructuralUtil::getIcon (ent->getType ()));
        }
      }

      if (head->getCategory () == Structural::Interface)
      {
        for (StructuralEntity *ent : parentHead->getChildren ())
        {
          _dialog->addActionInterface (
              ent->getUid (), ent->getId (),
              StructuralUtil::getIcon (ent->getType ()));
        }

        _dialog->setActionInterface (head->getId ());
      }
      else
      {
        for (StructuralEntity *ent : head->getChildren ())
        {
          _dialog->addActionInterface (
              ent->getUid (), ent->getId (),
              StructuralUtil::getIcon (ent->getType ()));
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

        QMap<QString, QString> props;
        props[ST_ATTR_ENT_TYPE] = StructuralUtil::typeToStr (Structural::Link);
        props[ST_ATTR_REFERENCE_XCONNECTOR_ID] = _dialog->getConnector ();

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

        props[ST_ATTR_ENT_TOP] = QString::number (y);
        props[ST_ATTR_ENT_LEFT] = QString::number (x);

        QMap<QString, QString> params = _dialog->getConnectorParams ();

        for (const QString &key : params.keys ())
        {
          if (!params.value (key).isEmpty ())
          {
            QString uid = StructuralUtil::createUid ();

            props[QString (ST_ATTR_LINKPARAM_NAME) + ":" + uid] = key;
            props[QString (ST_ATTR_LINKPARAM_VALUE) + ":" + uid]
                = params.value (key);
          }
        }

        QMap<QString, QString> settings
            = StructuralUtil::createSettings (true, true);

        insert (uid, (parent != nullptr ? parent->getUid () : ""), props,
                settings);

        if (_entities.contains (uid))
        {
          createBind (tail, _entities.value (uid), _dialog->getCondition (),
                      settings.value (ST_SETTINGS_CODE));
          createBind (_entities.value (uid), head, _dialog->getAction (),
                      settings.value (ST_SETTINGS_CODE));
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

  if ((parentTail != nullptr && parentHead != nullptr)
      || !ST_DEFAULT_WITH_BODY)
  {
    StructuralEntity *parent = nullptr;

    if (parentTail == parentHead)
      parent = parentTail;

    else if (parentTail != nullptr && parentTail->getParent () == parentHead
             && tail->getCategory () == Structural::Interface)
      parent = parentHead;

    else if (parentHead != nullptr && parentTail == parentHead->getParent ()
             && head->getCategory () == Structural::Interface)
      parent = parentTail;

    if (parent != nullptr || !ST_DEFAULT_WITH_BODY)
    {
      QString uid = StructuralUtil::createUid ();

      QMap<QString, QString> props;
      props[ST_ATTR_ENT_TYPE] = StructuralUtil::typeToStr (Structural::Bind);
      props[ST_ATTR_EDGE_TAIL] = tail->getUid ();
      props[ST_ATTR_EDGE_HEAD] = head->getUid ();

      props[ST_ATTR_BIND_ROLE] = role;
      props[ST_ATTR_ENT_ID] = role;

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
          emit updateRequested ();

          setMode (Structural::Pointing);
          emit switchedPointer (true);

          if (entityNonLink->getCategory () == Structural::Interface)
          {
            for (StructuralEntity *ent : parentNonLink->getChildren ())
            {
              _dialog->addActionInterface (
                  ent->getUid (), ent->getId (),
                  StructuralUtil::getIcon (ent->getType ()));
            }

            _dialog->setActionInterface (entityNonLink->getId ());
          }
          else
          {
            for (StructuralEntity *ent : entityNonLink->getChildren ())
            {
              _dialog->addActionInterface (
                  ent->getUid (), ent->getId (),
                  StructuralUtil::getIcon (ent->getType ()));
            }
          }

          _dialog->setMode (
              entityLink->getProperty (ST_ATTR_REFERENCE_XCONNECTOR_ID), "",
              "", StructuralLinkDialog::CreateAction);

          if (_dialog->exec ())
          {
            if (!_dialog->getActionInterface ().isEmpty ())
            {
              QString uid = _dialog->getActionInterface ();

              if (_entities.contains (uid))
              {
                entityNonLink = _entities.value (uid);
                parentNonLink = entityNonLink->getParent ();

                props[ST_ATTR_EDGE_HEAD] = entityNonLink->getUid ();
              }
            }

            props[ST_ATTR_BIND_ROLE] = _dialog->getAction ();
            props[ST_ATTR_ENT_ID] = _dialog->getAction ();
          }
          else
          {
            return;
          }
        }

        QMap<QString, QString> params = _dialog->getActionParams ();
        for (const QString &key : params.keys ())
        {
          if (!params.value (key).isEmpty ())
          {
            QString uid = StructuralUtil::createUid ();

            props.insert (QString (ST_ATTR_BINDPARAM_NAME) + ":" + uid, key);
            props.insert (QString (ST_ATTR_BINDPARAM_VALUE) + ":" + uid,
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
          emit updateRequested ();

          setMode (Structural::Pointing);
          emit switchedPointer (true);

          if (entityNonLink->getCategory () == Structural::Interface)
          {
            for (StructuralEntity *ent : parentNonLink->getChildren ())
            {
              _dialog->addConditionInterface (
                  ent->getUid (), ent->getId (),
                  StructuralUtil::getIcon (ent->getType ()));
            }

            _dialog->setConditionInterface (entityNonLink->getId ());
          }
          else
          {
            for (StructuralEntity *ent : entityNonLink->getChildren ())
            {
              _dialog->addConditionInterface (
                  ent->getUid (), ent->getId (),
                  StructuralUtil::getIcon (ent->getType ()));
            }
          }

          _dialog->setMode (
              entityLink->getProperty (ST_ATTR_REFERENCE_XCONNECTOR_ID), "",
              "", StructuralLinkDialog::CreateCondition);

          if (_dialog->exec ())
          {
            if (!_dialog->getConditionInterface ().isEmpty ())
            {
              QString uid = _dialog->getConditionInterface ();

              if (_entities.contains (uid))
              {
                entityNonLink = _entities.value (uid);
                parentNonLink = entityNonLink->getParent ();

                props[ST_ATTR_EDGE_TAIL] = entityNonLink->getUid ();
              }
            }

            props[ST_ATTR_BIND_ROLE] = _dialog->getCondition ();
            props[ST_ATTR_ENT_ID] = _dialog->getCondition ();
          }
          else
          {
            return;
          }
        }

        QMap<QString, QString> params = _dialog->getConditionParams ();

        for (const QString &key : params.keys ())
        {
          if (!params.value (key).isEmpty ())
          {
            QString uid = StructuralUtil::createUid ();

            props.insert (QString (ST_ATTR_BINDPARAM_NAME) + ":" + uid, key);
            props.insert (QString (ST_ATTR_BINDPARAM_VALUE) + ":" + uid,
                          params.value (key));
          }
        }
      }

      if (entityLink != nullptr && entityNonLink != nullptr)
      {
        props[ST_ATTR_REFERENCE_LINK_UID] = entityLink->getUid ();

        if (entityNonLink->getCategory () == Structural::Interface)
        {
          props[ST_ATTR_REFERENCE_INTERFACE_ID] = entityNonLink->getId ();
          props[ST_ATTR_REFERENCE_INTERFACE_UID] = entityNonLink->getUid ();
          props[ST_ATTR_REFERENCE_COMPONENT_ID] = parentNonLink->getId ();
          props[ST_ATTR_REFERENCE_COMPONENT_UID] = parentNonLink->getUid ();
        }
        else
        {
          props[ST_ATTR_REFERENCE_COMPONENT_ID] = entityNonLink->getId ();
          props[ST_ATTR_REFERENCE_COMPONENT_UID] = entityNonLink->getUid ();
          props[ST_ATTR_REFERENCE_INTERFACE_ID] = "";
          props[ST_ATTR_REFERENCE_INTERFACE_UID] = "";
        }
      }

      QMap<QString, QString> settings
          = StructuralUtil::createSettings (true, true);

      if (!code.isEmpty ())
        settings[ST_SETTINGS_CODE] = code;

      insert (uid, (parent != nullptr ? parent->getUid () : ""), props,
              settings);
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
        || (parentHead != nullptr && parentTail == parentHead->getParent ()))
    {
      QMap<QString, QString> previous = tail->getProperties ();
      QMap<QString, QString> props;

      if (tail->getType () == Structural::Port)
        props = tail->getProperties ();

      if (head->getCategory () == Structural::Interface)
      {
        props[ST_ATTR_REFERENCE_INTERFACE_ID] = head->getId ();
        props[ST_ATTR_REFERENCE_INTERFACE_UID] = head->getUid ();
        props[ST_ATTR_REFERENCE_COMPONENT_ID] = parentHead->getId ();
        props[ST_ATTR_REFERENCE_COMPONENT_UID] = parentHead->getUid ();
      }
      else
      {
        props[ST_ATTR_REFERENCE_COMPONENT_ID] = head->getId ();
        props[ST_ATTR_REFERENCE_COMPONENT_UID] = head->getUid ();
        props[ST_ATTR_REFERENCE_INTERFACE_ID] = "";
        props[ST_ATTR_REFERENCE_INTERFACE_UID] = "";
      }

      setMode (Structural::Pointing);
      emit switchedPointer (true);

      if (tail->getType () == Structural::Port)
      {
        change (tail->getUid (), props, previous,
                StructuralUtil::createSettings (true, true));
      }
      else if (tail->getType () == Structural::SwitchPort)
      {
        props[ST_ATTR_ENT_TYPE]
            = StructuralUtil::typeToStr (Structural::Mapping);
        props[ST_ATTR_EDGE_TAIL] = tail->getUid ();
        props[ST_ATTR_EDGE_HEAD] = head->getUid ();

        insert (StructuralUtil::createUid (), parentTail->getUid (), props,
                StructuralUtil::createSettings (true, true));
      }
    }
  }
}

QString
StructuralView::getNewId (StructuralEntity *ent)
{
  QString name = "";

  if (ent != nullptr)
  {
    StructuralType type = ent->getType ();

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

        for (StructuralEntity *e : _entities.values ())
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

  for (StructuralEntity *ent : _entities.values ())
  {
    if (ent->getParent () == nullptr)
    {
      if (ent->getTop () < top)
        top = ent->getTop ();

      if (ent->getLeft () < left)
        left = ent->getLeft ();

      if (ent->getLeft () + ent->getWidth () > right)
        right = ent->getLeft () + ent->getWidth ();

      if (ent->getTop () + ent->getHeight () > bottom)
        bottom = ent->getTop () + ent->getHeight ();
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
                               QMap<QString, QString> props)
{
  createEntity (type, props, StructuralUtil::createSettings ());
}

void
StructuralView::mouseMoveEvent (QMouseEvent *evt)
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
        QLineF (_tool->getLine ().p1 (), mapToScene (evt->pos ())));
    _scene->update ();
  }

  QGraphicsView::mouseMoveEvent (evt);
}

void
StructuralView::mousePressEvent (QMouseEvent *evt)
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
        QLineF (mapToScene (evt->pos ()), mapToScene (evt->pos ())));
    _tool->adjust (true);

    _linking = true;
  }
  else
  {
    QGraphicsView::mousePressEvent (evt);

    if (!evt->isAccepted ())
    {
      evt->accept ();

      select ("", StructuralUtil::createSettings ());
    }
  }
}

void
StructuralView::mouseReleaseEvent (QMouseEvent *evt)
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

          if (parentHead != nullptr && parentTail == parentHead->getParent ())
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

  QGraphicsView::mouseReleaseEvent (evt);
}

void
StructuralView::keyPressEvent (QKeyEvent *evt)
{
  if (evt->key () == Qt::Key_Delete || evt->key () == Qt::Key_Backspace)
  {
    performDelete ();
    evt->accept ();
  }
  else if (evt->key () == Qt::Key_Shift)
  {
    setMode (Structural::Linking);
    emit switchedLink (true);

    evt->accept ();
  }
  else if (evt->key () == Qt::Key_Control)
  {
    for (StructuralEntity *e : _entities.values ())
      e->setDraggable (true);

    evt->accept ();
  }

  if (!evt->isAccepted ())
    QGraphicsView::keyPressEvent (evt);
}

void
StructuralView::keyReleaseEvent (QKeyEvent *evt)
{
  if (evt->key () == Qt::Key_Shift)
  {
    setMode (Structural::Pointing);
    emit switchedPointer (true);

    evt->accept ();
  }
  else if (evt->key () == Qt::Key_Control)
  {
    for (StructuralEntity *e : _entities.values ())
      e->setDraggable (false);

    evt->accept ();
  }

  if (!evt->isAccepted ())
    QGraphicsView::keyReleaseEvent (evt);
}

void
StructuralView::wheelEvent (QWheelEvent *evt)
{
  if (evt->modifiers () & Qt::ControlModifier)
  {
    if (evt->delta () > 0)
      zoomIn ();
    else
      zoomOut ();

    evt->accept ();
  }
  else
  {
    QGraphicsView::wheelEvent (evt);
  }
}

void
StructuralView::dragEnterEvent (QDragEnterEvent *evt)
{
  QGraphicsView::dragEnterEvent (evt);

  if (!ST_DEFAULT_WITH_BODY)
  {
    if (!evt->isAccepted ())
    {
      QList<QUrl> list = evt->mimeData ()->urls ();
      StructuralType type
          = StructuralUtil::strToType (evt->mimeData ()->objectName ());

      if (!list.isEmpty ()
          || StructuralUtil::validateKinship (type, Structural::Body))
        evt->setAccepted (true);
      else
        evt->setAccepted (false);
    }
  }
}

void
StructuralView::dragMoveEvent (QDragMoveEvent *evt)
{
  QGraphicsView::dragMoveEvent (evt);

  if (!ST_DEFAULT_WITH_BODY)
  {
    if (!evt->isAccepted ())
    {
      QList<QUrl> list = evt->mimeData ()->urls ();
      StructuralType type
          = StructuralUtil::strToType (evt->mimeData ()->objectName ());

      if (!list.isEmpty ()
          || StructuralUtil::validateKinship (type, Structural::Body))
        evt->setAccepted (true);
      else
        evt->setAccepted (false);
    }
  }
}

void
StructuralView::dropEvent (QDropEvent *evt)
{
  QGraphicsView::dropEvent (evt);

  if (!ST_DEFAULT_WITH_BODY)
  {
    if (evt->isAccepted ())
    {
      QList<QUrl> list = evt->mimeData ()->urls ();
      StructuralType type
          = StructuralUtil::strToType (evt->mimeData ()->objectName ());

      if (!list.isEmpty ())
      {
        for (const QUrl &url : list)
        {
          QString filename = url.toLocalFile ();

          QMap<QString, QString> props;
          props[ST_ATTR_ENT_TYPE]
              = StructuralUtil::typeToStr (Structural::Media);
          props[ST_ATTR_ENT_ID]
              = StructuralUtil::formatId (QFileInfo (filename).baseName ());
          props[ST_ATTR_NODE_SRC] = filename;

          QPointF p = mapToScene (evt->pos ());

          props[ST_ATTR_ENT_TOP]
              = QString::number (p.y () - ST_DEFAULT_CONTENT_H / 2);
          props[ST_ATTR_ENT_LEFT]
              = QString::number (p.x () - ST_DEFAULT_CONTENT_W / 2);

          insert (StructuralUtil::createUid (), "", props,
                  StructuralUtil::createSettings ());
        }
      }
      else if (StructuralUtil::validateKinship (type, Structural::Body))
      {
        QPointF p = mapToScene (evt->pos ());

        QMap<QString, QString> props;
        props[ST_ATTR_ENT_TOP] = QString::number (p.y ());
        props[ST_ATTR_ENT_LEFT] = QString::number (p.x ());

        move (evt->mimeData ()->text (), "", props,
              StructuralUtil::createSettings ());
      }
    }
  }
}

void
StructuralView::clean ()
{
  select ("", StructuralUtil::createSettings ());

  for (const QString &uid : _entities.keys ())
  {
    QMap<QString, QString> settings
        = StructuralUtil::createSettings (true, false);
    settings[ST_SETTINGS_UNDO_TRACE] = "0";

    remove (uid, settings);
  }

  _references.clear ();
  _entities.clear ();
  _commands.clear ();

  emit switchedRedo (false);
  emit switchedUndo (false);
}

void
StructuralView::performLinkDialog (StructuralLink *ent)
{
  emit updateRequested ();

  _dialog->setMode (ent->getProperty (ST_ATTR_REFERENCE_XCONNECTOR_ID), "", "",
                    StructuralLinkDialog::EditLink);

  QMap<QString, QString> pLink;
  for (const QString &name : ent->getProperties ().keys ())
  {
    if (name.contains (ST_ATTR_LINKPARAM_NAME))
    {
      QString lpUid = name.right (name.length () - name.lastIndexOf (':') - 1);
      QString lpName = ent->getProperties ().value (name);
      QString lpValue = ent->getProperties ().value (
          QString (ST_ATTR_LINKPARAM_VALUE) + ":" + lpUid);

      pLink.insert (lpName, lpValue);
    }
  }

  _dialog->setConnectorParams (pLink);

  if (_dialog->exec ())
  {
    QMap<QString, QString> prev = ent->getProperties ();
    QMap<QString, QString> props = ent->getProperties ();

    props.insert (ST_ATTR_REFERENCE_XCONNECTOR_ID, _dialog->getConnector ());

    QMap<QString, QString> p = _dialog->getConnectorParams ();

    for (const QString &name : p.keys ())
    {
      if (props.key (name).contains (ST_ATTR_LINKPARAM_NAME))
      {
        QString key = props.key (name);
        QString uid = key.right (key.length () - key.lastIndexOf (':') - 1);

        if (!p.value (name).isEmpty ())
        {
          props.insert (QString (ST_ATTR_LINKPARAM_NAME) + ":" + uid, name);
          props.insert (QString (ST_ATTR_LINKPARAM_VALUE) + ":" + uid,
                        p.value (name));
        }
        else
        {
          props.remove (QString (ST_ATTR_LINKPARAM_NAME) + ":" + uid);
          props.remove (QString (ST_ATTR_LINKPARAM_VALUE) + ":" + uid);
        }
      }
      else if (!p.value (name).isEmpty ())
      {
        QString uid = StructuralUtil::createUid ();

        props.insert (QString (ST_ATTR_LINKPARAM_NAME) + ":" + uid, name);
        props.insert (QString (ST_ATTR_LINKPARAM_VALUE) + ":" + uid,
                      p.value (name));
      }
    }

    QMap<QString, QString> settings
        = StructuralUtil::createSettings (true, true);

    change (ent->getUid (), props, prev, settings);
  }
}

void
StructuralView::performBindDialog (StructuralBind *ent)
{
  QMap<QString, QString> pBind;
  for (const QString &name : ent->getProperties ().keys ())
  {
    if (name.contains (ST_ATTR_BINDPARAM_NAME))
    {
      QString bpUid = name.right (name.length () - name.lastIndexOf (':') - 1);
      QString bpName = ent->getProperties ().value (name);
      QString bpValue = ent->getProperties ().value (
          QString (ST_ATTR_BINDPARAM_VALUE) + ":" + bpUid);

      pBind.insert (bpName, bpValue);
    }
  }

  emit updateRequested ();

  if (StructuralUtil::isCondition (ent->getRole ()))
  {
    _dialog->setMode (
        ent->getHead ()->getProperty (ST_ATTR_REFERENCE_XCONNECTOR_ID), "", "",
        StructuralLinkDialog::EditCondition);

    _dialog->setCondition (ent->getId ());
    _dialog->setConditionParams (pBind);

    if (ent->getTail ()->getCategory () == Structural::Interface)
    {
      for (StructuralEntity *e : ent->getTail ()->getParent ()->getChildren ())
      {
        _dialog->addConditionInterface (
            e->getUid (), e->getId (),
            StructuralUtil::getIcon (e->getType ()));
      }

      _dialog->setConditionInterface (
          ent->getProperty (ST_ATTR_REFERENCE_INTERFACE_ID));
    }
    else
    {
      for (StructuralEntity *e : ent->getTail ()->getChildren ())
      {
        _dialog->addConditionInterface (
            e->getUid (), e->getId (),
            StructuralUtil::getIcon (e->getType ()));
      }

      if (!ST_DEFAULT_WITH_INTERFACES)
      {
        if (!ent->getProperty (ST_ATTR_REFERENCE_INTERFACE_UID).isEmpty ()
            && _entities.contains (
                   ent->getProperty (ST_ATTR_REFERENCE_INTERFACE_UID)))
        {
          StructuralEntity *interface = _entities.value (
              ent->getProperty (ST_ATTR_REFERENCE_INTERFACE_UID));

          _dialog->setConditionInterface (interface->getId ());
        }
      }
    }
  }
  else if (StructuralUtil::isAction (ent->getRole ()))
  {
    _dialog->setMode (
        ent->getTail ()->getProperty (ST_ATTR_REFERENCE_XCONNECTOR_ID), "", "",
        StructuralLinkDialog::EditAction);

    _dialog->setAction (ent->getId ());
    _dialog->setActionParams (pBind);

    if (ent->getHead ()->getCategory () == Structural::Interface)
    {
      for (StructuralEntity *e : ent->getHead ()->getParent ()->getChildren ())
      {
        _dialog->addActionInterface (e->getUid (), e->getId (),
                                     StructuralUtil::getIcon (e->getType ()));
      }

      _dialog->setActionInterface (
          ent->getProperty (ST_ATTR_REFERENCE_INTERFACE_ID));
    }
    else
    {
      for (StructuralEntity *e : ent->getHead ()->getChildren ())
      {
        _dialog->addActionInterface (e->getUid (), e->getId (),
                                     StructuralUtil::getIcon (e->getType ()));
      }

      if (!ST_DEFAULT_WITH_INTERFACES)
      {
        if (!ent->getProperty (ST_ATTR_REFERENCE_INTERFACE_UID).isEmpty ()
            && _entities.contains (
                   ent->getProperty (ST_ATTR_REFERENCE_INTERFACE_UID)))
        {
          StructuralEntity *interface = _entities.value (
              ent->getProperty (ST_ATTR_REFERENCE_INTERFACE_UID));

          _dialog->setActionInterface (interface->getId ());
        }
      }
    }
  }

  if (_dialog->exec ())
  {
    QMap<QString, QString> prev = ent->getProperties ();
    QMap<QString, QString> props = ent->getProperties ();

    QString role;
    QString property;
    QString interface;
    QMap<QString, QString> p;

    if (StructuralUtil::isCondition (ent->getRole ()))
    {
      role = _dialog->getCondition ();
      property = ST_ATTR_EDGE_TAIL;
      interface = _dialog->getConditionInterface ();
      p = _dialog->getConditionParams ();
    }
    else if (StructuralUtil::isAction (ent->getRole ()))
    {
      role = _dialog->getAction ();
      property = ST_ATTR_EDGE_HEAD;
      interface = _dialog->getActionInterface ();
      p = _dialog->getActionParams ();
    }

    props[ST_ATTR_BIND_ROLE] = role;
    props[ST_ATTR_ENT_ID] = role;

    if (!interface.isEmpty () && _entities.contains (interface))
    {
      props[property] = interface;
      props[ST_ATTR_REFERENCE_INTERFACE_UID] = interface;
      props[ST_ATTR_REFERENCE_INTERFACE_ID]
          = _entities.value (interface)->getId ();
    }

    for (const QString &name : p.keys ())
    {
      if (props.key (name).contains (ST_ATTR_BINDPARAM_NAME))
      {
        QString key = props.key (name);
        QString uid = key.right (key.length () - key.lastIndexOf (':') - 1);

        if (!p.value (name).isEmpty ())
        {
          props.insert (QString (ST_ATTR_BINDPARAM_NAME) + ":" + uid, name);
          props.insert (QString (ST_ATTR_BINDPARAM_VALUE) + ":" + uid,
                        p.value (name));
        }
        else
        {
          props.remove (QString (ST_ATTR_BINDPARAM_NAME) + ":" + uid);
          props.remove (QString (ST_ATTR_BINDPARAM_VALUE) + ":" + uid);
        }
      }
      else if (!p.value (name).isEmpty ())
      {
        QString uid = StructuralUtil::createUid ();

        props.insert (QString (ST_ATTR_BINDPARAM_NAME) + ":" + uid, name);
        props.insert (QString (ST_ATTR_BINDPARAM_VALUE) + ":" + uid,
                      p.value (name));
      }
    }

    QMap<QString, QString> settings
        = StructuralUtil::createSettings (true, true);

    change (ent->getUid (), props, prev, settings);
  }
}

StructuralEntity *
StructuralView::clone (StructuralEntity *ent, StructuralEntity *newparent)
{
  StructuralEntity *newent = nullptr;

  switch (ent->getType ())
  {
    case Structural::Media:
    {
      newent = new StructuralContent ();
      break;
    }

    case Structural::Link:
    {
      newent = new StructuralLink ();
      break;
    }

    case Structural::Body:
    case Structural::Context:
    case Structural::Switch:
    {
      newent = new StructuralComposition ();
      break;
    }

    case Structural::Area:
    case Structural::Property:
    case Structural::SwitchPort:
    case Structural::Port:
    {
      newent = new StructuralInterface ();
      break;
    }

    case Structural::Mapping:
    {
      newent = new StructuralEdge ();
      break;
    }

    case Structural::Bind:
    {
      newent = new StructuralBind ();
      assert (newparent != nullptr);
      break;
    }

    default:
    {
      break;
    }
  }

  if (newent != nullptr)
  {
    newent->setProperties (ent->getProperties ());

    if (newparent != nullptr)
      newent->setParent (newparent);

    for (StructuralEntity *child : ent->getChildren ())
      newent->addChild (clone (child, newent));
  }

  return newent;
}

bool
StructuralView::isChild (StructuralEntity *ent, StructuralEntity *parent)
{
  if (ent != parent)
    for (StructuralEntity *e : parent->getChildren ())
      if (e->getUid () == ent->getUid () || isChild (ent, e))
        return true;

  return false;
}

void
StructuralView::paste (StructuralEntity *ent, StructuralEntity *parent)
{
  paste (ent, parent, StructuralUtil::createUid (), true);
}

void
StructuralView::paste (StructuralEntity *ent, StructuralEntity *parent,
                       const QString &code, bool adjust)
{
  if (ent != nullptr)
  {
    QString uid = StructuralUtil::createUid ();

    QMap<QString, QString> props = ent->getProperties ();
    props[ST_ATTR_ENT_UID] = uid;

    if (adjust)
    {
      props.remove (ST_ATTR_ENT_TOP);
      props.remove (ST_ATTR_ENT_LEFT);
    }

    QMap<QString, QString> setting
        = StructuralUtil::createSettings (ST_VALUE_TRUE, ST_VALUE_TRUE, code);

    insert (uid, (parent != nullptr ? parent->getUid () : ""), props, setting);

    if (_entities.contains (uid))
    {
      _clipboardReferences[ent->getUid ()] = uid;

      for (StructuralEntity *e : ent->getChildren ())
      {
        if (e->getCategory () != Structural::Edge
            && e->getType () != Structural::Port
            && e->getType () != Structural::Link)
        {

          paste (e, _entities.value (uid), code, false);
        }
      }

      for (StructuralEntity *e : ent->getChildren ())
      {
        if (e->getCategory () == Structural::Edge
            || e->getType () == Structural::Port
            || e->getType () == Structural::Link)
        {

          QMap<QString, QString> p = e->getProperties ();

          QString prop = ST_ATTR_EDGE_TAIL;

          if (p.contains (prop))
            if (_clipboardReferences.contains (p.value (prop)))
              e->setProperty (prop,
                              _clipboardReferences.value (p.value (prop)));

          prop = ST_ATTR_EDGE_HEAD;

          if (p.contains (prop))
            if (_clipboardReferences.contains (p.value (prop)))
              e->setProperty (prop,
                              _clipboardReferences.value (p.value (prop)));

          prop = ST_ATTR_REFERENCE_COMPONENT_UID;

          if (p.contains (prop))
            if (_clipboardReferences.contains (p.value (prop)))
              e->setProperty (prop,
                              _clipboardReferences.value (p.value (prop)));

          prop = ST_ATTR_REFERENCE_INTERFACE_UID;

          if (p.contains (prop))
            if (_clipboardReferences.contains (p.value (prop)))
              e->setProperty (prop,
                              _clipboardReferences.value (p.value (prop)));

          prop = ST_ATTR_REFERENCE_LINK_UID;

          if (p.contains (prop))
            if (_clipboardReferences.contains (p.value (prop)))
              e->setProperty (prop,
                              _clipboardReferences.value (p.value (prop)));

          prop = ST_ATTR_REFERENCE_XCONNECTOR_UID;

          if (p.contains (prop))
            if (_clipboardReferences.contains (p.value (prop)))
              e->setProperty (prop,
                              _clipboardReferences.value (p.value (prop)));

          for (const QString &k : p.keys ())
            if (k.contains (ST_ATTR_LINKPARAM_NAME)
                || k.contains (ST_ATTR_LINKPARAM_VALUE)
                || k.contains (ST_ATTR_BINDPARAM_NAME)
                || k.contains (ST_ATTR_BINDPARAM_VALUE))
            {

              prop = k.left (k.length () - k.lastIndexOf (':') - 1);

              e->setProperty (prop + ":" + StructuralUtil::createUid (),
                              p.value (k));
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
  else if (!ST_DEFAULT_WITH_BODY)
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

    change (
        entity->getStructuralUid (),
        StructuralUtil::createProperties (nextTop, nextLeft, nextWidth,
                                          nextHeight),
        entity->getStructuralProperties (),
        StructuralUtil::createSettings (ST_VALUE_TRUE, ST_VALUE_FALSE, code));
  }

  qreal parentWidth;
  qreal parentHeight;

  if (entity != nullptr)
  {
    parentWidth = entity->getWidth ();
    parentHeight = entity->getHeight ();
  }
  else if (!ST_DEFAULT_WITH_BODY)
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
    properties[ST_ATTR_ENT_TOP] = QString::number (nextTop);
    properties[ST_ATTR_ENT_LEFT] = QString::number (nextLeft);

    change (
        e->getStructuralUid (), properties, e->getStructuralProperties (),
        StructuralUtil::createSettings (ST_VALUE_TRUE, ST_VALUE_FALSE, code));
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
           StructuralUtil::createSettings(ST_VALUE_TRUE, ST_VALUE_FALSE,
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
          properties[ST_ATTR_ENT_TOP] = QString::number (nextTop);
          properties[ST_ATTR_ENT_LEFT] = QString::number (nextLeft);

          change (link->getStructuralUid (), properties,
                  link->getStructuralProperties (),
                  StructuralUtil::createSettings (ST_VALUE_TRUE,
                                                  ST_VALUE_FALSE, code));

          break;
        }
      }
    }
  }
}
#endif
