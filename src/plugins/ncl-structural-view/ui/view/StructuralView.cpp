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
      _scene (new StructuralScene (this, _menu, this))
{
  _mode = Structural::Pointing;
  _zoom = ZOOM_ORIGINAL;

  _linking = false;
  _linkingHead = _linkingTail = nullptr;

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

  setScene (_scene);
  centerOn (0, 0);

  connect (&_commands, &QUndoStack::canUndoChanged, this,
           &StructuralView::canUndoChanged);
  connect (&_commands, &QUndoStack::canRedoChanged, this,
           &StructuralView::canRedoChanged);
}

StructuralScene *
StructuralView::scene ()
{
  return _scene;
}

StructuralLinkDialog *
StructuralView::dialog ()
{
  return _dialog;
}

StructuralInteractionMode
StructuralView::mode () const
{
  return _mode;
}

void
StructuralView::setMode (StructuralInteractionMode mode, bool notify)
{
  _mode = mode;

  if (notify)
    emit interactionModeChanged (_mode);
}

void
StructuralView::toogleMode (bool notify)
{
  if (mode () == StructuralInteractionMode::Pointing)
    setMode (StructuralInteractionMode::Linking, notify);
  else
    setMode (StructuralInteractionMode::Pointing, notify);
}

void
StructuralView::setError (const QString &uid, const QString &error)
{
  CPR_ASSERT (_scene->hasEntity (uid));

  StructuralEntity *ent = _scene->entity (uid);
  ent->setError (error);
  ent->adjust (true);
}

