#ifndef STRUCTURALUTIL_H
#define STRUCTURALUTIL_H

#include <QMap>
#include <QObject>

#include "StructuralEntity.h"
#include <QLoggingCategory>

class StructuralEntity;

namespace StructuralUtil
{
QString createUid ();

QMap<QString, QString> createSettings ();
QMap<QString, QString> createSettings (bool undo, bool notify);
QMap<QString, QString> createSettings (const QString &undo,
                                       const QString &notify,
                                       const QString &code);

QMap<QString, QString> createCoreTranslations (StructuralType type);
QMap<QString, QString> createPluginTranslations (StructuralType type);

QString categoryToStr (StructuralCategory category);
StructuralCategory strToCategory (const QString &category);

QString typeToStr (StructuralType type);
StructuralType strToType (const QString &type);

QString roleToString (StructuralRole role);
StructuralRole strToRole (const QString &role);

QString mimeToStr (StructuralMimeType mimetype);
StructuralMimeType strToMime (const QString &mimetype);

QString getIcon (StructuralType type);
QString getColor (StructuralType type);
QString getPrefix (StructuralType type);
QString getTooltip (StructuralType type, const QString &title = "",
                    const QString &info = "", const QString &warning = "",
                    const QString &error = "", const QString &extra = "");

QString getMimeTypeIcon (StructuralMimeType mimetype);
QString getMimeTooltip (StructuralMimeType mimetype, const QString &title = "",
                        const QString &info = "", const QString &warning = "",
                        const QString &error = "", const QString &extra = "");
StructuralMimeType getMimeByExtension (const QString &ext);

QString getIcon (StructuralRole role);

QVector<StructuralEntity *> getNeighbors (StructuralEntity *entity);
QVector<StructuralEntity *> getUpNeighbors (StructuralEntity *entity);

bool isCondition (StructuralRole role);
bool isCondition (const QString &role);
bool isAction (StructuralRole role);
bool isAction (const QString &role);

void adjustEdges (StructuralEntity *entity);

QString formatId (const QString &id);

bool validateKinship (StructuralType type, StructuralType parent);
}

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

Q_DECLARE_LOGGING_CATEGORY (CPR_PLUGIN_STRUCT)

#endif // STRUCTURALUTIL_H
