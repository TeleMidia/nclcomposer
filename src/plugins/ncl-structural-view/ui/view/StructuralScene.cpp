#include "StructuralScene.h"

#include "StructuralView.h"
#include "util/Utilities.h"

#include <set>
#include <vector>

StructuralScene::StructuralScene (StructuralView *view, StructuralMenu *menu,
                                  QObject *parent)
    : QGraphicsScene (parent), _view (view), _menu (menu)
{
  setSceneRect (0, 0, ST_DEFAULT_SCENE_W, ST_DEFAULT_SCENE_H);
}

void
StructuralScene::contextMenuEvent (QGraphicsSceneContextMenuEvent *event)
{
  QGraphicsScene::contextMenuEvent (event);

  if (!event->isAccepted ())
  {
    _menu->exec (event->screenPos ());
    event->accept ();
  }
}

bool
StructuralScene::hasEntity (const QString &uid)
{
  return entities ().contains (uid);
}

StructuralEntity *
StructuralScene::entity (const QString &uid)
{
  CPR_ASSERT (hasEntity (uid));
  return entities ().value (uid, nullptr);
}

QMap<QString, StructuralEntity *>
StructuralScene::entities ()
{
  QMap<QString, StructuralEntity *> entities = _nodes;

  for (const QString &k : _edges.keys ())
    entities.insert (k, _edges.value (k));

  return entities;
}

const QMap<QString, StructuralEntity *> &
StructuralScene::nodes ()
{
  return _nodes;
}

const QMap<QString, StructuralEdge *> &
StructuralScene::edges (void)
{
  return _edges;
}

/*!
 * \brief StructuralScene::edges gets the edges poiting that touchs on key.
 * \param key
 * \return
 */
QList<StructuralEdge *>
StructuralScene::edges (const QString &nodeuid)
{
  QList<StructuralEdge *> edges_of_k;
  for (StructuralEdge *e : _edges.values ())
  {
    if ((e->hasOrigin () && e->origin ()->uid () == nodeuid)
        || (e->hasDestination () && e->destination ()->uid () == nodeuid))
    {
      edges_of_k.append (e);
    }
  }
  return edges_of_k;
}

/*!
 * \brief StructuralScene::parallelEdges
 * \return the edges that have the same end nodes of edge
 */
QList<StructuralEdge *>
StructuralScene::parallelEdges (StructuralEdge *edge)
{
  CPR_ASSERT (edge->hasOrigin () && edge->hasOrigin ());
  QList<StructuralEdge *> parallel_edges;
  std::set<StructuralEntity *> edge_nodes
      = { edge->origin (), edge->destination () };

  for (StructuralEdge *e : edges ().values ())
  {
    if (e != edge && e->hasOrigin () && e->hasDestination ())
    {
      std::set<StructuralEntity *> e_nodes
          = { e->origin (), e->destination () };
      if (e_nodes == edge_nodes)
        parallel_edges.push_back (e);
    }
  }

  return parallel_edges;
}

QList<StructuralEntity *>
StructuralScene::entitiesByAttrId (const QString &id)
{
  QList<StructuralEntity *> list;
  for (StructuralEntity *e : entities ().values ())
  {
    if (e->id () == id)
      list.append (e);
  }

  return list;
}

QString
StructuralScene::createId (StructuralType type, const QString &customPrefix)
{
  QString prefix = customPrefix;
  if (prefix.isEmpty ())
    prefix = StructuralUtil::prefix (type); // get a default prefix

  CPR_ASSERT (!prefix.isEmpty ());

  QString id = "";
  for (int n = 0;; n++)
  {
    id = prefix + QString::number (n);
    if (entitiesByAttrId (id).size () == 0)
      break;
  }

  return id;
}

void
StructuralScene::clear ()
{
  QStrMap stngs = util::createSettings (true, false);
  stngs.insert (ST_SETTINGS_UNDO_TRACE, "0");

  // remove all the entities
  while (!entities ().empty ())
  {
    QString uid = entities ().keys ().at (0);
    remove (uid, stngs);
  }

  _refs.clear ();
  _nodes.clear ();
  _edges.clear ();

  QGraphicsScene::clear ();
}