void
StructuralView::clearErrors ()
{
  for (StructuralEntity *ent : _scene->entities ().values ())
    ent->setError ("");
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
StructuralView::insert (QString uid, QString parent, QStrMap props,
                        QStrMap stgs)
{
  CPR_ASSERT (!_scene->hasEntity (uid));

  // 1. Create the new entity.
  StructuralType type = util::strToType (props[ST_ATTR_ENT_TYPE]);
  StructuralEntity *e = _scene->createEntity (type);
  CPR_ASSERT_NON_NULL (e);
  e->setUid (uid);

  // 2. Add to the scene and as child of p
  _scene->entities ()[uid] = e;

  StructuralEntity *p = nullptr;
  if (_scene->hasEntity (parent))
    p = _scene->entity (parent);

  if (type != StructuralType::Body && p == nullptr)
    CPR_ASSERT_NOT_REACHED ();

  e->setStructuralParent (p);
  if (p == nullptr)
    _scene->addItem (e);

  // 3. Update the properties of the new entity
  _scene->updateWithDefaultProperties (e);
  e->setProperties (props);
  e->setMenu (_menu);

  // 4. Adjust the parent
  if (p && (p->structuralType () == Structural::Body
            || p->structuralType () == Structural::Switch
            || p->structuralType () == Structural::Context))
  {
    auto comp = cast (StructuralComposition *, p);
    CPR_ASSERT_NON_NULL (comp);

    if (comp->isCollapsed ())
      comp->uncollapse ();
  }

  // 5. Adjust the entity
  adjustEntity (e, props, stgs);
  if (stgs.value (ST_SETTINGS_ADJUST_REFERS, "") != ST_VALUE_FALSE)
    adjustReferences (e);

  connect (e, &StructuralEntity::insertAsked, this, &StructuralView::insert);
  connect (e, &StructuralEntity::changeAsked, this,
           &StructuralView::changeEntity);
  // connect (e, &StructuralEntity::removed, this, &StructuralView::remove);
  connect (e, &StructuralEntity::selectAsked, this, &StructuralView::select);
  connect (e, &StructuralEntity::moveAsked, this, &StructuralView::move);

  if (stgs[ST_SETTINGS_NOTIFY] == ST_VALUE_TRUE)
    emit inserted (uid, parent, e->properties (), stgs);

  if (!ST_OPT_SHOW_INTERFACES)
  {
    // Since interfaces are hidden, no update related with that entities
    // are notified.  However, the structural view create a new id when this
    // property is empty, then the change must be explicit notified for the
    // hidden entities (interface in this case).
    emit changed (uid, e->properties (),
                  e->properties (), // this param is not used
                  // in this case
                  util::createSettings (false, false));
  }

  if (type == StructuralType::Body)
    emit canAddBody (false);
}

void
StructuralView::adjustEntity (StructuralEntity *e, const QStrMap &props,
                              const QStrMap &stgs)
{
  CPR_ASSERT_NON_NULL (e);
  StructuralEntity *p = e->structuralParent ();

  // Adjust 'references'
  if (stgs[ST_SETTINGS_ADJUST_REFERS] != ST_VALUE_FALSE)
    adjustReferences (e);

  // Adjust 'angles'
  StructuralBind *bind = cast (StructuralBind *, e);
  if (bind && !props.contains (ST_ATTR_EDGE_ANGLE))
    bind->setAngle (calcNewAngle (bind));

  // Adjust 'others'
  if (!props.contains (ST_ATTR_ENT_TOP) || !props.contains (ST_ATTR_ENT_LEFT))
    e->adjust (false);
  else
    e->adjust (true);

  //  Adjust 'references' (again?)
  if (stgs[ST_SETTINGS_ADJUST_REFERS] != ST_VALUE_FALSE)
  {
    if (e->category () == Structural::Interface && p != nullptr
        && p->structuralType () == Structural::Media)
    {
      if (p->isReference ())
        adjustReferences (p);
      else
        for (const QString &key : _scene->refs ().keys ())
        {
          if (_scene->refs ().contains (key)
              && _scene->refs ().value (key) == p->uid ()
              && _scene->hasEntity (key))
          {
            adjustReferences (_scene->entity (key));
          }
        }
    }
  }
}

qreal
StructuralView::calcNewAngle (StructuralBind *bind)
{
  int min = 100000;
  int max = -100000;

  qreal angle = 0;

  if (bind->hasOrigin () && bind->hasDestination ())
  {
    for (StructuralEntity *e : _scene->entities ().values ())
    {
      if (e->structuralType () == Structural::Bind && e != bind)
      {
        StructuralBind *b = cast (StructuralBind *, e);
        CPR_ASSERT_NON_NULL (b);
        if (b->hasOrigin () && b->hasDestination ())
        {
          if ((bind->origin () == b->origin ()
               || bind->origin () == b->destination ())
              && (bind->destination () == b->destination ()
                  || bind->destination () == b->origin ()))
          {
            int current = b->angle ();

            if (b->destination ()->structuralType () != Structural::Link)
              current = -current;

            if (max < current)
              max = current;

            if (min > current)
              min = current;
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

    if (bind->destination ()->structuralType () != Structural::Link)
      return -angle;
  }

  return angle;
}

void
StructuralView::remove (QString uid, QStrMap stgs)
{
  CPR_ASSERT (_scene->hasEntity (uid));

  StructuralEntity *e = _scene->entity (uid);
  StructuralEntity *p = e->structuralParent ();
  StructuralType type = e->structuralType ();

  // Removing 'references'...
  if (e->category () == Structural::Interface)
  {
    QStrMap s = util::createSettings (ST_VALUE_FALSE, ST_VALUE_FALSE,
                                      stgs.value (ST_SETTINGS_CODE));

    for (const QString &key : _scene->refs ().keys (e->uid ()))
      remove (key, s);

    // Only remove interface -> interface references. Keeping
    // Media -> Media references to enable undo.  All "trash" references
    // are ignore when saving the project.
    _scene->refs ().remove (e->uid ());
  }
  else if (e->structuralType () == Structural::Media)
  {
    for (const QString &key : _scene->refs ().keys (e->uid ()))
    {
      CPR_ASSERT (_scene->hasEntity (key));
      _scene->entity (key)->setReference (false);
      _scene->entity (key)->adjust (true);
    }
  }

  // Removing 'children'...
  if (stgs.value (ST_SETTINGS_UNDO_TRACE) != ST_VALUE_FALSE)
    stgs[ST_SETTINGS_UNDO] = ST_VALUE_TRUE;

  while (!e->children ().isEmpty ())
  {
    removeEntity (e->children ().first ()->uid (), stgs);
  }

  // Removing 'edges'...
  if (e->category () != Structural::Edge)
  {
    QVector<StructuralEntity *> relatives = util::neighbors (e);
    relatives += util::upNeighbors (e);

    for (StructuralEntity *rel : relatives)
    {
      if (rel->category () == Structural::Edge)
      {
        StructuralEdge *edge = cast (StructuralEdge *, rel);
        if (edge->origin () == e || edge->destination () == e)
        {
          if (edge->structuralType () != Structural::Reference)
          {
            removeEntity (edge->uid (),
                          util::createSettings (
                              ST_VALUE_TRUE, stgs.value (ST_SETTINGS_NOTIFY),
                              stgs.value (ST_SETTINGS_CODE)));
          }
          else
          {
            if (ST_OPT_SHOW_INTERFACES)
            {
              // In case of 'Structural::Reference' edge, just change
              // 'Structural::Port' properties.
              //
              // Note:
              // The 'tail' of a 'Structural::Reference' edge
              // is always a 'Structural::Port' entity.
              auto props = edge->origin ()->properties ();

              props.remove (ST_ATTR_REF_COMPONENT_ID);
              props.remove (ST_ATTR_REF_COMPONENT_UID);
              props.remove (ST_ATTR_REF_INTERFACE_ID);
              props.remove (ST_ATTR_REF_INTERFACE_UID);

              change (edge->origin ()->uid (), props,
                      util::createSettings (ST_VALUE_TRUE,
                                            stgs.value (ST_SETTINGS_NOTIFY),
                                            stgs.value (ST_SETTINGS_CODE)));
            }
          }
        }
      }
    }
  }

  // Removing 'params'...
  if (type == Structural::Link || type == Structural::Bind)
  {
    for (const QString &key : e->properties ().keys ())
    {
      if (key.contains (ST_ATTR_LINKPARAM_NAME)
          || key.contains (ST_ATTR_BINDPARAM_NAME))
      {
        if (stgs[ST_SETTINGS_NOTIFY] == ST_VALUE_TRUE)
        {
          emit removed (key.right (key.length () - key.lastIndexOf (':') - 1),
                        stgs);
        }
      }
    }
  }

  if (!ST_OPT_WITH_BODY && !ST_OPT_USE_FLOATING_INTERFACES)
  {
    if (e->structuralType () == Structural::Port)
    {
      StructuralEntity *target = nullptr;
      QString compUid = e->property (ST_ATTR_REF_COMPONENT_UID);
      QString interfUid = e->property (ST_ATTR_REF_INTERFACE_UID);

      if (_scene->hasEntity (compUid))
      {
        target = _scene->entity (compUid);

        if (_scene->hasEntity (interfUid))
          target = _scene->entity (interfUid);
      }

      if (target)
        target->setProperty (ST_ATTR_ENT_AUTOSTART, ST_VALUE_FALSE);
    }
  }

  // Removing 'others'...
  if (p)
    p->removeChild (e);
  else
    _scene->removeItem (e);

  _scene->entities ().remove (uid);
  delete e;

  if (uid == _selected)
    emit select ("", stgs);

  if (type == Structural::Body)
    emit canAddBody (true);

  if (stgs[ST_SETTINGS_NOTIFY] == ST_VALUE_TRUE)
    emit removed (uid, stgs);
}

void
StructuralView::change (QString uid, QStrMap props, QStrMap stgs)
{
  CPR_ASSERT (_scene->hasEntity (uid));

  StructuralEntity *ent = _scene->entity (uid);
  auto *comp = dynamic_cast<StructuralComposition *> (ent);

  QStrMap prev = ent->properties ();

  if (ent->isCollapsed ()
      && props.value (ST_ATTR_ENT_COLLAPSED) == ST_VALUE_FALSE)
  {
    comp->uncollapse ();
  }

  if (!ent->isCollapsed ()
      && props.value (ST_ATTR_ENT_COLLAPSED) == ST_VALUE_TRUE)
  {
    comp->collapse ();
  }

  if (!ST_OPT_WITH_BODY && !ST_OPT_USE_FLOATING_INTERFACES)
  {
    if (ent->structuralType () == Structural::Port)
    {
      StructuralEntity *lasttarget = nullptr;
      QString compUid = prev.value (ST_ATTR_REF_COMPONENT_UID);
      QString interfUid = prev.value (ST_ATTR_REF_INTERFACE_UID);
      if (_scene->hasEntity (compUid))
      {
        lasttarget = _scene->entity (compUid);

        if (_scene->hasEntity (interfUid))
          lasttarget = _scene->entity (interfUid);
      }

      if (lasttarget)
        lasttarget->setProperty (ST_ATTR_ENT_AUTOSTART, ST_VALUE_FALSE);
    }
  }

  ent->setProperties (props);
  adjustReferences (ent);
  ent->adjust (true);

  if (stgs[ST_SETTINGS_NOTIFY] == ST_VALUE_TRUE)
    emit changed (uid, props, prev, stgs);
}

void
StructuralView::adjustAllReferences ()
{
  for (const QString &uid : _scene->entities ().keys ())
  {
    // warn: getEntities can be changed by the adjustReferences bellow
    if (_scene->hasEntity (uid))
    {
      StructuralEntity *e = _scene->entity (uid);
      CPR_ASSERT_NON_NULL (e);
      if (e->category () == Structural::Edge
          || e->structuralType () == Structural::Port || e->isReference ())
      {
        adjustReferences (e);
        e->adjust (true);
      }
    }
  }
}

void
StructuralView::adjustReferences (StructuralEntity *ent)
{
  CPR_ASSERT_NON_NULL (ent);

  StructuralType type = ent->structuralType ();
  switch (type)
  {
    case Structural::Media:
    {
      // Adjusting other media entities that refer to the current one
      bool hasChange = false;
      if (ent->hasProperty (ST_ATTR_REF_REFER_ID))
      {
        QString referUid = ent->property (ST_ATTR_REF_REFER_UID);
        CPR_ASSERT (_scene->hasEntity (referUid));

        StructuralEntity *refer = _scene->entity (referUid);
        CPR_ASSERT (ent != refer);

        if (!refer->isReference ())
        {
          if (refer->structuralType () == Structural::Media)
          {
            QString instance = "new";
            if (ent->hasProperty (ST_ATTR_NODE_INSTANCE))
              instance = ent->property (ST_ATTR_NODE_INSTANCE);

            for (StructuralEntity *child : ent->children ())
            {
              if (child->isReference ())
              {
                if (_scene->hasEntity (
                        child->property (ST_ATTR_REF_REFER_UID)))
                  if (_scene->entity (child->property (ST_ATTR_REF_REFER_UID))
                          ->structuralParent ()
                      != refer)
                    remove (child->uid (),
                            util::createSettings (false, false));
              }
            }

            QVector<QString> entities;
            QStrMap referToChild, childToRefer;

            for (StructuralEntity *child : ent->children ())
            {
              if (child->isReference ())
              {
                childToRefer[child->uid ()]
                    = child->property (ST_ATTR_REF_REFER_UID);
                referToChild[child->property (ST_ATTR_REF_REFER_UID)]
                    = child->uid ();
              }

              entities.append (child->uid ());
            }

            for (StructuralEntity *child : refer->children ())
            {
              if (child->category () == Structural::Interface)
              {
                if (!referToChild.contains (child->uid ()))
                {
                  if (!entities.contains (
                          _scene->refs ().value (child->uid ())))
                  {
                    QString uid = util::createUid ();

                    QStrMap props = child->properties ();
                    props[ST_ATTR_ENT_UID] = uid;
                    props[ST_ATTR_ENT_REFERENCE] = ST_VALUE_TRUE;

                    props[ST_ATTR_REF_REFER_ID] = child->id ();
                    props[ST_ATTR_REF_REFER_UID] = child->uid ();

                    props.remove (ST_ATTR_ENT_TOP);
                    props.remove (ST_ATTR_ENT_LEFT);

                    childToRefer[uid] = child->uid ();
                    childToRefer[child->uid ()] = uid;

                    _scene->refs ()[uid] = child->uid ();

                    QStrMap settings = util::createSettings (false, false);
                    settings[ST_SETTINGS_ADJUST_REFERS] = ST_VALUE_FALSE;

                    insert (uid, ent->uid (), props, settings);
                  }
                }
              }
            }

            if (instance == "new")
            {
              for (StructuralEntity *child : ent->children ())
                if (!child->isReference ())
                {
                  for (const QString &key :
                       _scene->refs ().keys (child->uid ()))
                    remove (key, util::createSettings (false, false));
                }
            }
            else if (instance == "instSame" || instance == "gradSame")
            {
              for (StructuralEntity *child : ent->children ())
              {
                if (!child->isReference ())
                {
                  for (const QString &key :
                       _scene->refs ().keys (child->uid ()))
                  {
                    if (_scene->hasEntity (key)
                        && _scene->entity (key)->structuralParent () != refer)
                    {
                      remove (key, util::createSettings (false, false));
                    }
                  }
                }
              }

              bool hasNewEntity = false;
              for (StructuralEntity *child : ent->children ())
              {
                if (child->category () == Structural::Interface)
                {
                  if (!childToRefer.contains (child->uid ()))
                  {
                    if (!_scene->refs ().values ().contains (child->uid ()))
                    {
                      QString uid = util::createUid ();

                      QStrMap props = child->properties ();
                      props[ST_ATTR_ENT_UID] = uid;
                      props[ST_ATTR_ENT_REFERENCE] = ST_VALUE_TRUE;

                      props.remove (ST_ATTR_ENT_TOP);
                      props.remove (ST_ATTR_ENT_LEFT);

                      props[ST_ATTR_REF_REFER_ID] = child->id ();
                      props[ST_ATTR_REF_REFER_UID] = child->uid ();

                      _scene->refs ()[uid] = child->uid ();

                      QStrMap settings = util::createSettings (false, false);
                      settings[ST_SETTINGS_ADJUST_REFERS] = ST_VALUE_FALSE;

                      insert (uid, refer->uid (), props, settings);

                      hasNewEntity = true;
                    }
                  }
                }
              }

              if (hasNewEntity)
              {
                for (const QString &key : _scene->refs ().keys (refer->uid ()))
                {
                  if (_scene->hasEntity (key) && key != ent->uid ())
                    adjustReferences (_scene->entity (key));
                }
              }
            }

            _scene->refs ()[ent->uid ()] = refer->uid ();
            ent->setReference (true);
            hasChange = true;
          }
        }
      }

      if (_scene->refs ().contains (ent->uid ()))
      {
        if (!hasChange)
        {
          for (StructuralEntity *child : ent->children ())
          {
            if (child->isReference ())
            {
              remove (child->uid (), util::createSettings (false, false));
            }
            else
            {
              for (const QString &key : _scene->refs ().keys (child->uid ()))
                remove (key, util::createSettings (false, false));
            }
          }

          ent->setReference (false);
        }
      }

      // Adjusting values of relatives entities
      QVector<StructuralEntity *> relatives = util::neighbors (ent);
      relatives += util::upNeighbors (ent);

      for (StructuralEntity *rel : relatives)
      {
        // Adjusting relative edges
        if (rel->category () == Structural::Edge)
        {
          StructuralEdge *edge = cast (StructuralEdge *, rel);
          CPR_ASSERT_NON_NULL (edge);

          // Adjusting edge if it already has an tail and head defined.
          if (edge->origin () == ent || edge->destination () == ent)
          {
            adjustReferences (edge);
          }
          // Adjusting edge if it is a bind and a component value was defined
          else if (edge->structuralType () == Structural::Bind)
          {
            StructuralBind *bind = cast (StructuralBind *, edge);
            CPR_ASSERT_NON_NULL (bind);

            if (bind->hasProperty (ST_ATTR_REF_COMPONENT_ID)
                && bind->hasProperty (ST_ATTR_REF_INTERFACE_ID))
            {
              QString compId = bind->property (ST_ATTR_REF_COMPONENT_ID);
              if (ent->id () == compId)
              {
                if (bind->hasDestination ()
                    && bind->destination ()->structuralType ()
                           == Structural::Link)
                {
                  bind->setOrigin (ent);
                  bind->setProperty (ST_ATTR_EDGE_ORIG, ent->uid ());
                  bind->setProperty (ST_ATTR_REF_COMPONENT_UID, ent->uid ());
                }
                else if (bind->hasOrigin ()
                         && bind->origin ()->structuralType ()
                                == Structural::Link)
                {
                  bind->setDestination (ent);
                  bind->setProperty (ST_ATTR_EDGE_DEST, ent->uid ());
                  bind->setProperty (ST_ATTR_REF_COMPONENT_UID, ent->uid ());
                }

                adjustReferences (bind);
              }
            }
          }
        }

        // Adjusting relative ports
        else if (rel->category () == Structural::Interface)
        {
          StructuralInterface *interf = cast (StructuralInterface *, rel);
          CPR_ASSERT_NON_NULL (interf);

          if (!ST_OPT_SHOW_INTERFACES)
          {
            if (interf->structuralType () == Structural::Port)
            {
              if (interf->property (ST_ATTR_REF_COMPONENT_ID) == ent->id ())
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
      bool isVisible = true;
      StructuralEdge *edge = cast (StructuralEdge *, ent);
      CPR_ASSERT_NON_NULL (edge);

      StructuralEntity *orig = nullptr, *dest = nullptr;
      QString origUid = edge->property (ST_ATTR_EDGE_ORIG);
      QString destUid = edge->property (ST_ATTR_EDGE_DEST);

      if (_scene->hasEntity (origUid))
        orig = _scene->entity (origUid);

      if (_scene->hasEntity (destUid))
        dest = _scene->entity (destUid);

      edge->setOrigin (orig);
      edge->setDestination (dest);

      if (orig && dest)
      {
        //        if (orig->getCategory () == Structural::Interface)
        //          edge->setOrigin (orig->getParent ());
        //        if (dest->getCategory () == Structural::Interface)
        //          edge->setDestination (dest->getParent ());

        if (ent->structuralParent ()
            && ent->structuralParent ()->isCollapsed ())
          isVisible = false;
      }
      else
        isVisible = false;

      ent->setHidden (!isVisible);

      break;
    }

    case Structural::Area:
    case Structural::Property:
    case Structural::Port:
    case Structural::Switch:
    {
      QVector<StructuralEntity *> relatives = util::neighbors (ent);
      relatives += util::upNeighbors (ent);

      for (StructuralEntity *rel : relatives)
      {
        if (rel->category () == Structural::Edge)
        {
          StructuralEdge *edge = cast (StructuralEdge *, rel);
          CPR_ASSERT_NON_NULL (edge);

          if (edge->origin () == ent || edge->destination () == ent)
          {
            adjustReferences (edge);
          }
          else if (edge->structuralType () == Structural::Bind)
          {
            StructuralEdge *bind = cast (StructuralEdge *, edge);
            CPR_ASSERT_NON_NULL (bind);

            QString interId = bind->property (ST_ATTR_REF_INTERFACE_ID);
            if (!interId.isEmpty ())
            {
              if (ent->id () == interId)
              {
                if (bind->hasDestination ()
                    && bind->destination ()->structuralType ()
                           == Structural::Link)
                {
                  bind->setOrigin (ent);
                  bind->setProperty (ST_ATTR_EDGE_ORIG, ent->uid ());
                }
                else if (bind->hasOrigin ()
                         && bind->origin ()->structuralType ()
                                == Structural::Link)
                {
                  bind->setDestination (ent);
                  bind->setProperty (ST_ATTR_EDGE_DEST, ent->uid ());
                }

                bind->setProperty (ST_ATTR_REF_INTERFACE_UID, ent->uid ());
                bind->setProperty (ST_ATTR_REF_COMPONENT_ID,
                                   ent->structuralParent ()->id ());
                bind->setProperty (ST_ATTR_REF_COMPONENT_UID,
                                   ent->structuralParent ()->uid ());
                adjustReferences (bind);
              }
            }
          }
        }
      }

      if (type == Structural::Port)
      {
        if (ST_OPT_SHOW_INTERFACES)
        {
          for (StructuralEntity *e : _scene->entities ().values ())
          {
            if (e->structuralType () == Structural::Reference
                && e->property (ST_ATTR_EDGE_ORIG) == ent->uid ())
            {
              remove (e->uid (), util::createSettings (false, false));
            }
          }

          StructuralEntity *comp = nullptr;
          StructuralEntity *interf = nullptr;

          QString compId = ent->property (ST_ATTR_REF_COMPONENT_ID);
          QString compUid = ent->property (ST_ATTR_REF_COMPONENT_UID);
          QString interfId = ent->property (ST_ATTR_REF_INTERFACE_ID);
          QString interfUid = ent->property (ST_ATTR_REF_INTERFACE_UID);

          if (_scene->hasEntity (interfUid))
            interf = _scene->entity (interfUid);

          if (_scene->hasEntity (compUid))
            comp = _scene->entity (compUid);

          StructuralEntity *dest = nullptr;
          if (comp)
          {
            dest = comp;

            if (interf)
            {
              if (interf->structuralParent () == comp)
                dest = interf;
            }
          }

          if (dest)
          {
            QStrMap props = {
              { ST_ATTR_ENT_TYPE, util::typeToStr (Structural::Reference) },
              { ST_ATTR_EDGE_ORIG, ent->uid () },
              { ST_ATTR_EDGE_DEST, dest->uid () },
              { ST_ATTR_REF_COMPONENT_ID, compId },
              { ST_ATTR_REF_COMPONENT_UID, compUid },
              { ST_ATTR_REF_INTERFACE_ID, interfId },
              { ST_ATTR_REF_INTERFACE_UID, interfUid }
            };

            QString parentUid = "";
            if (ent->structuralParent ())
              parentUid = ent->structuralParent ()->uid ();

            if (!ST_OPT_WITH_BODY && !ST_OPT_USE_FLOATING_INTERFACES)
            {
              if (ent->structuralParent ())
              {
                ent->setHidden (true);
                dest->setProperty (ST_ATTR_ENT_AUTOSTART, ST_VALUE_TRUE);
                props[ST_ATTR_ENT_HIDDEN] = ST_VALUE_TRUE;
              }
            }

            insert (util::createUid (), parentUid, props,
                    util::createSettings (false, false));
          }
        }
        else if (!ST_OPT_USE_FLOATING_INTERFACES)
        {
          StructuralEntity *comp = nullptr;
          StructuralEntity *interf = nullptr;
          QString compUid = ent->property (ST_ATTR_REF_COMPONENT_UID);
          QString interfUid = ent->property (ST_ATTR_REF_INTERFACE_UID);

          if (_scene->hasEntity (compUid))
            comp = _scene->entity (compUid);

          if (_scene->hasEntity (interfUid))
            interf = _scene->entity (interfUid);

          QStrMap props;
          QStrMap settings = util::createSettings (true, true);

          if (interf)
          {
            if (comp && interf->structuralParent () == comp)
            {
              props = interf->properties ();
              props[ST_ATTR_ENT_AUTOSTART] = ST_VALUE_TRUE;

              change (interf->uid (), props, settings);
            }
          }
          else if (comp)
          {
            props = comp->properties ();
            props[ST_ATTR_ENT_AUTOSTART] = ST_VALUE_TRUE;

            change (comp->uid (), props, settings);
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
StructuralView::select (QString uid, QStrMap stgs)
{
  CPR_ASSERT (_scene->hasEntity (uid) || uid == "");

  if (uid != _selected)
  {
    unselect ();

    if (uid != "")
    {
      StructuralEntity *ent = _scene->entity (uid);
      ent->setSelected (true);
      _selected = ent->uid ();
      CPR_ASSERT (ent->uid () == uid);

      bool canPaste = false;
      if (_clipboard && !_clipboard->isChild (ent)
          && ent->uid () != _clipboard->uid ())
      {
        if (util::validateKinship (_clipboard->structuralType (),
                                   ent->structuralType ()))
          canPaste = true;
      }

      emit canCutChanged (true);
      emit canCopyChanged (true);
      emit canPasteChanged (canPaste);
    }
  }

  if (stgs[ST_SETTINGS_NOTIFY] == ST_VALUE_TRUE)
    emit selected (uid, stgs);

  _scene->update ();
}

void
StructuralView::unselect ()
{
  if (_scene->hasEntity (_selected))
    _scene->entity (_selected)->setSelected (false);

  _selected = "";

  bool canPaste = false;
  if (_clipboard && !ST_OPT_WITH_BODY)
  {
    if (util::validateKinship (_clipboard->structuralType (),
                               Structural::Body))
      canPaste = true;
  }

  emit canCutChanged (false);
  emit canCopyChanged (false);
  emit canPasteChanged (canPaste);
}

void
StructuralView::move (QString uid, QString parent, QStrMap props, QStrMap stgs)
{
  CPR_ASSERT (_scene->hasEntity (uid));
  CPR_ASSERT (_scene->hasEntity (parent) || parent == "");

  StructuralEntity *e = _scene->entity (uid);
  StructuralEntity *p = _scene->entity (parent);
  StructuralType type = Structural::Body;

  if (p)
    type = p->structuralType ();

  CPR_ASSERT (util::validateKinship (e->structuralType (), type));

  if (e->structuralParent () != p)
  {
    for (StructuralEntity *ent : _scene->entities ().values ())
      ent->setDraggable (false);

    select ("", stgs);

    StructuralEntity *copy = _scene->clone (e);

    for (const QString &name : props.keys ())
      copy->setProperty (name, props.value (name));

    copy->adjust ();

    if (props.contains (ST_ATTR_ENT_TOP))
      copy->setTop (copy->top () - copy->height () / 2);

    if (props.contains (ST_ATTR_ENT_LEFT))
      copy->setLeft (copy->left () - copy->width () / 2);

    pasteEntity (copy, p, stgs.value (ST_SETTINGS_CODE), false);
    remove (uid, stgs);
  }
}

void
StructuralView::createEntity (StructuralType type, QStrMap props, QStrMap stgs)
{
  props.insert (ST_ATTR_ENT_TYPE, util::typeToStr (type));

  if (stgs.empty ())
    stgs = util::createSettings ();

  QString uid = util::createUid ();
  QString parent = "";

  if (_scene->hasEntity (_selected))
    parent = _selected;

  if (!props.contains (ST_ATTR_ENT_TYPE))
    props[ST_ATTR_ENT_TYPE] = util::typeToStr (type);

  if (!stgs.contains (ST_SETTINGS_UNDO))
    stgs[ST_SETTINGS_UNDO] = ST_VALUE_TRUE;

  if (!stgs.contains (ST_SETTINGS_NOTIFY))
    stgs[ST_SETTINGS_NOTIFY] = ST_VALUE_TRUE;

  if (!stgs.contains (ST_SETTINGS_CODE))
    stgs[ST_SETTINGS_CODE] = util::createUid ();

  Insert *cmd = new Insert (this, uid, parent, props, stgs);
  cmd->setText (stgs[ST_SETTINGS_CODE]);
  _commands.push (cmd);
}

void
StructuralView::changeEntity (QString uid, QStrMap props, QStrMap prev,
                              QStrMap stgs)
{
  Change *cmd = new Change (this, uid, props, prev, stgs);
  cmd->setText (stgs[ST_SETTINGS_CODE]);
  _commands.push (cmd);
}

void
StructuralView::removeEntity (QString uid, QStrMap stgs)
{
  CPR_ASSERT (_scene->hasEntity (uid));

  StructuralEntity *e = _scene->entity (uid);
  StructuralEntity *p = e->structuralParent ();

  if (!e->isReference () || e->category () != Structural::Interface)
  {
    QString parent = "";
    if (p)
      parent = p->uid ();

    Remove *cmd = new Remove (this, e->uid (), parent, e->properties (), stgs);
    cmd->setText (stgs[ST_SETTINGS_CODE]);

    _commands.push (cmd);
  }
}

void
StructuralView::performAutostart ()
{
  CPR_ASSERT_X (!ST_OPT_WITH_BODY && !ST_OPT_USE_FLOATING_INTERFACES,
                "performAutostart",
                "Autostart is only available when when body and floating"
                "interfaces are disabled in build");

  CPR_ASSERT (_scene->hasEntity (_selected));

  StructuralEntity *e = _scene->entity (_selected);
  QStrMap props = e->properties ();

  QStrMap stgs = util::createSettings (true, true);

  // Case 1: media already has an autostart property
  if (props.contains (ST_ATTR_ENT_AUTOSTART)
      && props[ST_ATTR_ENT_AUTOSTART] == ST_VALUE_TRUE)
  {
    QVector<StructuralEntity *> neighbors = util::neighbors (e);

    if (ST_OPT_SHOW_INTERFACES)
    {
      if (e->category () == Structural::Interface)
        neighbors += util::upNeighbors (e);
    }

    // If ST_DEFAULT_WITH_INTERFACES is disabled, interface entities are
    // just hidden. So, it is OK to iterate through interfaces, since they
    // exist even with ST_DEFAULT_WITH_INTERFACES disabled.
    for (StructuralEntity *neighbor : neighbors)
    {
      if (neighbor->category () == Structural::Interface)
      {
        if (neighbor->property (ST_ATTR_REF_COMPONENT_UID) == e->uid ()
            || (ST_OPT_SHOW_INTERFACES
                && neighbor->property (ST_ATTR_REF_INTERFACE_UID)
                       == e->uid ()))
        {
          remove (neighbor->uid (), stgs);
          break;
        }
      }
    }

    props[ST_ATTR_ENT_AUTOSTART] = ST_VALUE_FALSE;
  }
  // Case 2: media does not has autostart property
  else
  {
    QString pParent = "";
    QStrMap pProps
        = { { ST_ATTR_ENT_TYPE, util::typeToStr (Structural::Port) } };

    if (e->category () == Structural::Interface)
    {
      if (e->structuralParent ())
      {
        pProps.insert (ST_ATTR_REF_COMPONENT_UID,
                       e->structuralParent ()->uid ());
        pProps.insert (ST_ATTR_REF_COMPONENT_ID,
                       e->structuralParent ()->id ());

        pProps.insert (ST_ATTR_REF_INTERFACE_UID, e->uid ());
        pProps.insert (ST_ATTR_REF_INTERFACE_ID, e->id ());

        pParent = e->structuralParent ()->structuralParent () != nullptr
                      ? e->structuralParent ()->structuralParent ()->uid ()
                      : "";
      }
    }
    else
    {
      pProps.insert (ST_ATTR_REF_COMPONENT_UID, e->uid ());
      pProps.insert (ST_ATTR_REF_COMPONENT_ID, e->id ());

      pParent = e->structuralParent () != nullptr
                    ? e->structuralParent ()->uid ()
                    : "";
    }

    insert (util::createUid (), pParent, pProps, stgs);

    props[ST_ATTR_ENT_AUTOSTART] = ST_VALUE_TRUE;
  }

  change (e->uid (), props, util::createSettings (true, true));
}

void
StructuralView::undo ()
{
  if (_commands.canUndo ())
  {
    QString code = _commands.undoText ();

    while (code == _commands.undoText ())
      _commands.undo ();
  }
}

void
StructuralView::redo ()
{
  if (_commands.canRedo ())
  {
    QString code = _commands.redoText ();

    while (code == _commands.redoText ())
      _commands.redo ();
  }
}

void
StructuralView::cut ()
{
  copy ();

  if (_scene->hasEntity (_selected))
  {
    StructuralEntity *ent = _scene->entity (_selected);

    _clipboard->setId (ent->id ());

    if (_clipboard->uid () == ent->uid ())
      remove (_selected, util::createSettings ());
  }
  else
  {
    qWarning (CPR_PLUGIN_STRUCT) << "Trying to cut an entity, but no entity is"
                                    " selected.";
  }
}

void
StructuralView::copy ()
{
  CPR_ASSERT (_selected == "" || _scene->hasEntity (_selected));

  if (_scene->hasEntity (_selected))
  {
    if (_clipboard)
    {
      delete _clipboard;
      _clipboardRefs.clear ();
    }

    StructuralEntity *ent = _scene->entity (_selected);
    if (ent != nullptr && ent->category () != Structural::Edge
        && ent->structuralType () != Structural::Body)
    {
      _clipboard = _scene->clone (ent);
      emit canPasteChanged (false);
    }
  }
  else
  {
    qWarning (CPR_PLUGIN_STRUCT)
        << "Trying to copy, but no entity is selected.";
  }
}

void
StructuralView::paste ()
{
  if (_clipboard)
  {
    StructuralEntity *parent = nullptr;
    if (_scene->hasEntity (_selected))
      parent = _scene->entity (_selected);

    bool isValidPaste = false;
    if (!ST_OPT_WITH_BODY)
    {
      if (util::validateKinship (_clipboard->structuralType (),
                                 Structural::Body))
        isValidPaste = true;
    }
    else
    {
      if (parent)
      {
        isValidPaste
            = (_clipboard->uid () != parent->uid ())
              && (util::validateKinship (_clipboard->structuralType (),
                                         parent->structuralType ()))
              && (!_clipboard->isChild (parent));
      }
    }

    if (isValidPaste)
    {
      // 0: Cancel - Nothing todo.
      // 1: No - Paste entity.
      // 2: Yes - Create refer entity.
      int result = 1;

      if (_scene->hasEntity (_clipboard->uid ()))
        result = QMessageBox::question (
            this, tr ("Paste"),
            tr ("Would you like to make a reference instead?"), tr ("Cancel"),
            tr ("No"), tr ("Yes"), 1);

      if (result == 1)
      {
        StructuralType type = _clipboard->structuralType ();
        QString nextID = _scene->createNewId (
            type, QString ("c_") + _clipboard->id () + "_");
        _clipboard->setId (nextID);

        pasteEntity (_clipboard, parent, util::createUid (), true);
      }
      else if (result == 2)
      {
        QString uid = util::createUid ();

        QStrMap props = _clipboard->properties ();
        props[ST_ATTR_ENT_UID] = uid;

        QString refID = _clipboard->id ();
        refID = refID.section ('_', 1, 1);

        int n = 0;
        for (StructuralEntity *entity : _scene->entities ().values ())
          if (entity->id ().startsWith ("r_" + refID))
            ++n;

        if (n > 0)
          refID = "r_" + refID + "_" + QString::number (n);
        else
          refID = "r_" + refID;

        props[ST_ATTR_ENT_ID] = refID;

        props[ST_ATTR_REF_REFER_ID] = _clipboard->id ().section ('_', 1, 1);
        props[ST_ATTR_REF_REFER_UID] = _clipboard->uid ();

        props.remove (ST_ATTR_ENT_TOP);
        props.remove (ST_ATTR_ENT_LEFT);

        insert (uid, (parent != nullptr ? parent->uid () : ""), props,
                util::createSettings ());
      }

      // Fix entity drag status
      for (StructuralEntity *e : _scene->entities ().values ())
        e->setDraggable (false);
    }
  }
  else
  {
    qWarning (CPR_PLUGIN_STRUCT) << "Trying to paste, but there clipboard is "
                                    "empty.";
  }
}

void
StructuralView::createLink (StructuralEntity *orig, StructuralEntity *dest)
{
  StructuralEntity *parentOrig = orig->structuralParent ();
  StructuralEntity *parentDest = dest->structuralParent ();

  if ((parentOrig && parentDest) || !ST_OPT_WITH_BODY)
  {
    StructuralEntity *parent = nullptr;
    QPointF pOrig, pDest;

    if (parentOrig == parentDest)
    {
      pOrig = QPointF (orig->left (), orig->top ());
      pDest = QPointF (dest->left (), dest->top ());

      parent = parentOrig;
    }
    else if (parentOrig && (parentOrig->structuralParent () == parentDest)
             && (orig->category () == Structural::Interface))
    {
      pDest = QPointF (dest->left (), dest->top ());
      pOrig = parentOrig->mapToParent (orig->left (), orig->top ());

      parent = parentDest;
    }
    else if (parentDest && (parentOrig == parentDest->structuralParent ())
             && (dest->category () == Structural::Interface))
    {
      pOrig = QPointF (orig->left (), orig->top ());
      pDest = parentDest->mapToParent (dest->left (), dest->top ());

      parent = parentOrig;
    }
    else if ((parentOrig && parentDest) && (parentOrig->structuralParent ()
                                            == parentDest->structuralParent ())
             && (orig->category () == Structural::Interface)
             && (dest->category () == Structural::Interface))
    {
      pOrig = parentOrig->mapToParent (orig->left (), orig->top ());
      pDest = parentDest->mapToParent (dest->left (), dest->top ());

      parent = parentOrig->structuralParent ();
    }

    setMode (Structural::Pointing, true);

    if (parent || !ST_OPT_WITH_BODY)
    {
      emit updateRequested ();

      if (orig->category () == Structural::Interface)
      {
        for (StructuralEntity *ent : parentOrig->children ())
        {
          _dialog->addConditionInterface (ent->uid (), ent->id (),
                                          util::icon (ent->structuralType ()));
        }

        _dialog->setConditionInterface (orig->id ());
      }
      else
      {
        for (StructuralEntity *ent : orig->children ())
        {
          _dialog->addConditionInterface (ent->uid (), ent->id (),
                                          util::icon (ent->structuralType ()));
        }
      }

      if (dest->category () == Structural::Interface)
      {
        for (StructuralEntity *ent : parentDest->children ())
        {
          _dialog->addActionInterface (ent->uid (), ent->id (),
                                       util::icon (ent->structuralType ()));
        }

        _dialog->setActionInterface (dest->id ());
      }
      else
      {
        for (StructuralEntity *ent : dest->children ())
        {
          _dialog->addActionInterface (ent->uid (), ent->id (),
                                       util::icon (ent->structuralType ()));
        }
      }

      _dialog->setMode ();

      if (_dialog->exec ())
      {

        if (!_dialog->getConditionInterface ().isEmpty ())
        {
          QString uid = _dialog->getConditionInterface ();

          if (_scene->hasEntity (uid))
          {
            orig = _scene->entity (uid);
            parentOrig = orig->structuralParent ();

            pOrig = parentOrig->mapToParent (orig->left (), orig->top ());
          }
        }

        if (!_dialog->getActionInterface ().isEmpty ())
        {
          QString uid = _dialog->getActionInterface ();

          if (_scene->hasEntity (uid))
          {
            dest = _scene->entity (uid);
            parentDest = dest->structuralParent ();

            pDest = parentDest->mapToParent (dest->left (), dest->top ());
          }
        }

        pOrig.setX (pOrig.x () + orig->width () / 2);
        pOrig.setY (pOrig.y () + orig->height () / 2);
        pDest.setX (pDest.x () + dest->width () / 2);
        pDest.setY (pDest.y () + dest->height () / 2);

        QString uid = util::createUid ();

        QStrMap props;
        props[ST_ATTR_ENT_TYPE] = util::typeToStr (Structural::Link);
        props[ST_ATTR_REF_XCONNECTOR_ID] = _dialog->getConnector ();

        qreal x = 0.0, y = 0.0;

        if (pOrig.x () <= pDest.x () && pOrig.y () <= pDest.y ())
        {
          if (pOrig.x () + orig->width () / 2
              < pDest.x () - dest->width () / 2)
            x = (pOrig.x () + orig->width () / 2 + pDest.x ()
                 - dest->width () / 2)
                / 2;
          else
            x = pOrig.x () + (pDest.x () - pOrig.x ()) / 2;

          if (pOrig.y () + orig->height () / 2
              < pDest.y () - dest->height () / 2)
            y = (pOrig.y () + orig->height () / 2 + pDest.y ()
                 - dest->height () / 2)
                / 2;
          else
            y = pOrig.y () + (pDest.y () - pOrig.y ()) / 2;
        }
        else if (pOrig.x () > pDest.x () && pOrig.y () <= pDest.y ())
        {
          if (pDest.x () + dest->width () / 2
              < pOrig.x () - orig->width () / 2)
            x = (pDest.x () + dest->width () / 2 + pOrig.x ()
                 - orig->width () / 2)
                / 2;
          else
            x = pDest.x () + (pOrig.x () - pDest.x ()) / 2;

          if (pOrig.y () + orig->height () / 2
              < pDest.y () - dest->height () / 2)
            y = (pOrig.y () + orig->height () / 2 + pDest.y ()
                 - dest->height () / 2)
                / 2;
          else
            y = pOrig.y () + (pDest.y () - pOrig.y ()) / 2;
        }
        else if (pOrig.x () <= pDest.x () && pOrig.y () > pDest.y ())
        {
          if (pOrig.x () + orig->width () / 2
              < pDest.x () - dest->width () / 2)
            x = (pOrig.x () + orig->width () / 2 + pDest.x ()
                 - dest->width () / 2)
                / 2;
          else
            x = pOrig.x () + (pDest.x () - pOrig.x ()) / 2;

          if (pDest.y () + dest->height () / 2
              < pOrig.y () - orig->height () / 2)
            y = (pDest.y () + dest->height () / 2 + pOrig.y ()
                 - orig->height () / 2)
                / 2;
          else
            y = pDest.y () + (pOrig.y () - pDest.y ()) / 2;
        }
        else if (pOrig.x () > pDest.x () && pOrig.y () > pDest.y ())
        {
          if (pDest.x () + dest->width () / 2
              < pOrig.x () - orig->width () / 2)
            x = (pDest.x () + dest->width () / 2 + pOrig.x ()
                 - orig->width () / 2)
                / 2;
          else
            x = pDest.x () + (pOrig.x () - pDest.x ()) / 2;

          if (pDest.y () + dest->height () / 2
              < pOrig.y () - orig->height () / 2)
            y = (pDest.y () + dest->height () / 2 + pOrig.y ()
                 - orig->height () / 2)
                / 2;
          else
            y = pDest.y () + (pOrig.y () - pDest.y ()) / 2;
        }

        props[ST_ATTR_ENT_TOP] = QString::number (y);
        props[ST_ATTR_ENT_LEFT] = QString::number (x);

        QStrMap params = _dialog->getConnectorParams ();
        for (const QString &key : params.keys ())
        {
          if (!params.value (key).isEmpty ())
          {
            QString uid = util::createUid ();

            props[QString (ST_ATTR_LINKPARAM_NAME) + ":" + uid] = key;
            props[QString (ST_ATTR_LINKPARAM_VALUE) + ":" + uid]
                = params.value (key);
          }
        }

        QStrMap settings = util::createSettings (true, true);

        insert (uid, (parent != nullptr ? parent->uid () : ""), props,
                settings);

        CPR_ASSERT (_scene->hasEntity (uid));
        createBind (orig, _scene->entity (uid), _dialog->getCondition (),
                    settings.value (ST_SETTINGS_CODE));
        createBind (_scene->entity (uid), dest, _dialog->getAction (),
                    settings.value (ST_SETTINGS_CODE));
      }
    }
  }
}

void
StructuralView::createBind (StructuralEntity *orig, StructuralEntity *dest,
                            const QString &role, const QString &code)
{
  StructuralEntity *parentOrig = orig->structuralParent ();
  StructuralEntity *parentDest = dest->structuralParent ();

  if ((parentOrig && parentDest) || !ST_OPT_WITH_BODY)
  {
    StructuralEntity *parent = nullptr;

    if (parentOrig == parentDest)
    {
      parent = parentOrig;
    }
    else if (parentOrig && (parentOrig->structuralParent () == parentDest)
             && (orig->category () == Structural::Interface))
    {
      parent = parentDest;
    }
    else if (parentDest && (parentOrig == parentDest->structuralParent ())
             && (dest->category () == Structural::Interface))
    {
      parent = parentOrig;
    }

    if (parent || !ST_OPT_WITH_BODY)
    {
      QString uid = util::createUid ();

      QStrMap props = {
        { ST_ATTR_ENT_TYPE, util::typeToStr (Structural::Bind) },
        { ST_ATTR_EDGE_ORIG, orig->uid () },
        { ST_ATTR_EDGE_DEST, dest->uid () },
        { ST_ATTR_BIND_ROLE, role },
        { ST_ATTR_ENT_ID, role } // Why?
      };

      StructuralEntity *entityLink = nullptr;
      StructuralEntity *entityNonLink = nullptr;
      StructuralEntity *parentNonLink = nullptr;

      if (orig->structuralType () == Structural::Link)
      {
        entityLink = orig;
        entityNonLink = dest;
        parentNonLink = parentDest;

        if (role.isEmpty ())
        {
          emit updateRequested ();

          setMode (Structural::Pointing, true);

          if (entityNonLink->category () == Structural::Interface)
          {
            for (StructuralEntity *ent : parentNonLink->children ())
            {
              _dialog->addActionInterface (
                  ent->uid (), ent->id (),
                  util::icon (ent->structuralType ()));
            }

            _dialog->setActionInterface (entityNonLink->id ());
          }
          else
          {
            for (StructuralEntity *ent : entityNonLink->children ())
            {
              _dialog->addActionInterface (
                  ent->uid (), ent->id (),
                  util::icon (ent->structuralType ()));
            }
          }

          _dialog->setMode (entityLink->property (ST_ATTR_REF_XCONNECTOR_ID),
                            "", "", StructuralLinkDialog::CreateAction);

          if (_dialog->exec ())
          {
            if (!_dialog->getActionInterface ().isEmpty ())
            {
              QString uid = _dialog->getActionInterface ();

              if (_scene->hasEntity (uid))
              {
                entityNonLink = _scene->entity (uid);
                parentNonLink = entityNonLink->structuralParent ();

                props[ST_ATTR_EDGE_DEST] = entityNonLink->uid ();
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

        QStrMap params = _dialog->getActionParams ();
        for (const QString &key : params.keys ())
        {
          if (!params.value (key).isEmpty ())
          {
            QString uid = util::createUid ();

            props.insert (QString (ST_ATTR_BINDPARAM_NAME) + ":" + uid, key);
            props.insert (QString (ST_ATTR_BINDPARAM_VALUE) + ":" + uid,
                          params.value (key));
          }
        }
      }
      else if (dest->structuralType () == Structural::Link)
      {
        entityLink = dest;
        entityNonLink = orig;
        parentNonLink = parentOrig;

        if (role.isEmpty ())
        {
          emit updateRequested ();

          setMode (Structural::Pointing, true);

          if (entityNonLink->category () == Structural::Interface)
          {
            for (StructuralEntity *ent : parentNonLink->children ())
            {
              _dialog->addConditionInterface (
                  ent->uid (), ent->id (),
                  util::icon (ent->structuralType ()));
            }

            _dialog->setConditionInterface (entityNonLink->id ());
          }
          else
          {
            for (StructuralEntity *ent : entityNonLink->children ())
            {
              _dialog->addConditionInterface (
                  ent->uid (), ent->id (),
                  util::icon (ent->structuralType ()));
            }
          }

          _dialog->setMode (entityLink->property (ST_ATTR_REF_XCONNECTOR_ID),
                            "", "", StructuralLinkDialog::CreateCondition);

          if (_dialog->exec ())
          {
            if (!_dialog->getConditionInterface ().isEmpty ())
            {
              QString uid = _dialog->getConditionInterface ();

              if (_scene->hasEntity (uid))
              {
                entityNonLink = _scene->entity (uid);
                parentNonLink = entityNonLink->structuralParent ();

                props[ST_ATTR_EDGE_ORIG] = entityNonLink->uid ();
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

        QStrMap params = _dialog->getConditionParams ();

        for (const QString &key : params.keys ())
        {
          if (!params.value (key).isEmpty ())
          {
            QString uid = util::createUid ();

            props.insert (QString (ST_ATTR_BINDPARAM_NAME) + ":" + uid, key);
            props.insert (QString (ST_ATTR_BINDPARAM_VALUE) + ":" + uid,
                          params.value (key));
          }
        }
      }

      if (entityLink && entityNonLink)
      {
        props[ST_ATTR_REF_LINK_UID] = entityLink->uid ();

        if (entityNonLink->category () == Structural::Interface)
        {
          props[ST_ATTR_REF_INTERFACE_ID] = entityNonLink->id ();
          props[ST_ATTR_REF_INTERFACE_UID] = entityNonLink->uid ();
          props[ST_ATTR_REF_COMPONENT_ID] = parentNonLink->id ();
          props[ST_ATTR_REF_COMPONENT_UID] = parentNonLink->uid ();
        }
        else
        {
          props[ST_ATTR_REF_COMPONENT_ID] = entityNonLink->id ();
          props[ST_ATTR_REF_COMPONENT_UID] = entityNonLink->uid ();
          //          props[ST_ATTR_REF_INTERFACE_ID] = "";
          //          props[ST_ATTR_REF_INTERFACE_UID] = "";
        }
      }

      QStrMap settings = util::createSettings (true, true);

      if (!code.isEmpty ())
        settings[ST_SETTINGS_CODE] = code;

      insert (uid, (parent != nullptr ? parent->uid () : ""), props, settings);
    }
  }
}

void
StructuralView::createReference (StructuralEntity *orig,
                                 StructuralEntity *dest)
{
  if (orig->structuralType () == Structural::Port
      || orig->structuralType () == Structural::SwitchPort)
  {
    StructuralEntity *parentTail = orig->structuralParent ();
    StructuralEntity *parentHead = dest->structuralParent ();

    if (parentTail == parentHead
        || (parentHead && parentTail == parentHead->structuralParent ()))
    {
      QStrMap previous = orig->properties ();
      QStrMap props;

      if (orig->structuralType () == Structural::Port)
        props = orig->properties ();

      if (dest->category () == Structural::Interface)
      {
        props[ST_ATTR_REF_INTERFACE_ID] = dest->id ();
        props[ST_ATTR_REF_INTERFACE_UID] = dest->uid ();
        props[ST_ATTR_REF_COMPONENT_ID] = parentHead->id ();
        props[ST_ATTR_REF_COMPONENT_UID] = parentHead->uid ();
      }
      else
      {
        props[ST_ATTR_REF_COMPONENT_ID] = dest->id ();
        props[ST_ATTR_REF_COMPONENT_UID] = dest->uid ();
        //        props[ST_ATTR_REF_INTERFACE_ID] = "";
        //        props[ST_ATTR_REF_INTERFACE_UID] = "";
      }

      setMode (Structural::Pointing, true);

      if (orig->structuralType () == Structural::Port)
      {
        change (orig->uid (), props, util::createSettings (true, true));
      }
      else if (orig->structuralType () == Structural::SwitchPort)
      {
        props[ST_ATTR_ENT_TYPE] = util::typeToStr (Structural::Mapping);
        props[ST_ATTR_EDGE_ORIG] = orig->uid ();
        props[ST_ATTR_EDGE_DEST] = dest->uid ();

        insert (util::createUid (), parentTail->uid (), props,
                util::createSettings (true, true));
      }
    }
  }
}

void
StructuralView::deleteSelected ()
{
  CPR_ASSERT (_selected == "" || _scene->hasEntity (_selected));

  if (_selected != "")
    removeEntity (_selected, util::createSettings ());
  else
    qWarning (CPR_PLUGIN_STRUCT) << "Trying to delete, but no entity is "
                                    "selected";
}

void
StructuralView::snapshot ()
{
  qreal top = _scene->height ();
  qreal left = _scene->width ();
  qreal bottom = 0.0;
  qreal right = 0.0;

  for (StructuralEntity *ent : _scene->entities ().values ())
  {
    if (ent->structuralParent ())
    {
      if (ent->top () < top)
        top = ent->top ();

      if (ent->left () < left)
        left = ent->left ();

      if (ent->left () + ent->width () > right)
        right = ent->left () + ent->width ();

      if (ent->top () + ent->height () > bottom)
        bottom = ent->top () + ent->height ();
    }
  }

  StructuralEntity *selected = nullptr;
  if (_scene->hasEntity (_selected))
    selected = _scene->entity (_selected);

  if (selected)
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

      if (selected)
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
StructuralView::mouseMoveEvent (QMouseEvent *evt)
{
  if (_linking)
  {
    if (_linkingTail)
      _linkingTail->setHovering (false);

    if (_linkingHead)
      _linkingHead->setHovering (false);

    QList<QGraphicsItem *> origItems = _scene->items (_tool->getLine ().p1 ());
    if (origItems.count () && origItems.first () == _tool)
      origItems.removeFirst ();

    QList<QGraphicsItem *> destItems = _scene->items (_tool->getLine ().p2 ());
    if (destItems.count () && destItems.first () == _tool)
      destItems.removeFirst ();

    if (origItems.count ())
    {
      _linkingTail = cast (StructuralEntity *, origItems.first ());
      if (_linkingTail)
        _linkingTail->setHovering (true);
    }

    if (destItems.count ())
    {
      _linkingHead = cast (StructuralEntity *, destItems.first ());
      if (_linkingHead)
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
    if (_tool)
      delete _tool;

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
      select ("", util::createSettings ());
    }
  }
}

void
StructuralView::mouseReleaseEvent (QMouseEvent *evt)
{
  if (_linking)
  {
    if (_linkingTail)
      _linkingTail->setHovering (false);

    if (_linkingHead)
      _linkingHead->setHovering (false);

    QList<QGraphicsItem *> tailItems = _scene->items (_tool->getLine ().p1 ());

    if (tailItems.count () && tailItems.first () == _tool)
      tailItems.removeFirst ();

    QList<QGraphicsItem *> headtems = _scene->items (_tool->getLine ().p2 ());

    if (headtems.count () && headtems.first () == _tool)
      headtems.removeFirst ();

    if (tailItems.count () && headtems.count ())
    {
      StructuralEntity *orig = (StructuralEntity *)tailItems.first ();
      StructuralEntity *dest = (StructuralEntity *)headtems.first ();

      if (orig != dest)
      {
        // if linking NODE to NODE
        if (orig->category () == Structural::Node
            && dest->category () == Structural::Node)
        {
          if (orig->structuralType () != Structural::Link
              && dest->structuralType () != Structural::Link)
            createLink (orig, dest);
          else if (orig->structuralType () == Structural::Link)
            createBind (orig, dest);
          else if (dest->structuralType () == Structural::Link)
            createBind (orig, dest);
        }
        // if linking NODE to INTERFACE
        else if (orig->category () == Structural::Node
                 && dest->category () == Structural::Interface)
        {
          if (orig->structuralType () != Structural::Link)
            createLink (orig, dest);
          else
            createBind (orig, dest);
        }
        // if linking INTERFACE to NODE
        else if (orig->category () == Structural::Interface
                 && dest->category () == Structural::Node)
        {
          StructuralEntity *parentTail = orig->structuralParent ();
          StructuralEntity *parentHead = dest->structuralParent ();

          if (parentTail == parentHead
              && (orig->structuralType () == Structural::Port
                  || orig->structuralType () == Structural::SwitchPort))
            createReference (orig, dest);
          else if (dest->structuralType () != Structural::Link)
            createLink (orig, dest);
          else
            createBind (orig, dest);
        }
        // if linking INTERFACE to INTERFACE
        else if (orig->category () == Structural::Interface
                 && dest->category () == Structural::Interface)
        {

          StructuralEntity *parentTail = orig->structuralParent ();
          StructuralEntity *parentHead = dest->structuralParent ();

          if (parentHead && parentTail == parentHead->structuralParent ())
            createReference (orig, dest);
          else
            createLink (orig, dest);
        }
      }
    }

    if (_tool)
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
    deleteSelected ();
    evt->accept ();
  }
  else if (evt->key () == Qt::Key_Shift)
  {
    toogleMode (true);
    evt->accept ();
  }
  else if (evt->key () == Qt::Key_Control)
  {
    for (StructuralEntity *e : _scene->entities ().values ())
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
    toogleMode (true);
    evt->accept ();
  }
  else if (evt->key () == Qt::Key_Control)
  {
    for (StructuralEntity *e : _scene->entities ().values ())
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

  if (!ST_OPT_WITH_BODY)
  {
    if (!evt->isAccepted ())
    {
      QList<QUrl> list = evt->mimeData ()->urls ();
      StructuralType type = util::strToType (evt->mimeData ()->objectName ());

      if (!list.isEmpty () || util::validateKinship (type, Structural::Body))
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

  if (!ST_OPT_WITH_BODY)
  {
    if (!evt->isAccepted ())
    {
      QList<QUrl> list = evt->mimeData ()->urls ();
      StructuralType type = util::strToType (evt->mimeData ()->objectName ());

      if (!list.isEmpty () || util::validateKinship (type, Structural::Body))
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

  if (!ST_OPT_WITH_BODY)
  {
    if (evt->isAccepted ())
    {
      QList<QUrl> list = evt->mimeData ()->urls ();
      StructuralType type = util::strToType (evt->mimeData ()->objectName ());

      if (!list.isEmpty ())
      {
        for (const QUrl &url : list)
        {
          QString filename = url.toLocalFile ();
          QPointF p = mapToScene (evt->pos ());

          QStrMap props
              = { { ST_ATTR_ENT_TYPE, util::typeToStr (Structural::Media) },
                  { ST_ATTR_ENT_ID,
                    util::formatId (QFileInfo (filename).baseName ()) },
                  { ST_ATTR_NODE_SRC, filename },
                  { ST_ATTR_ENT_TOP,
                    QString::number (p.y () - ST_DEFAULT_CONTENT_H / 2) },
                  { ST_ATTR_ENT_LEFT,
                    QString::number (p.x () - ST_DEFAULT_CONTENT_W / 2) } };

          insert (util::createUid (), "", props, util::createSettings ());
        }
      }
      else if (util::validateKinship (type, Structural::Body))
      {
        QPointF p = mapToScene (evt->pos ());
        QStrMap props = { { ST_ATTR_ENT_TOP, QString::number (p.y ()) },
                          { ST_ATTR_ENT_LEFT, QString::number (p.x ()) } };

        move (evt->mimeData ()->text (), "", props, util::createSettings ());
      }
    }
  }
}

void
StructuralView::clean ()
{
  select ("", util::createSettings ());

  // todo: the following code should be in StructuralScene
  QStrMap settings = util::createSettings (true, false);
  settings.insert (ST_SETTINGS_UNDO_TRACE, "0");
  while (!_scene->entities ().empty ())
  {
    QString uid = _scene->entities ().keys ().at (0);
    remove (uid, settings);
  }
  // endtodo

  _scene->clear ();
  _commands.clear (); // why?
}

void
StructuralView::performLinkDialog (StructuralLink *ent)
{
  emit updateRequested ();

  _dialog->setMode (ent->property (ST_ATTR_REF_XCONNECTOR_ID), "", "",
                    StructuralLinkDialog::EditLink);

  QStrMap pLink;
  for (const QString &name : ent->properties ().keys ())
  {
    if (name.contains (ST_ATTR_LINKPARAM_NAME))
    {
      QString lpUid = name.right (name.length () - name.lastIndexOf (':') - 1);
      QString lpName = ent->properties ().value (name);
      QString lpValue = ent->properties ().value (
          QString (ST_ATTR_LINKPARAM_VALUE) + ":" + lpUid);

      pLink.insert (lpName, lpValue);
    }
  }

  _dialog->setConnectorParams (pLink);

  if (_dialog->exec ())
  {
    QStrMap prev = ent->properties ();
    QStrMap props = ent->properties ();

    props.insert (ST_ATTR_REF_XCONNECTOR_ID, _dialog->getConnector ());

    QStrMap p = _dialog->getConnectorParams ();

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
        QString uid = util::createUid ();

        props.insert (QString (ST_ATTR_LINKPARAM_NAME) + ":" + uid, name);
        props.insert (QString (ST_ATTR_LINKPARAM_VALUE) + ":" + uid,
                      p.value (name));
      }
    }

    QStrMap settings = util::createSettings (true, true);

    change (ent->uid (), props, settings);
  }
}

void
StructuralView::performBindDialog (StructuralBind *ent)
{
  QStrMap pBind;
  for (const QString &name : ent->properties ().keys ())
  {
    if (name.contains (ST_ATTR_BINDPARAM_NAME))
    {
      QString bpUid = name.right (name.length () - name.lastIndexOf (':') - 1);
      QString bpName = ent->properties ().value (name);
      QString bpValue = ent->properties ().value (
          QString (ST_ATTR_BINDPARAM_VALUE) + ":" + bpUid);

      pBind.insert (bpName, bpValue);
    }
  }

  emit updateRequested ();

  if (util::isCondition (ent->getRole ()))
  {
    _dialog->setMode (
        ent->destination ()->property (ST_ATTR_REF_XCONNECTOR_ID), "", "",
        StructuralLinkDialog::EditCondition);

    _dialog->setCondition (ent->id ());
    _dialog->setConditionParams (pBind);

    if (ent->origin ()->category () == Structural::Interface)
    {
      for (StructuralEntity *e :
           ent->origin ()->structuralParent ()->children ())
      {
        _dialog->addConditionInterface (e->uid (), e->id (),
                                        util::icon (e->structuralType ()));
      }

      _dialog->setConditionInterface (
          ent->property (ST_ATTR_REF_INTERFACE_ID));
    }
    else
    {
      for (StructuralEntity *e : ent->origin ()->children ())
      {
        _dialog->addConditionInterface (e->uid (), e->id (),
                                        util::icon (e->structuralType ()));
      }

      if (!ST_OPT_SHOW_INTERFACES)
      {
        if (ent->hasProperty (ST_ATTR_REF_INTERFACE_UID)
            && _scene->hasEntity (ent->property (ST_ATTR_REF_INTERFACE_UID)))
        {
          StructuralEntity *interface = _scene->entity (
              ent->property (ST_ATTR_REF_INTERFACE_UID));

          _dialog->setConditionInterface (interface->id ());
        }
      }
    }
  }
  else if (util::isAction (ent->getRole ()))
  {
    _dialog->setMode (ent->origin ()->property (ST_ATTR_REF_XCONNECTOR_ID), "",
                      "", StructuralLinkDialog::EditAction);

    _dialog->setAction (ent->id ());
    _dialog->setActionParams (pBind);

    if (ent->destination ()->category () == Structural::Interface)
    {
      for (StructuralEntity *e :
           ent->destination ()->structuralParent ()->children ())
      {
        _dialog->addActionInterface (e->uid (), e->id (),
                                     util::icon (e->structuralType ()));
      }

      _dialog->setActionInterface (ent->property (ST_ATTR_REF_INTERFACE_ID));
    }
    else
    {
      for (StructuralEntity *e : ent->destination ()->children ())
      {
        _dialog->addActionInterface (e->uid (), e->id (),
                                     util::icon (e->structuralType ()));
      }

      if (!ST_OPT_SHOW_INTERFACES)
      {
        if (ent->hasProperty (ST_ATTR_REF_INTERFACE_UID)
            && _scene->hasEntity (ent->property (ST_ATTR_REF_INTERFACE_UID)))
        {
          StructuralEntity *interface = _scene->entity (
              ent->property (ST_ATTR_REF_INTERFACE_UID));

          _dialog->setActionInterface (interface->id ());
        }
      }
    }
  }

  if (_dialog->exec ())
  {
    QStrMap prev = ent->properties ();
    QStrMap props = ent->properties ();

    QString role, property, interface;
    QStrMap p;

    if (util::isCondition (ent->getRole ()))
    {
      role = _dialog->getCondition ();
      property = ST_ATTR_EDGE_ORIG;
      interface = _dialog->getConditionInterface ();
      p = _dialog->getConditionParams ();
    }
    else if (util::isAction (ent->getRole ()))
    {
      role = _dialog->getAction ();
      property = ST_ATTR_EDGE_DEST;
      interface = _dialog->getActionInterface ();
      p = _dialog->getActionParams ();
    }

    props[ST_ATTR_BIND_ROLE] = role;
    props[ST_ATTR_ENT_ID] = role;

    if (!interface.isEmpty () && _scene->hasEntity (interface))
    {
      props[property] = interface;
      props[ST_ATTR_REF_INTERFACE_UID] = interface;
      props[ST_ATTR_REF_INTERFACE_ID] = _scene->entity (interface)->id ();
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
        QString uid = util::createUid ();

        props.insert (QString (ST_ATTR_BINDPARAM_NAME) + ":" + uid, name);
        props.insert (QString (ST_ATTR_BINDPARAM_VALUE) + ":" + uid,
                      p.value (name));
      }
    }

    QStrMap settings = util::createSettings (true, true);

    changeEntity (ent->uid (), props, prev, settings);
  }
}

void
StructuralView::pasteEntity (StructuralEntity *ent, StructuralEntity *parent,
                             const QString &code, bool adjust)
{
  CPR_ASSERT_NON_NULL (ent);

  QString uid = util::createUid ();
  QStrMap props = ent->properties ();
  props[ST_ATTR_ENT_UID] = uid;

  if (adjust)
  {
    props.remove (ST_ATTR_ENT_TOP);
    props.remove (ST_ATTR_ENT_LEFT);
  }

  QStrMap setting = util::createSettings (ST_VALUE_TRUE, ST_VALUE_TRUE, code);

  insert (uid, (parent != nullptr ? parent->uid () : ""), props, setting);

  if (_scene->hasEntity (uid))
  {
    _clipboardRefs[ent->uid ()] = uid;

    for (StructuralEntity *e : ent->children ())
    {
      if (e->category () != Structural::Edge
          && e->structuralType () != Structural::Port
          && e->structuralType () != Structural::Link)
      {

        pasteEntity (e, _scene->entity (uid), code, false);
      }
    }

    for (StructuralEntity *e : ent->children ())
    {
      if (e->category () == Structural::Edge
          || e->structuralType () == Structural::Port
          || e->structuralType () == Structural::Link)
      {

        QStrMap p = e->properties ();
        auto update_if_contains = [&](const QString &name) {
          if (p.contains (name) && _clipboardRefs.contains (p.value (name)))
            e->setProperty (name, _clipboardRefs.value (p.value (name)));
        };

        update_if_contains (ST_ATTR_EDGE_ORIG);
        update_if_contains (ST_ATTR_EDGE_DEST);
        update_if_contains (ST_ATTR_REF_COMPONENT_UID);
        update_if_contains (ST_ATTR_REF_INTERFACE_UID);
        update_if_contains (ST_ATTR_REF_LINK_UID);
        update_if_contains (ST_ATTR_REF_XCONNECTOR_UID);

        for (const QString &k : p.keys ())
        {
          if (k.contains (ST_ATTR_LINKPARAM_NAME)
              || k.contains (ST_ATTR_LINKPARAM_VALUE)
              || k.contains (ST_ATTR_BINDPARAM_NAME)
              || k.contains (ST_ATTR_BINDPARAM_VALUE))
          {
            QString name = k.left (k.length () - k.lastIndexOf (':') - 1);
            e->setProperty (name + ":" + util::createUid (), p.value (k));
            e->setProperty (k, "");
          }
        }

        pasteEntity (e, _scene->entity (uid), code, false);
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

  if (_scene->hasEntity (_selected))
    entity = _scene->getEntity (_selected);

  adjustLayout (entity, util::createUid ());
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
    foreach (StructuralEntity *current, _scene->getEntities ().values ())
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

  qreal nextTop, nextLeft, nextWidth, nextHeight;

  if (entity)
  {
    nextTop = entity->getTop ();
    nextLeft = entity->getLeft ();
    nextWidth = GD_bb (g).UR.x + PADDING;
    nextHeight = GD_bb (g).UR.y + PADDING;

    change (entity->getStructuralUid (),
            util::createProperties (nextTop, nextLeft, nextWidth, nextHeight),
            util::createSettings (ST_VALUE_TRUE, ST_VALUE_FALSE, code));
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
    StructuralEntity *e = _scene->getEntity (key);

    nextTop = (GD_bb (g).UR.y - ND_coord (n).y) + parentHeight / 2
              - GD_bb (g).UR.y / 2 - e->getHeight () / 2;
    nextLeft = ND_coord (n).x + parentWidth / 2 - GD_bb (g).UR.x / 2
               - e->getWidth () / 2;

    QStringMap properties = e->getStructuralProperties ();
    properties[ST_ATTR_ENT_TOP] = QString::number (nextTop);
    properties[ST_ATTR_ENT_LEFT] = QString::number (nextLeft);

    change (e->getStructuralUid (), properties,
            util::createSettings (ST_VALUE_TRUE, ST_VALUE_FALSE, code));
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
           util::createProperties(nextTop, nextLeft),
           link->getStructuralProperties(),
           util::createSettings(ST_VALUE_TRUE, ST_VALUE_FALSE,
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

          QStringMap properties = link->getStructuralProperties ();
          properties[ST_ATTR_ENT_TOP] = QString::number (nextTop);
          properties[ST_ATTR_ENT_LEFT] = QString::number (nextLeft);

          change (link->getStructuralUid (), properties,
                  util::createSettings (ST_VALUE_TRUE, ST_VALUE_FALSE, code));

          break;
        }
      }
    }
  }
}
#endif
