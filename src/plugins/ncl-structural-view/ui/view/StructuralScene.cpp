#include "StructuralScene.h"

#include "StructuralView.h"
#include "util/Utilities.h"

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
  return _entities.contains (uid);
}

StructuralEntity *
StructuralScene::entity (const QString &uid)
{
  CPR_ASSERT (hasEntity (uid));
  return _entities.value (uid, nullptr);
}

QMap<QString, StructuralEntity *> &
StructuralScene::entities ()
{
  return _entities;
}

QMap<QString, QString> &
StructuralScene::refs ()
{
  return _refs;
}

QList<StructuralEntity *>
StructuralScene::entitiesByAttrId (const QString &id)
{
  QList<StructuralEntity *> list;
  for (StructuralEntity *e : _entities.values ())
  {
    if (e->id () == id)
      list.append (e);
  }

  return list;
}

QString
StructuralScene::createNewId (StructuralType type, const QString &customPrefix)
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
  // TODO: remove entities
  _refs.clear ();
  _entities.clear ();

  QGraphicsScene::clear ();
}

StructuralEntity *
StructuralScene::body ()
{
  StructuralEntity *body = nullptr;
  for (StructuralEntity *ent : _entities.values ())
  {
    if (ent->structuralType () == Structural::Body)
    {
      body = ent;
      break;
    }
  }

  return body;
}

#define for_each_qelem_child(E, P)                                            \
  for (QDomElement E = P.firstChildElement (); !E.isNull ();                  \
       E = E.nextSiblingElement ())

#define for_each_qelem_child_of_type(E, P, T)                                 \
  for (QDomElement E = P.firstChildElement (T); !E.isNull ();                 \
       E = E.nextSiblingElement (T))

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
    if (elt.nodeName () == "entity")
    {
      load (elt, root);
    }
    else if (elt.nodeName () == "reference")
    {
      CPR_ASSERT (!elt.attribute ("uid").isEmpty ());
      CPR_ASSERT (!elt.attribute ("refer").isEmpty ());
      _refs.insert (elt.attribute ("uid"), elt.attribute ("refer"));
    }
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

  _view->insert (entUid, parentUid, props, stgs);

  for_each_qelem_child_of_type (child, elt, "entity") { load (child, elt); }
}

QString
StructuralScene::save ()
{
  QDomDocument *doc = new QDomDocument ();
  QDomElement root = doc->createElement ("structural");

  for (const QString &key : _refs.keys ())
  {
    CPR_ASSERT (_entities.contains (key)
                && _entities.contains (_refs.value (key)));

    QDomElement ref = doc->createElement ("reference");
    ref.setAttribute ("uid", key);
    ref.setAttribute ("refer", _refs.value (key));

    root.appendChild (ref);
  }

  if (ST_OPT_WITH_BODY)
  {
    for (StructuralEntity *e : _entities.values ())
    {
      if (e->structuralType () == Structural::Body)
        createXmlElement (e, doc, root);
    }
  }
  else
  {
    for (StructuralEntity *e : _entities.values ())
    {
      if (e->structuralParent () == nullptr)
        createXmlElement (e, doc, root);
    }
  }

  doc->appendChild (root);

  return doc->toString (4);
}

void
StructuralScene::createXmlElement (StructuralEntity *ent, QDomDocument *doc,
                                   QDomElement parent)
{
  if (ent->structuralType () != Structural::Reference)
  {
    QDomElement elt = doc->createElement ("entity");
    elt.setAttribute ("uid", ent->uid ());
    elt.setAttribute ("type", ent->structuralType ());

    for (const QString &key : ent->properties ().keys ())
      elt.setAttribute (key, ent->property (key));

    for (StructuralEntity *e : ent->children ())
      createXmlElement (e, doc, elt);

    parent.appendChild (elt);
  }
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
    e->setId (createNewId (e->structuralType ()));

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
