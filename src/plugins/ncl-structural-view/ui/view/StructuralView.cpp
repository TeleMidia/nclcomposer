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
StructuralView::getScene ()
{
  return _scene;
}

StructuralLinkDialog *
StructuralView::getDialog ()
{
  return _dialog;
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
  CPR_ASSERT (_scene->hasEntity (uid));

  StructuralEntity *ent = _scene->getEntity (uid);
  ent->setError (error);
  ent->adjust (true);
}

void
StructuralView::clearErrors ()
{
  for (StructuralEntity *ent : _scene->getEntities ().values ())
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
StructuralView::insert (QString uid, QString parent,
                        QMap<QString, QString> props,
                        QMap<QString, QString> settings)
{
  CPR_ASSERT (!_scene->hasEntity (uid));

  StructuralType type = StructuralUtil::strToType (props[ST_ATTR_ENT_TYPE]);
  StructuralEntity *e = _scene->createEntity (type);
  CPR_ASSERT_NON_NULL (e);

  StructuralEntity *p = nullptr;
  if (_scene->hasEntity (parent))
    p = _scene->getEntity (parent);

  if (type != StructuralType::Body && p == nullptr)
    CPR_ASSERT_NOT_REACHED ();

  e->setUid (uid);

  if (!props.contains (ST_ATTR_ENT_ID))
    e->setId (_scene->createNewId (e->getType ()));

  if (p)
    e->setParent (p);
  else
    _scene->addItem (e);

  if (!props.contains (ST_ATTR_ENT_TOP))
  {
    if (p)
      e->setTop (p->getHeight () / 2 - e->getHeight () / 2);
    else
    {
      auto rect = this->mapToScene (this->viewport ()->rect ().center ());
      e->setTop (rect.y () - e->getHeight () / 2);
    }
  }

  if (!props.contains (ST_ATTR_ENT_LEFT))
  {
    if (p)
      e->setLeft (p->getWidth () / 2 - e->getWidth () / 2);
    else
    {
      auto rect = this->mapToScene (this->viewport ()->rect ().center ());
      e->setLeft (rect.x () - e->getWidth () / 2);
    }
  }

  e->setMenu (_menu);
  e->setProperties (props);

  _scene->getEntities ()[uid] = e;

  // Adjust 'compositions'
  if (p)
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
    adjustReferences (e);

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
    if (e->getCategory () == Structural::Interface && p != nullptr
        && p->getType () == Structural::Media)
    {
      if (p->isReference ())
        adjustReferences (p);
      else
        for (const QString &key : _scene->getRefs ().keys ())
        {
          if (_scene->getRefs ().contains (key)
              && _scene->getRefs ().value (key) == p->getUid ()
              && _scene->hasEntity (key))
          {
            adjustReferences (_scene->getEntity (key));
          }
        }
    }
  }

  connect (e, &StructuralEntity::insertAsked, this, &StructuralView::insert);
  connect (e, &StructuralEntity::changeAsked, this,
           &StructuralView::changeEntity);
//  connect (e, &StructuralEntity::removed, this, &StructuralView::remove);
  connect (e, &StructuralEntity::selectAsked, this, &StructuralView::select);
  connect (e, &StructuralEntity::moveAsked, this, &StructuralView::move);

  if (settings[ST_SETTINGS_NOTIFY] == ST_VALUE_TRUE)
  {
    emit inserted (uid, parent, e->getProperties (), settings);
  }

  if (!ST_OPT_SHOW_INTERFACES)
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

  if (type == StructuralType::Body)
    emit canAddBody(false);
}

