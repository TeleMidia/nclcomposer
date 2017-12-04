#ifndef STRUCTURALUTIL_H
#define STRUCTURALUTIL_H

#include <QMap>
#include <QDomElement>
#include <QObject>

#include "util/Utilities.h"
#include "StructuralEntity.h"
#include <QLoggingCategory>

class StructuralEntity;

namespace StructuralUtil
{
QString createUid ();

QStrMap createSettings (bool undo = true, bool notify = true);
QStrMap createSettings (const QString &undo, const QString &notify,
                        const QString &code);

QStrMap struct2nclTranslations (StructuralType type);
QStrMap ncl2structTranslations (StructuralType type);

QString categorytostr (StructuralCategory category);
StructuralCategory strtocategory (const QString &category);
StructuralCategory categoryfromtype (StructuralType type);

QString typetostr (StructuralType type);
StructuralType strtotype (const QString &type);

QString roletostr (StructuralRole role);
StructuralRole strtorole (const QString &role);

QString mimetostr (StructuralMimeType mimetype);
StructuralMimeType strtomime (const QString &mimetype);

QString icon (StructuralType type);
QString color (StructuralType type);
QString prefix (StructuralType type);
QString tooltip (StructuralType type, const QString &title = "",
                 const QString &info = "", const QString &warning = "",
                 const QString &error = "", const QString &extra = "");

QString mimeicon (StructuralMimeType mimetype);
QString mimetooltip (StructuralMimeType mimetype, const QString &title = "",
                     const QString &info = "", const QString &warning = "",
                     const QString &error = "", const QString &extra = "");
StructuralMimeType exttomime (const QString &ext);

QString icon (StructuralRole role);

QVector<StructuralEntity *> neighbors (StructuralEntity *entity);
QVector<StructuralEntity *> upneighbors (StructuralEntity *entity);

bool isCondition (StructuralRole role);
bool isCondition (const QString &role);
bool isAction (StructuralRole role);
bool isAction (const QString &role);

void adjustEdges (StructuralEntity *entity);

QString formatId (const QString &id);

bool validateKinship (StructuralType type, StructuralType parent);

QStrMap qdom_attrs_to_qmap (const QDomNamedNodeMap &attrs);
}

// Some QDom macro utilities.
#define for_each_qelem_child(E, P)                                            \
  for (QDomElement E = P.firstChildElement (); !E.isNull ();                  \
       E = E.nextSiblingElement ())

#define for_each_qelem_child_of_type(E, P, T)                                 \
  for (QDomElement E = P.firstChildElement (T); !E.isNull ();                 \
       E = E.nextSiblingElement (T))

template <typename K, typename V>
std::pair<V, K>
flip (const std::pair<K, V> &pair)
{
  return std::make_pair (pair.second, pair.first);
}

template <typename K, typename V>
std::map<K, V>
invert (const std::map<V, K> &map)
{
  std::map<K, V> inverted;
  std::transform (map.begin (), map.end (),
                  std::inserter (inverted, inverted.begin ()), flip<V, K>);

  return inverted;
}

/*!
 * Gets a copy of only the pairs that have the keys passed as param.
 */
template <typename K, typename V>
QMap<K, V>
qmap_filter_by_keys (const QMap<K,V> &map, const QVector <K> &keys)
{
  QMap <K, V> ret;
  for (K k : keys)
  {
    CPR_ASSERT (map.count (k));
    ret.insert (k, map[k]);
  }
  return ret;
}

/*!
 * Copy all key/values of orig to dest.
 */
template <typename K, typename V>
void
qmap_insert_all (QMap<K,V> &dest, const QMap<K,V> &orig)
{
  for (K k : orig.keys ())
    dest.insert (k, orig[k]);
}

Q_DECLARE_LOGGING_CATEGORY (CPR_PLUGIN_STRUCT)

#endif // STRUCTURALUTIL_H