StructuralEntity *
StructuralScene::body ()
{
  StructuralEntity *body = nullptr;
  for (StructuralEntity *ent : entities ().values ())
  {
    if (ent->structuralType () == Structural::Body)
    {
      body = ent;
      break;
    }
  }

  return body;
}

bool
StructuralScene::hasBody ()
{
  return body () != nullptr;
}

static QMap<QString, QString>
qdom_attrs_to_qmap (const QDomNamedNodeMap &attrs)
{
  QMap<QString, QString> map;
  for (int i = 0; i < attrs.count (); i++)
  {
    QDomAttr a = attrs.item (i).toAttr ();
    map.insert (a.name (), a.value ());
  }
  return map;
}

void
StructuralScene::load (const QString &data)
{
  QDomDocument doc;
  doc.setContent (data);

  QDomElement root = doc.documentElement ();
  CPR_ASSERT (root.tagName () == "structural");

  for_each_qelem_child (elt, root)
  {
    CPR_ASSERT (elt.nodeName () == "entity");
    load (elt, root);
  }
}

void
StructuralScene::load (QDomElement elt, QDomElement parent)
{
  CPR_ASSERT (!elt.attribute ("uid").isEmpty ());

  QString entUid = elt.attribute ("uid");
  QString parentUid = parent.attribute ("uid");
  QMap<QString, QString> props = qdom_attrs_to_qmap (elt.attributes ());

  auto stgs = StructuralUtil::createSettings (false, false);
  stgs[ST_SETTINGS_ADJUST_REFERS] = ST_VALUE_FALSE;

  insert (entUid, parentUid, props, stgs);

  for_each_qelem_child_of_type (child, elt, "entity") { load (child, elt); }
}

QString
StructuralScene::save ()
{
  QDomDocument *doc = new QDomDocument ();
  QDomElement root = doc->createElement ("structural");

  for (StructuralEntity *e : entities ().values ())
  {
    if (e->structuralParent () == nullptr)
      createXmlElement (e, doc, root);
  }

  doc->appendChild (root);

  return doc->toString (2);
}

void
StructuralScene::createXmlElement (StructuralEntity *ent, QDomDocument *doc,
                                   QDomElement parent)
{
  if (ent->structuralType () != Structural::Reference)
  {
    QDomElement elt = doc->createElement ("entity");
    CPR_ASSERT (!ent->uid ().isEmpty ());
    CPR_ASSERT (!util::typetostr (ent->structuralType ()).isEmpty ());

    elt.setAttribute ("uid", ent->uid ());
    elt.setAttribute ("type", util::typetostr (ent->structuralType ()));

    for (const QString &key : ent->properties ().keys ())
      elt.setAttribute (key, ent->property (key));

    for (StructuralEntity *e : ent->children ())
      createXmlElement (e, doc, elt);

    parent.appendChild (elt);
  }
}

void
StructuralScene::insertIntoMap (const QString &uid, StructuralEntity *ent)
{
  Structural::Category cat = ent->category ();
  if (cat == Structural::Node || cat == Structural::Interface)
  {
    CPR_ASSERT (!_nodes.contains (uid));
    _nodes.insert (uid, ent);
  }
  else if (cat == Structural::Edge)
  {
    CPR_ASSERT (!_edges.contains (uid));
    StructuralEdge *edge = cast (StructuralEdge *, ent);
    CPR_ASSERT_NON_NULL (edge);
    _edges.insert (uid, edge);
  }
  else
    CPR_ASSERT_NOT_REACHED ();
}

void
StructuralScene::removeFromMap (const QString &uid)
{
  bool contains = false;
  if (_nodes.contains (uid))
  {
    _nodes.remove (uid);
    contains = true;
  }
  else if (_edges.contains (uid))
  {
    _edges.remove (uid);
    contains = true;
  }

  CPR_ASSERT (contains);
}