qreal
StructuralView::getNewAngle (StructuralBind *bind)
{
  int min = 100000;
  int max = -100000;

  qreal angle = 0;

  if (bind->getTail () != nullptr && bind->getHead () != nullptr)
  {
    for (StructuralEntity *e : _scene->getEntities ().values ())
    {
      if (e->getType () == Structural::Bind && e != bind)
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
StructuralView::remove (QString uid, QMap<QString, QString> stgs)
{
  CPR_ASSERT (_scene->hasEntity (uid));

  StructuralEntity *e = _scene->getEntity (uid);
  StructuralEntity *p = e->getParent ();
  StructuralType type = e->getType ();

  // Removing 'references'...
  if (e->getCategory () == Structural::Interface)
  {
    for (const QString &key : _scene->getRefs ().keys (e->getUid ()))
      remove (key,
              StructuralUtil::createSettings (ST_VALUE_FALSE, ST_VALUE_FALSE,
                                              stgs.value (ST_SETTINGS_CODE)));

    // Only remove interface -> interface references. Keeping
    // Media -> Media references to enable undo.  All "trash" references
    // are ignore when saving the project.
    _scene->getRefs ().remove (e->getUid ());
  }
  else if (e->getType () == Structural::Media)
  {
    for (const QString &key : _scene->getRefs ().keys (e->getUid ()))
    {
      if (_scene->hasEntity (key))
      {
        _scene->getEntity (key)->setReference (false);
        _scene->getEntity (key)->adjust (true);
      }
    }
  }

  // Removing 'children'...
  if (stgs.value (ST_SETTINGS_UNDO_TRACE) != ST_VALUE_FALSE)
    stgs[ST_SETTINGS_UNDO] = ST_VALUE_TRUE;

  while (!e->getChildren ().isEmpty ())
    removeEntity (e->getChildren ().first ()->getUid (), stgs);

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
            removeEntity (edge->getUid (),
                          StructuralUtil::createSettings (
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
              auto previous = edge->getTail ()->getProperties ();
              auto props = edge->getTail ()->getProperties ();

              props[ST_ATTR_REFERENCE_COMPONENT_ID] = "";
              props[ST_ATTR_REFERENCE_COMPONENT_UID] = "";
              props[ST_ATTR_REFERENCE_INTERFACE_ID] = "";
              props[ST_ATTR_REFERENCE_INTERFACE_UID] = "";

              change (edge->getTail ()->getUid (), props, previous,
                      StructuralUtil::createSettings (
                          ST_VALUE_TRUE, stgs.value (ST_SETTINGS_NOTIFY),
                          stgs.value (ST_SETTINGS_CODE)));
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
          if (stgs[ST_SETTINGS_NOTIFY] == ST_VALUE_TRUE)
          {
            emit removed (
                key.right (key.length () - key.lastIndexOf (':') - 1), stgs);
          }
        }
      }
    }

    if (!ST_OPT_WITH_BODY && !ST_OPT_USE_FLOATING_INTERFACES)
    {
      if (e->getType () == Structural::Port)
      {
        StructuralEntity *target = nullptr;

        QString compUid = e->getProperty (ST_ATTR_REFERENCE_COMPONENT_UID);
        QString interfUid = e->getProperty (ST_ATTR_REFERENCE_INTERFACE_UID);
        if (_scene->hasEntity (compUid))
        {
          target = _scene->getEntity (compUid);

          if (_scene->hasEntity (interfUid))
            target = _scene->getEntity (interfUid);
        }

        if (target != nullptr)
          target->setProperty (ST_ATTR_ENT_AUTOSTART, ST_VALUE_FALSE);
      }
    }

    // Removing 'others'...
    if (p)
      p->removeChild (e);
    else
      _scene->removeItem (e);

    _scene->getEntities ().remove (uid);
    delete e;

    if (uid == _selected)
      emit select ("", stgs);

    if (type == Structural::Body)
      emit canAddBody (true);

    if (stgs[ST_SETTINGS_NOTIFY] == ST_VALUE_TRUE)
      emit removed (uid, stgs);
  }
}

void
StructuralView::change (QString uid, QMap<QString, QString> props,
                        QMap<QString, QString> prev,
                        QMap<QString, QString> settings)
{
  CPR_ASSERT (_scene->hasEntity (uid));

  StructuralEntity *ent = _scene->getEntity (uid);
  auto *comp = dynamic_cast<StructuralComposition *> (ent);

  if (ent->isUncollapsed ()
      && props.value (ST_ATTR_ENT_UNCOLLAPSED) == ST_VALUE_FALSE)
  {
    comp->collapse ();
  }

  if (!ent->isUncollapsed ()
      && props.value (ST_ATTR_ENT_UNCOLLAPSED) == ST_VALUE_TRUE)
  {
    comp->collapse ();
  }

  if (!ST_OPT_WITH_BODY && !ST_OPT_USE_FLOATING_INTERFACES)
  {
    if (ent->getType () == Structural::Port)
    {
      StructuralEntity *lasttarget = nullptr;
      QString compUid = prev.value (ST_ATTR_REFERENCE_COMPONENT_UID);
      QString interfUid = prev.value (ST_ATTR_REFERENCE_INTERFACE_UID);
      if (_scene->hasEntity (compUid))
      {
        lasttarget = _scene->getEntity (compUid);

        if (_scene->hasEntity (interfUid))
          lasttarget = _scene->getEntity (interfUid);
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
    emit changed (uid, props, prev, settings);
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
      // Adjusting others media entities that refer the current one
      bool hasChange = false;

      if (!ent->getProperty (ST_ATTR_REFERENCE_REFER_ID).isEmpty ())
      {
        QString referUid = ent->getProperty (ST_ATTR_REFERENCE_REFER_UID);
        if (_scene->hasEntity (referUid))
        {
          StructuralEntity *refer = _scene->getEntity (referUid);

          if (ent != refer && !refer->isReference ())
          {
            if (refer->getType () == Structural::Media)
            {
              QString instance = "new";

              if (!ent->getProperty (ST_ATTR_NODE_INSTANCE).isEmpty ())
                instance = ent->getProperty (ST_ATTR_NODE_INSTANCE);

              for (StructuralEntity *child : ent->getChildren ())
                if (child->isReference ())
                  if (_scene->hasEntity (
                          child->getProperty (ST_ATTR_REFERENCE_REFER_UID)))
                    if (_scene
                            ->getEntity (child->getProperty (
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
                            _scene->getRefs ().value (child->getUid ())))
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

                      _scene->getRefs ()[uid] = child->getUid ();

                      QMap<QString, QString> settings
                          = StructuralUtil::createSettings (false, false);
                      settings[ST_SETTINGS_ADJUST_REFERENCE] = ST_VALUE_FALSE;

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
                         _scene->getRefs ().keys (child->getUid ()))
                      remove (key,
                              StructuralUtil::createSettings (false, false));
                  }
              }
              else if (instance == "instSame" || instance == "gradSame")
              {
                for (StructuralEntity *child : ent->getChildren ())
                {
                  if (!child->isReference ())
                  {
                    for (const QString &key :
                         _scene->getRefs ().keys (child->getUid ()))
                    {
                      if (_scene->hasEntity (key))
                      {
                        if (_scene->getEntity (key)->getParent () != refer)
                        {
                          remove (key, StructuralUtil::createSettings (false,
                                                                       false));
                        }
                      }
                    }
                  }
                }

                bool hasNewEntity = false;

                for (StructuralEntity *child : ent->getChildren ())
                {
                  if (child->getCategory () == Structural::Interface)
                  {
                    if (!childToRefer.contains (child->getUid ()))
                    {
                      if (!_scene->getRefs ().values ().contains (
                              child->getUid ()))
                      {
                        QString uid = StructuralUtil::createUid ();

                        QMap<QString, QString> props = child->getProperties ();
                        props[ST_ATTR_ENT_UID] = uid;
                        props[ST_ATTR_ENT_REFERENCE] = ST_VALUE_TRUE;

                        props.remove (ST_ATTR_ENT_TOP);
                        props.remove (ST_ATTR_ENT_LEFT);

                        props[ST_ATTR_REFERENCE_REFER_ID] = child->getId ();
                        props[ST_ATTR_REFERENCE_REFER_UID] = child->getUid ();

                        _scene->getRefs ()[uid] = child->getUid ();

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
                       _scene->getRefs ().keys (refer->getUid ()))
                    if (_scene->hasEntity (key))
                      if (key != ent->getUid ())
                        adjustReferences (_scene->getEntity (key));
              }

              _scene->getRefs ()[ent->getUid ()] = refer->getUid ();
              ent->setReference (true);
              hasChange = true;
            }
          }
        }
      }

      if (_scene->getRefs ().contains (ent->getUid ()))
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
              for (const QString &key :
                   _scene->getRefs ().keys (child->getUid ()))
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

            if (!bind->getProperty (ST_ATTR_REFERENCE_COMPONENT_ID).isEmpty ()
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

          if (!ST_OPT_SHOW_INTERFACES)
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
      if (_scene->hasEntity (ent->getProperty (ST_ATTR_EDGE_TAIL)))
        ((StructuralEdge *)ent)
            ->setTail (
                _scene->getEntity (ent->getProperty (ST_ATTR_EDGE_TAIL)));

      if (_scene->hasEntity (ent->getProperty (ST_ATTR_EDGE_HEAD)))
        ((StructuralEdge *)ent)
            ->setHead (
                _scene->getEntity (ent->getProperty (ST_ATTR_EDGE_HEAD)));

      bool isVisible = true;

      StructuralEntity *tail = ((StructuralEdge *)ent)->getTail ();
      StructuralEntity *head = ((StructuralEdge *)ent)->getHead ();

      if (tail != nullptr && head != nullptr)
      {
        if (!ST_OPT_SHOW_INTERFACES)
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

          if (_scene->hasEntity (
                  ent->getProperty (ST_ATTR_REFERENCE_INTERFACE_UID)))
            interface = _scene->getEntity (
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

          if (_scene->hasEntity (
                  ent->getProperty (ST_ATTR_REFERENCE_COMPONENT_UID)))
            component = _scene->getEntity (
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

            if (!bind->getProperty (ST_ATTR_REFERENCE_INTERFACE_ID).isEmpty ())
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
        if (ST_OPT_SHOW_INTERFACES)
        {
          for (StructuralEntity *e : _scene->getEntities ().values ())
            if (e->getType () == Structural::Reference)
              if (e->getProperty (ST_ATTR_EDGE_TAIL) == ent->getUid ())
                remove (e->getUid (),
                        StructuralUtil::createSettings (false, false));

          StructuralEntity *component = nullptr;
          StructuralEntity *interface = nullptr;

          if (_scene->hasEntity (
                  ent->getProperty (ST_ATTR_REFERENCE_INTERFACE_UID)))
            interface = _scene->getEntity (
                ent->getProperty (ST_ATTR_REFERENCE_INTERFACE_UID));

          if (_scene->hasEntity (
                  ent->getProperty (ST_ATTR_REFERENCE_COMPONENT_UID)))
            component = _scene->getEntity (
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

              if (!ST_OPT_WITH_BODY
                  && !ST_OPT_USE_FLOATING_INTERFACES)
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
        else if (!ST_OPT_USE_FLOATING_INTERFACES)
        {
          StructuralEntity *component = nullptr;
          StructuralEntity *interface = nullptr;
          QString compUid = ent->getProperty (ST_ATTR_REFERENCE_COMPONENT_UID);
          QString interUid = ent->getProperty (ST_ATTR_REFERENCE_INTERFACE_UID);

          if (_scene->hasEntity (compUid))
            component = _scene->getEntity (compUid);

          if (_scene->hasEntity (interUid))
            interface = _scene->getEntity (interUid);

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
  CPR_ASSERT (_scene->hasEntity (uid) || uid == "");

  if (_scene->hasEntity (uid))
  {
    if (uid != _selected)
    {
      if (_scene->hasEntity (_selected))
        _scene->getEntity (_selected)->setSelected (false);

      StructuralEntity *ent = _scene->getEntity (uid);
      ent->setSelected (true);
      _selected = ent->getUid ();
      CPR_ASSERT (ent->getUid () == uid);

      bool canPaste = false;

      if (_clipboard != nullptr)
      {
        if (ent->getUid () != _clipboard->getUid ())
        {
          if (StructuralUtil::validateKinship (_clipboard->getType (),
                                               ent->getType ()))
            if (!isChild (ent, _clipboard))
              canPaste = true;
        }
      }

      emit canCutChanged (true);
      emit canCopyChanged (true);
      emit canPasteChanged (canPaste);

      if (settings[ST_SETTINGS_NOTIFY] == ST_VALUE_TRUE)
      {
        emit selected (uid, settings);
      }
    }
  }
  else
  {
    _selected = ""; // unselect

    emit canCutChanged (false);
    emit canCopyChanged (false);

    bool enablePaste = false;

    if (!ST_OPT_WITH_BODY)
    {
      if (_clipboard != nullptr)
      {
        if (StructuralUtil::validateKinship (_clipboard->getType (),
                                             Structural::Body))
          enablePaste = true;
      }
    }

    emit canPasteChanged (enablePaste);

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
  CPR_ASSERT (_scene->hasEntity (uid));
  CPR_ASSERT (_scene->hasEntity (parent) || parent == "");

  StructuralEntity *e = _scene->getEntity (uid);
  StructuralEntity *p = _scene->getEntity (parent);
  StructuralType type = Structural::Body;

  if (p != nullptr)
    type = p->getType ();

  CPR_ASSERT (StructuralUtil::validateKinship (e->getType (), type));

  if (e->getParent () != p)
  {
    for (StructuralEntity *ent : _scene->getEntities ().values ())
      ent->setDraggable (false);

    select ("", settings);

    StructuralEntity *copy = _scene->clone (e);

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

  if (stgs.empty ())
    stgs = StructuralUtil::createSettings ();

  QString uid = StructuralUtil::createUid ();
  QString parent = "";

  if (_scene->hasEntity (_selected))
    parent = _selected;

  if (!props.contains (ST_ATTR_ENT_TYPE))
    props[ST_ATTR_ENT_TYPE] = StructuralUtil::typeToStr (type);

  if (!stgs.contains (ST_SETTINGS_UNDO))
    stgs[ST_SETTINGS_UNDO] = ST_VALUE_TRUE;

  if (!stgs.contains (ST_SETTINGS_NOTIFY))
    stgs[ST_SETTINGS_NOTIFY] = ST_VALUE_TRUE;

  if (!stgs.contains (ST_SETTINGS_CODE))
    stgs[ST_SETTINGS_CODE] = StructuralUtil::createUid ();

  Insert *cmd = new Insert (this, uid, parent, props, stgs);
  cmd->setText (stgs[ST_SETTINGS_CODE]);
  _commands.push (cmd);
}

void
StructuralView::changeEntity (QString uid, QMap<QString, QString> props,
                              QMap<QString, QString> prev,
                              QMap<QString, QString> stgs)
{
  Change *cmd = new Change (this, uid, props, prev, stgs);
  cmd->setText (stgs[ST_SETTINGS_CODE]);
  _commands.push (cmd);
}

void
StructuralView::removeEntity (QString uid, QMap<QString, QString> stgs)
{
  CPR_ASSERT (_scene->hasEntity (uid));

  StructuralEntity *e = _scene->getEntity (uid);
  StructuralEntity *p = e->getParent ();

  if (!e->isReference () || e->getCategory () != Structural::Interface)
  {
    QString parent = "";
    if (p)
      parent = p->getUid ();

    Remove *cmd
        = new Remove (this, e->getUid (), parent, e->getProperties (), stgs);
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

  StructuralEntity *e = _scene->getEntity (_selected);
  QMap<QString, QString> props = e->getProperties ();

  QMap<QString, QString> stgs = StructuralUtil::createSettings (true, true);

  // Case 1: media already has an autostart property
  if (props.contains (ST_ATTR_ENT_AUTOSTART)
      && props[ST_ATTR_ENT_AUTOSTART] == ST_VALUE_TRUE)
  {
    QVector<StructuralEntity *> neighbors = StructuralUtil::getNeighbors (e);

    if (ST_OPT_SHOW_INTERFACES)
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
            || (ST_OPT_SHOW_INTERFACES
                && neighbor->getProperty (ST_ATTR_REFERENCE_INTERFACE_UID)
                       == e->getUid ()))
        {
          remove (neighbor->getUid (), stgs);
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
    QMap<QString, QString> pProps
        = { { ST_ATTR_ENT_TYPE,
              StructuralUtil::typeToStr (Structural::Port) } };

    if (e->getCategory () == Structural::Interface)
    {
      if (e->getParent ())
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

    insert (StructuralUtil::createUid (), pParent, pProps, stgs);

    props[ST_ATTR_ENT_AUTOSTART] = ST_VALUE_TRUE;
  }

  change (e->getUid (), props, e->getProperties (),
          StructuralUtil::createSettings (true, true));
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

  for (const QString &key : _scene->getRefs ().keys ())
  {
    CPR_ASSERT (_scene->hasEntity (key));
    StructuralEntity *ent = _scene->getEntity (key);
    if (ent->getType () == Structural::Media)
      adjustReferences (ent);
  }

  for (const QString &key : _scene->getEntities ().keys ())
  {
    if (_scene->hasEntity (key))
    {
      StructuralEntity *ent = _scene->getEntity (key);

      if (ent->getCategory () == Structural::Edge)
      {
        StructuralEntity *c = _scene->getEntity (
            ent->getProperty (ST_ATTR_REFERENCE_COMPONENT_UID));
        StructuralEntity *i = _scene->getEntity (
            ent->getProperty (ST_ATTR_REFERENCE_INTERFACE_UID));

        if (c != nullptr && i != nullptr)
        {
          for (StructuralEntity *r : c->getChildren ())
          {
            if (r->getId ()
                == ent->getProperty (ST_ATTR_REFERENCE_INTERFACE_ID))
            {
              ent->setProperty (ST_ATTR_REFERENCE_INTERFACE_UID, r->getUid ());

              if (((StructuralEdge *)ent)->getTail () != nullptr)
                ent->setProperty (ST_ATTR_EDGE_HEAD, r->getUid ());
              else
                ent->setProperty (ST_ATTR_EDGE_TAIL, r->getUid ());

              adjustReferences (ent);
              ent->adjust (true);

              break;
            }
          }
        }
      }
    }
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
StructuralView::performCut ()
{
  performCopy ();

  StructuralEntity *ent = _scene->getEntity (_selected);

  if (ent)
  {
    _clipboard->setId (ent->getId ());

    if (_clipboard->getUid () == ent->getUid ())
      remove (_selected, StructuralUtil::createSettings ());
  }
}

void
StructuralView::performCopy ()
{
  if (_scene->hasEntity (_selected))
  {
    if (_clipboard)
    {
      delete _clipboard;
      _clipboardRefs.clear ();
    }

    StructuralEntity *ent = _scene->getEntity (_selected);
    if (ent != nullptr && ent->getCategory () != Structural::Edge
        && ent->getType () != Structural::Body)
    {
      _clipboard = _scene->clone (ent);
      _clipboard->setId ("c_" + _clipboard->getId ());

      emit canPasteChanged (false);
    }
  }
}

void
StructuralView::performPaste ()
{
  if (_clipboard != nullptr)
  {
    StructuralEntity *parent = nullptr;

    if (_scene->hasEntity (_selected))
      parent = _scene->getEntity (_selected);

    bool isAValidPaste = false;

    if (!ST_OPT_WITH_BODY)
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

      if (_scene->hasEntity (_clipboard->getUid ()))
        result = QMessageBox::question (
            this, tr ("Paste"),
            tr ("Would you like to make a reference instead?"), tr ("Cancel"),
            tr ("No"), tr ("Yes"), 1);

      if (result == 1)
      {
        QString nextID = _clipboard->getId ();

        int n = 0;

        for (StructuralEntity *entity : _scene->getEntities ().values ())
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

        for (StructuralEntity *entity : _scene->getEntities ().values ())
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
      for (StructuralEntity *e : _scene->getEntities ().values ())
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
      || !ST_OPT_WITH_BODY)
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

    if (parent != nullptr || !ST_OPT_WITH_BODY)
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

          if (_scene->hasEntity (uid))
          {
            tail = _scene->getEntity (uid);
            parentTail = tail->getParent ();

            ptail
                = parentTail->mapToParent (tail->getLeft (), tail->getTop ());
          }
        }

        if (!_dialog->getActionInterface ().isEmpty ())
        {
          QString uid = _dialog->getActionInterface ();

          if (_scene->hasEntity (uid))
          {
            head = _scene->getEntity (uid);
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

        if (_scene->hasEntity (uid))
        {
          createBind (tail, _scene->getEntity (uid), _dialog->getCondition (),
                      settings.value (ST_SETTINGS_CODE));
          createBind (_scene->getEntity (uid), head, _dialog->getAction (),
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
      || !ST_OPT_WITH_BODY)
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

    if (parent != nullptr || !ST_OPT_WITH_BODY)
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

              if (_scene->hasEntity (uid))
              {
                entityNonLink = _scene->getEntity (uid);
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

              if (_scene->hasEntity (uid))
              {
                entityNonLink = _scene->getEntity (uid);
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

void
StructuralView::performDelete ()
{
  CPR_ASSERT (_selected == "" || _scene->hasEntity (_selected));

  if (_selected != "")
    removeEntity (_selected, StructuralUtil::createSettings ());
}

void
StructuralView::snapshot ()
{
  qreal top = _scene->height ();
  qreal left = _scene->width ();
  qreal bottom = 0.0;
  qreal right = 0.0;

  for (StructuralEntity *ent : _scene->getEntities ().values ())
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

  if (_scene->hasEntity (_selected))
    selected = _scene->getEntity (_selected);

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
    for (StructuralEntity *e : _scene->getEntities ().values ())
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
    for (StructuralEntity *e : _scene->getEntities ().values ())
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

  if (!ST_OPT_WITH_BODY)
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

  if (!ST_OPT_WITH_BODY)
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

  // todo: the following code should be in StructuralScene
  for (const QString &uid : _scene->getEntities ().keys ())
  {
    QMap<QString, QString> settings
        = StructuralUtil::createSettings (true, false);
    settings[ST_SETTINGS_UNDO_TRACE] = "0";

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

      if (!ST_OPT_SHOW_INTERFACES)
      {
        if (!ent->getProperty (ST_ATTR_REFERENCE_INTERFACE_UID).isEmpty ()
            && _scene->hasEntity (
                   ent->getProperty (ST_ATTR_REFERENCE_INTERFACE_UID)))
        {
          StructuralEntity *interface = _scene->getEntity (
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

      if (!ST_OPT_SHOW_INTERFACES)
      {
        if (!ent->getProperty (ST_ATTR_REFERENCE_INTERFACE_UID).isEmpty ()
            && _scene->hasEntity (
                   ent->getProperty (ST_ATTR_REFERENCE_INTERFACE_UID)))
        {
          StructuralEntity *interface = _scene->getEntity (
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

    if (!interface.isEmpty () && _scene->hasEntity (interface))
    {
      props[property] = interface;
      props[ST_ATTR_REFERENCE_INTERFACE_UID] = interface;
      props[ST_ATTR_REFERENCE_INTERFACE_ID]
          = _scene->getEntity (interface)->getId ();
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

    changeEntity (ent->getUid (), props, prev, settings);
  }
}

bool
StructuralView::isChild (StructuralEntity *ent, StructuralEntity *parent)
{
  CPR_ASSERT (ent != parent);

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
  CPR_ASSERT_NON_NULL (ent);

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

  if (_scene->hasEntity (uid))
  {
    _clipboardRefs[ent->getUid ()] = uid;

    for (StructuralEntity *e : ent->getChildren ())
    {
      if (e->getCategory () != Structural::Edge
          && e->getType () != Structural::Port
          && e->getType () != Structural::Link)
      {

        paste (e, _scene->getEntity (uid), code, false);
      }
    }

    for (StructuralEntity *e : ent->getChildren ())
    {
      if (e->getCategory () == Structural::Edge
          || e->getType () == Structural::Port
          || e->getType () == Structural::Link)
      {

        QMap<QString, QString> p = e->getProperties ();
        auto update_if_contains = [&](const QString &name) {
          if (p.contains(name) && _clipboardRefs.contains (p.value (name)))
            e->setProperty (name, _clipboardRefs.value(p.value (name)));
        };

        update_if_contains (ST_ATTR_EDGE_TAIL);
        update_if_contains (ST_ATTR_EDGE_HEAD);
        update_if_contains (ST_ATTR_REFERENCE_COMPONENT_UID);
        update_if_contains (ST_ATTR_REFERENCE_INTERFACE_UID);
        update_if_contains (ST_ATTR_REFERENCE_LINK_UID);
        update_if_contains (ST_ATTR_REFERENCE_XCONNECTOR_UID);

        for (const QString &k : p.keys ())
        {
          if (k.contains (ST_ATTR_LINKPARAM_NAME)
              || k.contains (ST_ATTR_LINKPARAM_VALUE)
              || k.contains (ST_ATTR_BINDPARAM_NAME)
              || k.contains (ST_ATTR_BINDPARAM_VALUE))
          {
            QString name = k.left (k.length () - k.lastIndexOf (':') - 1);
            e->setProperty (name + ":" + StructuralUtil::createUid (),
                            p.value (k));
            e->setProperty (k, "");
          }
        }

        paste (e, _scene->getEntity (uid), code, false);
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
    StructuralEntity *e = _scene->getEntity (key);

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