StructuralEntity *
StructuralScene::createEntity (StructuralType type)
{
  StructuralEntity *e = nullptr;
  switch (type)
  {
    case Structural::Media:
      e = new StructuralContent ();
      break;

    case Structural::Body:
      CPR_ASSERT (body () == nullptr);
      e = new StructuralComposition ();
      e->setStructuralType (type);

      e->setWidth (ST_DEFAULT_BODY_W);
      e->setHeight (ST_DEFAULT_BODY_H);
      break;

    case Structural::Context:
    case Structural::Switch:
      e = new StructuralComposition ();
      e->setStructuralType (type);
      break;

    case Structural::Link:
      e = new StructuralLink ();
      e->setStructuralType (type);
      //    connect ((StructuralLink *)e, &StructuralLink::performedEdit, this,
      //             &StructuralView::performLinkDialog);
      break;

    case Structural::Area:
    case Structural::Property:
    case Structural::Port:
    case Structural::SwitchPort:
      e = new StructuralInterface ();
      e->setStructuralType (type);
      break;

    case Structural::Reference:
    case Structural::Mapping:
      e = new StructuralEdge ();
      e->setStructuralType (type);
      break;

    case Structural::Bind:
      e = new StructuralBind ();
      e->setStructuralType (type);
      //    connect ((StructuralBind *)e, &StructuralBind::performedEdit, this,
      //             &StructuralView::performBindDialog);
      break;

    default:
      CPR_ASSERT_NOT_REACHED ();
      break;
  }

  if (e)
    e->setMenu (_menu);

  return e;
}

StructuralEntity *
StructuralScene::clone (const StructuralEntity *ent,
                        StructuralEntity *newparent)
{
  StructuralEntity *newent = createEntity (ent->structuralType ());
  newent->setStructuralParent (newparent);
  newent->setProperties (ent->properties ());

  for (StructuralEntity *child : ent->children ())
    newent->addChild (this->clone (child, newent));

  return newent;
}

void
StructuralScene::updateWithDefaultProperties (StructuralEntity *e)
{
  StructuralEntity *p = e->structuralParent ();

  if (e->id ().isEmpty ())
    e->setId (createId (e->structuralType ()));

  if (p)
    e->setTop (p->height () / 2 - e->height () / 2);
  else
  {
    auto rect
        = views ().at (1)->mapToScene (views ().at (1)->rect ().center ());
    e->setTop (rect.y () - e->height () / 2);
  }

  if (p)
    e->setLeft (p->width () / 2 - e->width () / 2);
  else
  {
    auto rect
        = views ().at (1)->mapToScene (views ().at (1)->rect ().center ());
    e->setLeft (rect.x () - e->width () / 2);
  }
}

void
StructuralScene::insert (QString uid, QString parent, QStrMap props,
                         QStrMap stgs)
{
  CPR_ASSERT (!hasEntity (uid));

  // 1. Create the new entity.
  StructuralType type = util::strtotype (props[ST_ATTR_ENT_TYPE]);
  StructuralEntity *e = createEntity (type);
  CPR_ASSERT_NON_NULL (e);
  e->setUid (uid);

  // 2. Add to the scene and as child of p
  insertIntoMap (uid, e);

  StructuralEntity *p = nullptr;
  if (hasEntity (parent))
    p = entity (parent);

  if (type != StructuralType::Body && p == nullptr)
    CPR_ASSERT_NOT_REACHED ();

  e->setStructuralParent (p);
  if (p == nullptr)
    addItem (e);

  // 3. Update the properties of the new entity
  updateWithDefaultProperties (e);
  e->setProperties (props);

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
  _view->adjustEntity (e, props, stgs);
  if (stgs.value (ST_SETTINGS_ADJUST_REFERS, "") != ST_VALUE_FALSE)
    _view->adjustReferences (e);

  connect (e, &StructuralEntity::insertAsked, this, &StructuralScene::insert);
  connect (e, &StructuralEntity::changeAsked, _view,
           &StructuralView::changeEntity);
  connect (e, &StructuralEntity::selectAsked, _view, &StructuralView::select);
  connect (e, &StructuralEntity::moveAsked, _view, &StructuralView::move);

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

  //  if (type == StructuralType::Body)
  //    emit canAddBody (false);
}

void
StructuralScene::remove (QString uid, QStrMap stgs)
{
  CPR_ASSERT (hasEntity (uid));

  StructuralEntity *e = entity (uid);
  StructuralEntity *p = e->structuralParent ();
  StructuralType type = e->structuralType ();

  if (stgs.value (ST_SETTINGS_UNDO_TRACE) != ST_VALUE_FALSE)
    stgs.insert (ST_SETTINGS_UNDO, ST_VALUE_TRUE);

  removeChildren (e, stgs);

  removeEdges (e, util::createSettings (ST_VALUE_TRUE,
                                        stgs.value (ST_SETTINGS_NOTIFY),
                                        stgs.value (ST_SETTINGS_CODE)));

  if (type == Structural::Link || type == Structural::Bind)
    removeParams (e, stgs);

  if (e->structuralType () == Structural::Port)
  {
    StructuralEntity *target = nullptr;
    QString compUid = e->property (ST_ATTR_REF_COMPONENT_UID);
    QString interfUid = e->property (ST_ATTR_REF_INTERFACE_UID);

    if (hasEntity (interfUid))
      target = entity (interfUid);
    else if (hasEntity (compUid))
      target = entity (compUid);

    if (target)
      target->setProperty (ST_ATTR_ENT_AUTOSTART, ST_VALUE_FALSE);
  }

  if (p)
    p->removeChild (e);
  else
    removeItem (e);

  removeFromMap (uid);
  delete e;

  //  if (uid == _selected)
  //    emit select ("", stgs);

  //  if (type == Structural::Body)
  //    emit canAddBody (true);

  if (stgs[ST_SETTINGS_NOTIFY] == ST_VALUE_TRUE)
    emit removed (uid, stgs);
}

void
StructuralScene::removeEdges (StructuralEntity *ent, const QStrMap &stgs)
{
  CPR_ASSERT (hasEntity (ent->uid ()));
  for (StructuralEdge *edge : edges (ent->uid ()))
  {
    _view->removeEntity (edge->uid (), stgs);
  }
}

void
StructuralScene::removeChildren (StructuralEntity *ent, const QStrMap &stgs)
{
  CPR_ASSERT (hasEntity (ent->uid ()));
  while (!ent->children ().isEmpty ())
  {
    _view->removeEntity (ent->children ().first ()->uid (), stgs);
  }
}

void
StructuralScene::removeParams (StructuralEntity *ent, const QStrMap &stgs)
{
  CPR_ASSERT (hasEntity (ent->uid ()));
  for (const QString &key : ent->properties ().keys ())
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

void
StructuralScene::change (QString uid, QStrMap props, QStrMap stgs)
{
  CPR_ASSERT (hasEntity (uid));

  StructuralEntity *ent = entity (uid);
  auto *comp = cast (StructuralComposition *, ent);

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

      if (hasEntity (interfUid))
        lasttarget = entity (interfUid);
      else if (hasEntity (compUid))
        lasttarget = entity (compUid);

      if (lasttarget)
        lasttarget->setProperty (ST_ATTR_ENT_AUTOSTART, ST_VALUE_FALSE);
    }
  }

  ent->setProperties (props);
  _view->adjustReferences (ent);
  ent->adjust (true);

  if (stgs[ST_SETTINGS_NOTIFY] == ST_VALUE_TRUE)
    emit changed (uid, props, prev, stgs);
}

void
StructuralScene::updateAngle (StructuralBind *bind)
{
  CPR_ASSERT (bind->hasOrigin () && bind->hasDestination ());
  int min = INT_MAX;
  int max = INT_MIN;

  for (StructuralEdge *e : parallelEdges (bind))
  {
    int cur = e->angle ();

    if (e->destination ()->structuralType () != Structural::Link)
      cur *= -1;

    max = std::max (max, cur);
    min = std::min (min, cur);
  }

  qreal angle = 0;
  if (min != INT_MAX || max != INT_MIN)
  {
    if (max <= abs (min))
      angle = max + 60;
    else if (max > abs (min))
      angle = min - 60;
  }

  if (bind->destination ()->structuralType () != Structural::Link)
    angle *= -1;

  bind->setAngle (angle);
}
